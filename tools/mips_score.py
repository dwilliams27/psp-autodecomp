#!/usr/bin/env python3
"""Instruction-aware scoring for the SNC permuter.

The byte-diff objective (``permuter.score_bytes``) is blind to the structure
of MIPS code: a single wrong register spelling early in a function shifts every
later branch displacement, so the raw byte diff explodes (4142/5640 on
``gcDoEntitySpawn::operator=``) even though the instruction stream is almost
correct. The hill-climber then has no usable gradient.

This module decodes both byte streams into MIPS32 instructions and produces a
*guide score* that masks the branch-displacement cascade while keeping the real
signal (opcodes, register operands, load/store offsets, ALU immediates). The
guide score is for RANKING only. The win condition stays the relocation-masked
raw byte diff (``raw_score``), so a match is only ever declared on
``raw_score == 0``.

Contract (see project task brief):
  decode_insn(word) -> InsnFields
  score_instructions(compiled, expected, relocs, weights=None)
      -> (guide_score, raw_score)

little-endian MIPS32 (Allegrex). Reuses ``mask_relocation_bytes`` from common.
"""

import os
import struct
import sys
from dataclasses import dataclass
from functools import lru_cache

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from common import mask_relocation_bytes  # noqa: E402


# ---------------------------------------------------------------------------
# Instruction decoding
# ---------------------------------------------------------------------------

# Instruction "kind" classifies how operands are laid out, which drives both
# branch-masking and field-level comparison.
KIND_RTYPE = "rtype"          # opcode 0: rs,rt,rd,shamt,funct
KIND_ITYPE = "itype"          # generic immediate (ALU imm, lui, ...)
KIND_MEM = "mem"              # load/store: base(rs), rt, signed offset
KIND_BRANCH = "branch"        # PC-relative conditional/unconditional branch
KIND_JUMP = "jump"            # j / jal absolute 26-bit target
KIND_REGIMM = "regimm"        # opcode 1: bltz/bgez/... (branch on one reg)
KIND_COP = "cop"              # coprocessor (fpu/cop0/cop2) — opaque payload
KIND_OTHER = "other"          # everything else (treated opaquely)


@dataclass(frozen=True)
class InsnFields:
    """Decoded fields of one 32-bit MIPS instruction (little-endian word)."""

    word: int
    opcode: int
    rs: int
    rt: int
    rd: int
    shamt: int
    funct: int
    imm: int          # raw 16-bit immediate (unsigned bit pattern)
    target: int       # raw 26-bit jump target field
    kind: str

    # Convenience: sign-extended 16-bit immediate.
    @property
    def simm(self):
        return self.imm - 0x10000 if self.imm & 0x8000 else self.imm


# Opcodes that are PC-relative branches (operands are register(s), the 16-bit
# field is a displacement we mask out for the guide score).
_BRANCH_OPCODES = {
    0x04,  # beq
    0x05,  # bne
    0x06,  # blez
    0x07,  # bgtz
    0x14,  # beql
    0x15,  # bnel
    0x16,  # blezl
    0x17,  # bgtzl
}

# REGIMM (opcode 1) rt-encoded branches: bltz/bgez/bltzl/bgezl/bltzal/...
_REGIMM_OPCODE = 0x01

# Jump opcodes: 26-bit absolute target.
_JUMP_OPCODES = {0x02, 0x03}  # j, jal

# Integer GPR load/store opcodes (base in rs is a GPR; the 16-bit signed
# offset is REAL signal — keep it; rs/rt are GPR lockers).
# Deliberately EXCLUDES the FP/VFPU load/store family (lwc1/swc1 0x31/0x39,
# and the Allegrex VFPU lv/sv quadword ops 0x35/0x36/0x3d/0x3e): those carry
# FP/VFPU register fields, not freely-allocated GPRs, so they must NOT have
# their register fields masked as cheap "lockers" — they fall to KIND_OTHER
# and are compared full-word (conservative; never silently equated).
_MEM_OPCODES = {
    0x20,  # lb
    0x21,  # lh
    0x22,  # lwl
    0x23,  # lw
    0x24,  # lbu
    0x25,  # lhu
    0x26,  # lwr
    0x28,  # sb
    0x29,  # sh
    0x2a,  # swl
    0x2b,  # sw
    0x2e,  # swr
    0x30,  # ll
    0x38,  # sc
}

# GPR immediate opcodes: rs/rt are GPRs (lockers), imm is real signal.
_ITYPE_OPCODES = {
    0x08,  # addi
    0x09,  # addiu
    0x0a,  # slti
    0x0b,  # sltiu
    0x0c,  # andi
    0x0d,  # ori
    0x0e,  # xori
    0x0f,  # lui
}

# Coprocessor instruction opcodes — opaque payloads we do not crack open.
_COP_OPCODES = {0x10, 0x11, 0x12, 0x13, 0x18, 0x1c}


def decode_insn(word):
    """Decode one little-endian MIPS32 instruction word into ``InsnFields``.

    ``word`` is the 32-bit value as stored (already host int). Returns the
    standard field breakdown plus a ``kind`` classification.
    """
    word &= 0xFFFFFFFF
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    target = word & 0x03FFFFFF

    if opcode == 0x00:
        kind = KIND_RTYPE
    elif opcode == _REGIMM_OPCODE:
        kind = KIND_REGIMM
    elif opcode in _JUMP_OPCODES:
        kind = KIND_JUMP
    elif opcode in _BRANCH_OPCODES:
        kind = KIND_BRANCH
    elif opcode in _MEM_OPCODES:
        kind = KIND_MEM
    elif opcode in _ITYPE_OPCODES:
        kind = KIND_ITYPE
    elif opcode in _COP_OPCODES:
        kind = KIND_COP
    else:
        # Unknown / FP / VFPU / anything we do not confidently model as
        # GPR-bearing. Compared full-word (conservative): never register-masked.
        kind = KIND_OTHER

    return InsnFields(
        word=word,
        opcode=opcode,
        rs=rs,
        rt=rt,
        rd=rd,
        shamt=shamt,
        funct=funct,
        imm=imm,
        target=target,
        kind=kind,
    )


@lru_cache(maxsize=8)
def _decode_stream(data):
    """Decode a byte string into a tuple of ``InsnFields`` (4 bytes = 1 word).

    Cached (bytes are hashable, InsnFields is frozen): the fixed ``expected``
    stream is decoded once per worker instead of once per candidate. Returns an
    immutable tuple so cached results can never be mutated by a caller.
    """
    n = len(data) // 4
    words = struct.unpack("<%dI" % n, data[: n * 4])
    return tuple(decode_insn(w) for w in words)


# ---------------------------------------------------------------------------
# Comparison keys
# ---------------------------------------------------------------------------

def _opcode_key(insn):
    """Coarse key for alignment / LCS: identifies the *operation*, not operands.

    For R-type the funct is the real operation selector, so it is part of the
    key. For everything else the opcode (plus regimm rt sub-selector) suffices.
    """
    if insn.kind == KIND_RTYPE:
        return ("r", insn.funct)
    if insn.kind == KIND_REGIMM:
        # rt selects bltz/bgez/etc.; it is the operation, not a data register.
        return ("regimm", insn.rt)
    if insn.kind == KIND_COP:
        # Keep the cop selector (rs) so movc/cvt/etc. don't collapse together.
        return ("cop", insn.opcode, insn.rs)
    if insn.kind == KIND_OTHER:
        return ("other", insn.opcode)
    return ("i", insn.opcode)


def _structure_equal(a, b):
    """True if the two aligned instructions match in everything that is REAL
    signal: opcode/funct, the structural shape, and any non-register
    immediate/offset payload — but IGNORING register fields and IGNORING
    branch/jump displacements (those are masked for the guide).

    A return of True with non-identical registers => the "locker problem":
    right instruction, wrong register.
    """
    if a.opcode != b.opcode or a.kind != b.kind:
        return False

    if a.kind == KIND_RTYPE:
        # funct + shamt are the operation/structure; registers ignored.
        return a.funct == b.funct and a.shamt == b.shamt

    if a.kind == KIND_REGIMM:
        # rt is the branch selector (operation). Displacement masked.
        return a.rt == b.rt

    if a.kind == KIND_BRANCH:
        # opcode already equal; displacement (imm) masked. Registers ignored.
        return True

    if a.kind == KIND_JUMP:
        # 26-bit target masked. opcode equal => structurally same.
        return True

    if a.kind == KIND_MEM:
        # Load/store OFFSET is real signal — keep it. Registers ignored.
        return a.imm == b.imm

    if a.kind == KIND_COP:
        # Opaque payload: compare everything except nothing-specific. Cop ops
        # rarely carry GP registers we permute, so require full bit equality
        # minus the (already-equal) opcode. Compare the remaining 26 bits.
        return (a.word & 0x03FFFFFF) == (b.word & 0x03FFFFFF)

    if a.kind == KIND_OTHER:
        # Unknown / FP / VFPU: full-word equality (opcode already matched). Any
        # remaining bit difference is structural — never masked as a locker.
        return a.word == b.word

    # KIND_ITYPE: generic immediate (addiu/andi/ori/lui/slti/...). The
    # immediate is real signal; registers ignored.
    return a.imm == b.imm


def _registers_equal(a, b):
    """True if the register fields relevant to this kind are identical.

    Only called when ``_structure_equal`` is already True, to distinguish a
    full structural match (registers correct) from a locker miss (registers
    wrong). Branch/jump displacements never enter here.
    """
    if a.kind == KIND_RTYPE:
        return a.rs == b.rs and a.rt == b.rt and a.rd == b.rd
    if a.kind == KIND_REGIMM:
        return a.rs == b.rs
    if a.kind == KIND_BRANCH:
        return a.rs == b.rs and a.rt == b.rt
    if a.kind == KIND_JUMP:
        return True  # no register operands
    if a.kind == KIND_MEM:
        return a.rs == b.rs and a.rt == b.rt
    if a.kind == KIND_COP:
        return True  # payload already fully compared in _structure_equal
    if a.kind == KIND_OTHER:
        return True  # full word already compared in _structure_equal
    # KIND_ITYPE
    return a.rs == b.rs and a.rt == b.rt


# ---------------------------------------------------------------------------
# Alignment
# ---------------------------------------------------------------------------

def _lcs_alignment(a_keys, b_keys):
    """Longest-common-subsequence alignment over opcode keys.

    Returns a list of (i, j) aligned index pairs (matched positions). Used only
    when the two instruction streams differ in length, so a length-changing
    mutation still produces a finite, meaningful score instead of inf.
    """
    n, m = len(a_keys), len(b_keys)
    # DP table of LCS lengths.
    dp = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        row = dp[i]
        row_next = dp[i + 1]
        ak = a_keys[i]
        for j in range(m - 1, -1, -1):
            if ak == b_keys[j]:
                row[j] = row_next[j + 1] + 1
            else:
                row[j] = row_next[j] if row_next[j] >= row[j + 1] else row[j + 1]
    # Backtrack to collect matched pairs.
    pairs = []
    i = j = 0
    while i < n and j < m:
        if a_keys[i] == b_keys[j]:
            pairs.append((i, j))
            i += 1
            j += 1
        elif dp[i + 1][j] >= dp[i][j + 1]:
            i += 1
        else:
            j += 1
    return pairs


def _cost_for_pairs(a, b, matched, len_diff):
    """Given an alignment (list of (i, j) matched pairs), return
    ``(struct_diff, reg_diff)``.

    Matched pairs are classified structurally then by register equality. Every
    UNMATCHED instruction beyond the unavoidable length delta is a genuine
    structural substitution (not a pure insert/delete) and counts as a struct
    diff.
    """
    struct_diff = 0
    reg_diff = 0
    for ia, ib in matched:
        xa, xb = a[ia], b[ib]
        if not _structure_equal(xa, xb):
            struct_diff += 1
        elif not _registers_equal(xa, xb):
            reg_diff += 1
    unmatched = max(len(a), len(b)) - len(matched)
    struct_diff += max(0, unmatched - len_diff)
    return struct_diff, reg_diff


def _best_alignment_cost(a, b, len_diff):
    """Return the lowest ``(struct_diff, reg_diff)`` over candidate alignments.

    Two alignments are considered:

    * **1:1 index alignment** — the naive position-for-position pairing. Cheap,
      and exact when the streams are genuinely positionally aligned (the truly
      identical / pure register-spelling case).
    * **LCS over opcode-only keys** — realigns around local insert/delete shifts
      so that a single early register swap that nudges a handful of inlined
      instructions forward/back is NOT misread as a long structural cascade.

    For equal-length REG_ALLOC near-misses (e.g. gcDoEntitySpawn::operator=)
    forced 1:1 alignment misattributes net-zero local shifts as ~1200 false
    structural diffs and buries the register gradient; LCS recovers the real
    picture (a handful of struct diffs, hundreds of register-only lockers). We
    take whichever alignment yields the lower structural cost so the guide score
    can never regress relative to naive 1:1, while exposing the locker gradient
    when realignment helps. Ranking prefers fewer structural diffs first (weight
    W_struct >> W_reg), then fewer register lockers as a tie-break.
    """
    # 1:1 index alignment over the common prefix length.
    n = min(len(a), len(b))
    one_to_one = [(i, i) for i in range(n)]
    s1, r1 = _cost_for_pairs(a, b, one_to_one, len_diff)

    # Fast path: if 1:1 already has zero structural diffs, LCS cannot do better
    # on the (dominant) structural term, so skip the O(n*m) DP entirely. This is
    # the common register-only / late-search case. Safe because the tie-break
    # below prefers fewer structural diffs first (W_struct >> W_reg).
    if s1 == 0:
        return s1, r1

    # LCS realignment over opcode-only keys.
    a_keys = [_opcode_key(x) for x in a]
    b_keys = [_opcode_key(x) for x in b]
    matched = _lcs_alignment(a_keys, b_keys)
    s2, r2 = _cost_for_pairs(a, b, matched, len_diff)

    # Prefer the alignment with fewer structural diffs (the W_struct term
    # dominates); break ties on fewer register lockers.
    if (s1, r1) <= (s2, r2):
        return s1, r1
    return s2, r2


# ---------------------------------------------------------------------------
# Scoring
# ---------------------------------------------------------------------------

DEFAULT_WEIGHTS = {
    "len": 10000,    # per instruction of length difference — fix size first
    "struct": 1000,  # per structurally-different aligned pair
    "reg": 1,        # per pair identical except a register field (locker)
}


def raw_byte_score(compiled, expected, relocs):
    """Relocation-masked raw differing-byte count. Identical to
    ``permuter.score_bytes`` for equal-length inputs; returns ``inf`` on a
    size mismatch (the win-condition objective)."""
    if compiled is None or len(compiled) != len(expected):
        return float("inf")
    if relocs:
        c = mask_relocation_bytes(compiled, relocs)
        e = mask_relocation_bytes(expected, relocs)
    else:
        c, e = compiled, expected
    return sum(1 for x, y in zip(c, e) if x != y)


def score_instructions(compiled, expected, relocs, weights=None):
    """Score ``compiled`` against ``expected`` at instruction granularity.

    Returns ``(guide_score, raw_score)``:

    * ``guide_score`` — ranking/hill-climbing objective. Masks the
      branch-displacement cascade and relocation bytes; weighted sum of length
      difference, structural differences, and register-only (locker) misses.
      ``guide_score == 0`` iff the streams are structurally identical with
      correct registers (branch displacements aside).
    * ``raw_score`` — relocation-masked raw byte diff (== score_bytes). The
      WIN CONDITION. ``inf`` on compile failure / size mismatch.

    SAFETY INVARIANT: because the guide masks branch displacements, a
    branch-offset-only residual yields ``guide_score == 0`` while
    ``raw_score > 0``. Callers MUST declare an exact match only on
    ``raw_score == 0``, never on ``guide_score == 0`` alone.
    """
    w = dict(DEFAULT_WEIGHTS)
    if weights:
        w.update(weights)

    raw = raw_byte_score(compiled, expected, relocs)

    if compiled is None:
        return float("inf"), raw

    a = _decode_stream(compiled)   # compiled
    b = _decode_stream(expected)   # expected

    len_diff = abs(len(a) - len(b))

    struct_diff, reg_diff = _best_alignment_cost(a, b, len_diff)

    guide = (
        w["len"] * len_diff
        + w["struct"] * struct_diff
        + w["reg"] * reg_diff
    )
    return guide, raw


# ---------------------------------------------------------------------------
# Self-check / decoder validation against objdump
# ---------------------------------------------------------------------------

def _objdump_fields(o_path):
    """Run objdump and parse (offset, word, mnemonic, operand-registers).

    Returns list of (word_int, mnemonic, regset) where regset is the set of
    register tokens objdump printed (e.g. {'sp','s2'}). Used to cross-check the
    decoder's opcode/register extraction.
    """
    import re
    import subprocess

    from common import OBJDUMP

    out = subprocess.check_output(
        [OBJDUMP, "-d", o_path], stderr=subprocess.DEVNULL
    ).decode("utf-8", "replace")

    rows = []
    line_re = re.compile(r"^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(\S+)(.*)$")
    reg_re = re.compile(r"\b([a-z][a-z0-9]{1,2})\b")
    # MIPS o32 register names objdump emits.
    regnames = {
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "s8", "fp", "ra",
    }
    for line in out.splitlines():
        m = line_re.match(line)
        if not m:
            continue
        word = int(m.group(1), 16)
        # objdump prints big-endian hex of the bytes; for elf32-tradlittlemips
        # the displayed word is already the instruction word (not byte-swapped).
        mnem = m.group(2)
        ops = m.group(3)
        regs = {t for t in reg_re.findall(ops) if t in regnames}
        rows.append((word, mnem, regs))
    return rows


_REGNUM_TO_NAME = [
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra",
]


def _self_check(paths, limit=400):
    """Validate decode_insn opcode/register extraction against objdump.

    For each GPR-using instruction objdump prints, confirm every register name
    objdump shows is among the GPR fields the decoder extracted for that kind.
    Reports a mismatch count. Returns (checked, mismatches).
    """
    checked = 0
    mismatches = 0
    examples = []

    # objdump prints fp as the name for s8 in some configs; accept both.
    alias = {"fp": "s8", "s8": "s8"}

    for path in paths:
        if not os.path.exists(path):
            print(f"  skip (missing): {path}")
            continue
        rows = _objdump_fields(path)
        for word, mnem, regs in rows:
            if checked >= limit:
                break
            insn = decode_insn(word)

            # Determine which decoder GPR fields are "live" for this kind, and
            # map to names.
            live = set()
            if insn.kind == KIND_RTYPE:
                live = {insn.rs, insn.rt, insn.rd}
            elif insn.kind == KIND_REGIMM:
                live = {insn.rs}
            elif insn.kind == KIND_BRANCH:
                live = {insn.rs, insn.rt}
            elif insn.kind == KIND_MEM:
                live = {insn.rs, insn.rt}
            elif insn.kind == KIND_ITYPE:
                live = {insn.rs, insn.rt}
            # JUMP/COP: no GPR cross-check (objdump shows targets/cop regs).
            if insn.kind in (KIND_JUMP, KIND_COP):
                continue

            live_names = {_REGNUM_TO_NAME[r] for r in live}
            norm_objdump = {alias.get(r, r) for r in regs}
            norm_live = {alias.get(r, r) for r in live_names}

            checked += 1
            # Every register objdump printed must be derivable from a live
            # decoder field. (Decoder may list extras like an unused rt=0.)
            if not norm_objdump.issubset(norm_live):
                mismatches += 1
                if len(examples) < 12:
                    examples.append(
                        f"{word:08x} {mnem} objdump={sorted(norm_objdump)} "
                        f"decoder={sorted(norm_live)} kind={insn.kind}"
                    )
        if checked >= limit:
            break

    print(f"decoder self-check: {checked} insns checked, {mismatches} mismatches")
    for ex in examples:
        print("  MISMATCH:", ex)
    return checked, mismatches


def _synthetic_score_check():
    """Sanity-check the scorer's invariants on hand-built streams."""
    # Two identical instructions -> guide 0, raw 0.
    addiu = struct.pack("<I", 0x27BDFF90)  # addiu sp,sp,-112
    g, r = score_instructions(addiu, addiu, [])
    assert g == 0 and r == 0, (g, r)

    # Same instruction except destination register (rt) differs by 1 ->
    # locker miss: guide == W_reg, raw > 0.
    addiu2_word = 0x27BDFF90 ^ (1 << 16)  # flip a bit in rt
    addiu2 = struct.pack("<I", addiu2_word)
    g, r = score_instructions(addiu2, addiu, [])
    assert g == DEFAULT_WEIGHTS["reg"], (g, r)
    assert r > 0, r

    # Branch displacement-only difference -> guide 0 but raw > 0
    # (the SAFETY INVARIANT). beq a0,a1,off
    beq_a = struct.pack("<I", 0x10850004)  # beq a0,a1,+4
    beq_b = struct.pack("<I", 0x10850040)  # beq a0,a1,+64 (different displacement)
    g, r = score_instructions(beq_a, beq_b, [])
    assert g == 0, ("branch-disp guide should be 0", g)
    assert r > 0, ("branch-disp raw should be >0", r)

    # Different opcode -> structural difference.
    ori = struct.pack("<I", 0x34840001)  # ori a0,a0,1
    g, r = score_instructions(ori, addiu, [])
    assert g == DEFAULT_WEIGHTS["struct"], (g, r)

    # Load/store offset difference IS structural (kept).
    lw_a = struct.pack("<I", 0x8E440008)  # lw a0,8(s2)
    lw_b = struct.pack("<I", 0x8E440010)  # lw a0,16(s2)
    g, r = score_instructions(lw_a, lw_b, [])
    assert g == DEFAULT_WEIGHTS["struct"], ("mem offset must be structural", g, r)

    # Length difference dominates.
    g, r = score_instructions(addiu + ori, addiu, [])
    assert g >= DEFAULT_WEIGHTS["len"], (g, r)
    assert r == float("inf"), r  # size mismatch -> raw inf

    print("synthetic score check: OK")


if __name__ == "__main__":
    args = sys.argv[1:]
    repo = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    os.chdir(repo)

    _synthetic_score_check()

    if args:
        paths = args
    else:
        paths = ["expected/build/func/00148aa0.o"]
        # Add a few build/src objects if present.
        import glob
        paths += sorted(glob.glob("build/src/*.o"))[:3]

    checked, mismatches = _self_check(paths, limit=600)
    if mismatches:
        sys.exit(1)
