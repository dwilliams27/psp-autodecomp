#!/usr/bin/env python3
"""Adversarial ground-truth tests for tools/mips_score.py.

These tests do NOT trust the implementation's own decoder. Expected
instruction fields are derived independently from
``mipsel-linux-gnu-objdump`` run over real .o bytes
(expected/build/func/00148aa0.o, the gcDoEntitySpawn::operator= near-miss),
plus a small set of hand-decoded words computed from first principles.

What is covered (per the mips_score contract):
  (a) decode_insn field correctness on a sample of real instructions.
  (b) score_instructions == (0, 0) on identical input.
  (c) a single register-only word difference -> guide carries W_reg, raw > 0.
  (d) THE SAFETY TEST: a branch-displacement-only difference -> guide_score == 0
      but raw_score > 0. Exact match must NOT be declared on guide alone.
  (e) a structural difference (different opcode) -> W_struct.

Run from the repo root:
    python3 -m pytest tools/test_mips_score.py -v
"""

from __future__ import annotations

import os
import re
import struct
import subprocess
import sys

import pytest

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)

# mips_score is produced by the Foundations agent. Until it lands, skip loudly
# (collection-time skip, not a silent pass) so the suite stays green but visible.
mips_score = pytest.importorskip(
    "mips_score",
    reason="tools/mips_score.py not present yet (built by Foundations agent)",
)

OBJDUMP = "mipsel-linux-gnu-objdump"
EXPECTED_O = os.path.join(REPO, "expected", "build", "func", "00148aa0.o")

# o32 register name -> number, as emitted by objdump operands.
REG = {
    "zero": 0, "at": 1, "v0": 2, "v1": 3, "a0": 4, "a1": 5, "a2": 6, "a3": 7,
    "t0": 8, "t1": 9, "t2": 10, "t3": 11, "t4": 12, "t5": 13, "t6": 14,
    "t7": 15, "s0": 16, "s1": 17, "s2": 18, "s3": 19, "s4": 20, "s5": 21,
    "s6": 22, "s7": 23, "t8": 24, "t9": 25, "k0": 26, "k1": 27, "gp": 28,
    "sp": 29, "fp": 30, "s8": 30, "ra": 31,
}


# ---------------------------------------------------------------------------
# Independent (objdump / first-principles) ground truth
# ---------------------------------------------------------------------------

def _objdump_lines():
    """Return list of (word:int, mnemonic:str, operand_str:str) from real .o."""
    if not os.path.exists(EXPECTED_O):
        pytest.skip(f"missing ground-truth object: {EXPECTED_O}")
    out = subprocess.run(
        [OBJDUMP, "-d", EXPECTED_O],
        capture_output=True, text=True, check=True,
    ).stdout
    rows = []
    # e.g.  "      10:\t8e440008 \tlw\ta0,8(s2)"
    line_re = re.compile(
        r"^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(\S+)(?:\s+(\S.*?))?\s*$"
    )
    for line in out.splitlines():
        m = line_re.match(line)
        if not m:
            continue
        word = int(m.group(1), 16)
        mnem = m.group(2)
        operands = (m.group(3) or "").strip()
        rows.append((word, mnem, operands))
    return rows


def _ref_decode(word: int):
    """First-principles little-endian MIPS32 field extraction (not from SUT)."""
    return {
        "opcode": (word >> 26) & 0x3F,
        "rs": (word >> 21) & 0x1F,
        "rt": (word >> 16) & 0x1F,
        "rd": (word >> 11) & 0x1F,
        "shamt": (word >> 6) & 0x1F,
        "funct": word & 0x3F,
        "imm": word & 0xFFFF,
    }


def _field(fields, name):
    """Read a field from decode_insn output, supporting dict or attr style."""
    if isinstance(fields, dict):
        assert name in fields, f"decode_insn missing field {name!r}: {fields}"
        return fields[name]
    assert hasattr(fields, name), f"decode_insn missing attr {name!r}"
    return getattr(fields, name)


# ---------------------------------------------------------------------------
# (a) decode_insn field correctness on real instructions
# ---------------------------------------------------------------------------

def test_decode_matches_first_principles_over_real_object():
    """Every real word in the function must decode bit-for-bit per the manual."""
    rows = _objdump_lines()
    assert len(rows) > 100, "expected a large real function disassembly"
    checked = 0
    for word, _mnem, _ops in rows:
        ref = _ref_decode(word)
        got = mips_score.decode_insn(word)
        for fld in ("opcode", "rs", "rt", "rd", "shamt", "funct", "imm"):
            assert _field(got, fld) == ref[fld], (
                f"decode_insn({word:#010x}) field {fld}: "
                f"got {_field(got, fld)} expected {ref[fld]}"
            )
        checked += 1
    assert checked > 100


@pytest.mark.parametrize(
    "word,mnem",
    [
        (0x27BDFF90, "addiu"),   # addiu sp,sp,-112  -> opcode 9, imm signed -112
        (0xAFB20044, "sw"),      # sw s2,68(sp)      -> opcode 43, base sp, rt s2
        (0x8E440008, "lw"),      # lw a0,8(s2)       -> opcode 35
        (0x00809025, "or"),      # move s2,a0 == or  -> opcode 0, funct 0x25
        (0x10B0003C, "beq"),     # beq a1,s0         -> opcode 4
        (0x549E0003, "bnel"),    # bnel a0,s8        -> opcode 0x15 (21)
        (0x0C049DD7, "jal"),     # jal               -> opcode 3
        (0x00C0F809, "jalr"),    # jalr a2           -> opcode 0, funct 9
    ],
)
def test_decode_known_opcodes(word, mnem):
    """Spot-check opcode against hand-verified mnemonics from objdump."""
    expected_opcode = {
        "addiu": 9, "sw": 43, "lw": 35, "or": 0,
        "beq": 4, "bnel": 21, "jal": 3, "jalr": 0,
    }[mnem]
    got = mips_score.decode_insn(word)
    assert _field(got, "opcode") == expected_opcode


def test_decode_register_operands_against_objdump():
    """Cross-check rs/rt/rd of R-type and I-type ops with objdump operand text."""
    # lw a0,8(s2): rt=a0, base(rs)=s2, imm=8
    lw = mips_score.decode_insn(0x8E440008)
    assert _field(lw, "rt") == REG["a0"]
    assert _field(lw, "rs") == REG["s2"]
    assert _field(lw, "imm") == 8

    # sw s2,68(sp): rt=s2, base(rs)=sp
    sw = mips_score.decode_insn(0xAFB20044)
    assert _field(sw, "rt") == REG["s2"]
    assert _field(sw, "rs") == REG["sp"]

    # move s2,a0  == or s2,a0,zero : rd=s2, rs=a0, rt=zero
    mv = mips_score.decode_insn(0x00809025)
    assert _field(mv, "rd") == REG["s2"]
    assert _field(mv, "rs") == REG["a0"]
    assert _field(mv, "rt") == REG["zero"]

    # beq a1,s0 : rs=a1, rt=s0
    beq = mips_score.decode_insn(0x10B0003C)
    assert _field(beq, "rs") == REG["a1"]
    assert _field(beq, "rt") == REG["s0"]


# ---------------------------------------------------------------------------
# Helpers for scoring tests
# ---------------------------------------------------------------------------

def _words_to_bytes(words):
    """Little-endian pack a list of 32-bit words."""
    return b"".join(struct.pack("<I", w) for w in words)


# A short, real, branch-free prologue slice (decodes cleanly, no relocs).
PROLOGUE_WORDS = [
    0x27BDFF90,  # addiu sp,sp,-112
    0xAFB20044,  # sw    s2,68(sp)
    0x00809025,  # move  s2,a0
    0xAFB30048,  # sw    s3,72(sp)
    0x8E440008,  # lw    a0,8(s2)
    0x00A09825,  # move  s3,a1
    0x2405FFFC,  # li    a1,-4
    0x00852824,  # and   a1,a0,a1
]


# ---------------------------------------------------------------------------
# (b) identical input -> (0, 0)
# ---------------------------------------------------------------------------

def test_identical_streams_score_zero_zero():
    data = _words_to_bytes(PROLOGUE_WORDS)
    guide, raw = mips_score.score_instructions(data, data, relocs=[])
    assert guide == 0, f"identical input must have guide_score 0, got {guide}"
    assert raw == 0, f"identical input must have raw_score 0, got {raw}"


# ---------------------------------------------------------------------------
# (c) register-only difference -> guide has W_reg, raw > 0
# ---------------------------------------------------------------------------

def test_register_only_diff_costs_w_reg_and_raw():
    """move s2,a0 (00809025) vs move s3,a0 (00809825): only rd differs.

    Same opcode/funct/rs/rt -> classified as a register-only mismatch.
    Guide must be nonzero (the locker problem), raw must be > 0.
    """
    # 0x00809025 rd=18 (s2). Flip rd to 19 (s3): set bits[15:11]=19.
    base = 0x00809025
    flipped = (base & ~(0x1F << 11)) | (19 << 11)  # -> 0x00809825
    assert flipped == 0x00809825

    # Confirm independence of our construction: only rd changed.
    a = _ref_decode(base)
    b = _ref_decode(flipped)
    assert a["opcode"] == b["opcode"]
    assert a["funct"] == b["funct"]
    assert a["rs"] == b["rs"] and a["rt"] == b["rt"]
    assert a["rd"] != b["rd"]

    words_a = list(PROLOGUE_WORDS)
    words_b = list(PROLOGUE_WORDS)
    words_b[2] = flipped  # index 2 is the move

    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )

    assert raw > 0, "a real byte differs, raw must be > 0"
    assert guide > 0, "register mismatch must produce a nonzero guide_score"
    # The diff is exactly one register field. With default weights
    # (W_len=10000, W_struct=1000, W_reg=1) a single register-only pair
    # must score below the structural unit cost.
    assert guide < 1000, (
        f"register-only diff should cost W_reg (<1000), got guide={guide}; "
        "decoder likely misclassified a register swap as structural"
    )


# ---------------------------------------------------------------------------
# (d) THE SAFETY TEST: branch-displacement-only diff
# ---------------------------------------------------------------------------

def test_branch_displacement_only_is_guide_zero_but_raw_nonzero():
    """beq a1,s0 with two different jump offsets.

    Real branch:  beq a1,s0,0x1ec == 0x10B0003C  (imm field = 0x003c)
    Variant:      same beq, different displacement (imm field = 0x0099)

    opcode + rs + rt are identical; only the branch displacement differs.
    The guide MUST mask branch displacements -> guide_score == 0.
    The raw byte diff MUST still see it -> raw_score > 0.

    This is the safety invariant: a search ranking by guide could see 0 here,
    but the function is NOT byte-exact, so exact match must NOT be declared.
    """
    real_beq = 0x10B0003C
    # Change only the 16-bit displacement field.
    variant_beq = (real_beq & ~0xFFFF) | 0x0099
    assert variant_beq == 0x10B00099

    a = _ref_decode(real_beq)
    b = _ref_decode(variant_beq)
    assert a["opcode"] == b["opcode"] == 4  # beq
    assert a["rs"] == b["rs"] and a["rt"] == b["rt"]
    assert a["imm"] != b["imm"]

    words_a = PROLOGUE_WORDS + [real_beq, 0x00852824]
    words_b = PROLOGUE_WORDS + [variant_beq, 0x00852824]

    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )

    assert raw > 0, (
        "branch-displacement bytes differ; raw_score must remain > 0 so the "
        "WIN CONDITION still rejects this as a match"
    )
    assert guide == 0, (
        f"SAFETY VIOLATION: branch displacement must be masked in the guide, "
        f"expected guide_score == 0 but got {guide}. If guide treats the "
        "displacement as signal, the cascade gradient problem is not fixed."
    )


def test_bnel_displacement_is_also_masked():
    """Likely-variant branch (bnel) displacement must mask the same way."""
    real_bnel = 0x549E0003  # bnel a0,s8,+...
    variant = (real_bnel & ~0xFFFF) | 0x0040
    words_a = PROLOGUE_WORDS + [real_bnel]
    words_b = PROLOGUE_WORDS + [variant]
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert raw > 0
    assert guide == 0, (
        f"bnel displacement must be masked in guide; got guide={guide}"
    )


def test_jal_target_difference_is_masked_in_guide():
    """j-type target (jal) difference is a displacement; guide must mask it."""
    jal_a = 0x0C049DD7
    jal_b = (jal_a & ~0x03FFFFFF) | 0x00001234  # different 26-bit target
    words_a = PROLOGUE_WORDS + [jal_a]
    words_b = PROLOGUE_WORDS + [jal_b]
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert raw > 0
    assert guide == 0, (
        f"jal target is a jump displacement and must be masked; got {guide}"
    )


# ---------------------------------------------------------------------------
# Load/store offset and ALU immediate are SIGNAL, not masked
# ---------------------------------------------------------------------------

def test_load_store_offset_difference_is_structural_signal():
    """Contract: KEEP load/store offsets. lw a0,8(s2) vs lw a0,16(s2)."""
    lw_a = 0x8E440008  # lw a0,8(s2)
    lw_b = (lw_a & ~0xFFFF) | 16  # lw a0,16(s2)
    words_a = list(PROLOGUE_WORDS)
    words_b = list(PROLOGUE_WORDS)
    words_a[4] = lw_a
    words_b[4] = lw_b
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert raw > 0
    assert guide >= 1000, (
        "load/store offset is real signal and must NOT be masked; expected "
        f"a structural-weight cost (>= W_struct=1000), got guide={guide}"
    )


def test_alu_immediate_difference_is_structural_signal():
    """Contract: KEEP ALU immediates. li a1,-4 vs li a1,-8."""
    li_a = 0x2405FFFC  # addiu a1,zero,-4
    li_b = (li_a & ~0xFFFF) | (0xFFF8 & 0xFFFF)  # addiu a1,zero,-8
    words_a = list(PROLOGUE_WORDS)
    words_b = list(PROLOGUE_WORDS)
    words_a[6] = li_a
    words_b[6] = li_b
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert raw > 0
    assert guide >= 1000, (
        "ALU immediate is real signal and must NOT be masked; expected a "
        f"structural-weight cost (>= W_struct=1000), got guide={guide}"
    )


# ---------------------------------------------------------------------------
# (e) structural difference (different opcode) -> W_struct
# ---------------------------------------------------------------------------

def test_opcode_difference_is_structural():
    """lw (opcode 35) vs sw (opcode 43) at identical fields -> structural."""
    lw = 0x8E440008          # lw a0,8(s2)
    sw = (lw & ~(0x3F << 26)) | (43 << 26)  # sw a0,8(s2)
    assert _ref_decode(lw)["opcode"] == 35
    assert _ref_decode(sw)["opcode"] == 43

    words_a = list(PROLOGUE_WORDS)
    words_b = list(PROLOGUE_WORDS)
    words_a[4] = lw
    words_b[4] = sw
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert raw > 0
    assert guide >= 1000, (
        f"opcode mismatch must carry structural weight (>= W_struct=1000), "
        f"got guide={guide}"
    )


# ---------------------------------------------------------------------------
# Length-changing alignment: unequal instruction counts get a finite score
# ---------------------------------------------------------------------------

def test_unequal_length_is_finite_and_dominated_by_w_len():
    """A length difference must yield a finite, W_len-dominated score (LCS)."""
    words_a = list(PROLOGUE_WORDS)
    words_b = list(PROLOGUE_WORDS) + [0x00852824]  # one extra real instruction
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=[]
    )
    assert guide != float("inf"), "length mismatch must be a finite score (LCS)"
    assert guide >= 10000, (
        "a 1-instruction length difference must include W_len (10000) so the "
        f"search drives to correct size first; got guide={guide}"
    )


# ---------------------------------------------------------------------------
# Relocation masking interplay (R_MIPS_26 jal target == reloc type 4)
# ---------------------------------------------------------------------------

def test_relocation_masked_jal_target_scores_zero():
    """A jal whose 26-bit target differs but is reloc-masked -> guide & raw 0.

    Relocation type 4 (R_MIPS_26) zeroes the low 26 bits. Two jals differing
    only inside that masked field must register as identical in BOTH scores,
    matching the existing score_bytes semantics for raw_score.
    """
    jal_a = 0x0C049DD7
    jal_b = (jal_a & ~0x03FFFFFF) | 0x00000777
    words_a = PROLOGUE_WORDS + [jal_a]
    words_b = PROLOGUE_WORDS + [jal_b]
    jal_offset = len(PROLOGUE_WORDS) * 4
    relocs = [(jal_offset, 4)]  # R_MIPS_26 at the jal word
    guide, raw = mips_score.score_instructions(
        _words_to_bytes(words_a), _words_to_bytes(words_b), relocs=relocs
    )
    assert raw == 0, (
        f"R_MIPS_26-masked jal target must be 0 in raw_score, got raw={raw}"
    )
    assert guide == 0, f"masked jal must also be 0 in guide_score, got {guide}"


if __name__ == "__main__":
    sys.exit(pytest.main([__file__, "-v"]))
