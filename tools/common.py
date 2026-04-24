#!/usr/bin/env python3
"""Shared utilities for decompilation tools."""

import json
import os
import re
import sys

DB_PATH = "config/functions.json"
EBOOT_PATH = "extern/iso_extract/PSP_GAME/SYSDIR/EBOOT.BIN.dec"
MAP_PATH = "extern/extracted_symbols/Game-dvd.map"
TEXT_FILE_OFFSET = 0xA0  # .text section file offset in the PRX

OBJDUMP = "mipsel-linux-gnu-objdump"
OBJCOPY = "mipsel-linux-gnu-objcopy"
NM = "mipsel-linux-gnu-nm"
READELF = "mipsel-linux-gnu-readelf"
AS = "mipsel-linux-gnu-as"
AS_FLAGS = ["-march=allegrex", "-mabi=eabi", "-EL"]
CLAUDE = "/usr/local/bin/claude"
CLAUDE_MODEL = "claude-opus-4-7"
# Resolved via PATH — codex ships through nvm, not /usr/local/bin.
CODEX = "codex"
CODEX_MODEL = "gpt-5.4"


def load_db():
    """Load functions.json. Exits with error if missing."""
    if not os.path.exists(DB_PATH):
        print(f"Error: {DB_PATH} not found. Run 'python3 tools/func_db.py build' first.", file=sys.stderr)
        sys.exit(1)
    with open(DB_PATH, "r") as f:
        return json.load(f)


def save_db(functions):
    """Save functions.json."""
    os.makedirs(os.path.dirname(DB_PATH), exist_ok=True)
    with open(DB_PATH, "w") as f:
        json.dump(functions, f, indent=2)


def find_function(functions, query, strict=True):
    """Find a function by address or name substring.

    If strict=True, exits on not-found or ambiguous. If False, returns None.
    """
    if query.startswith("0x") or query.startswith("0X"):
        addr = query.lower()
        for f in functions:
            if f["address"] == addr:
                return f
        if strict:
            print(f"Error: no function at address {query}", file=sys.stderr)
            sys.exit(1)
        return None

    matches = [f for f in functions if query.lower() in f["name"].lower()]
    if len(matches) == 1:
        return matches[0]
    if len(matches) == 0:
        if strict:
            print(f"Error: no function matching '{query}'", file=sys.stderr)
            sys.exit(1)
        return None

    # Ambiguous
    print(f"Multiple matches for '{query}':", file=sys.stderr)
    for m in matches[:10]:
        print(f"  {m['address']}  {m['name']}", file=sys.stderr)
    if len(matches) > 10:
        print(f"  ... and {len(matches) - 10} more", file=sys.stderr)
    print(f"\nBe more specific, or use an address.", file=sys.stderr)
    sys.exit(1)


def filter_functions(functions, class_name=None, name=None, obj=None,
                     size_min=None, size_max=None, status=None, leaf=False, limit=None):
    """Filter functions by common criteria."""
    results = functions
    if class_name:
        results = [f for f in results if f["class_name"] and class_name.lower() in f["class_name"].lower()]
    if name:
        results = [f for f in results if name.lower() in f["name"].lower()]
    if obj:
        results = [f for f in results if f["obj_file"] and obj.lower() in f["obj_file"].lower()]
    if size_min is not None:
        results = [f for f in results if f["size"] >= size_min]
    if size_max is not None:
        results = [f for f in results if f["size"] <= size_max]
    if status:
        results = [f for f in results if f["match_status"] == status]
    if leaf:
        results = [f for f in results if f.get("is_leaf", False)]
    if limit:
        results = results[:limit]
    return results


def build_addr_map(functions):
    """Build address string → function record lookup."""
    return {f["address"]: f for f in functions}


# Opcode ranges (bits 31:26) that are VFPU instructions on Allegrex.
# Standard mipsel objdump mis-decodes these as MIPS64/Loongson opcodes.
_VFPU_OPCODES = frozenset({
    0x12,  # COP2: mtv, mfv, mfvc
    0x18,  # VFPU0: vadd, vsub, vdiv, vmul
    0x19,  # VFPU1: vdot, vscl, vcrs
    0x1A,  # VFPU2: vlgb, vuc2i, vs2i, vi2f, etc.
    0x1B,  # VFPU3: vcmp, vmin, vmax, etc.
    0x1C,  # VFPU4: vsin, vcos, vexp2, etc.
    0x1D,  # VFPU5: vpfx*, vcst, vnop, vsync
    0x34,  # VFPU6: vsqrt, vmov, vneg, vzero, vone, etc.
    0x35,  # VFPU7: vpfxs, vpfxt, vpfxd, viim, vfim
    0x36,  # lv.q
    0x37,  # lv.q (alternate)
    0x3C,  # vtfm, vmmul
    0x3E,  # sv.q
    0x3F,  # sv.q (alternate)
})

_VFPU_INSN_RE = re.compile(r"^(\s*[0-9a-f]+:\s+)([0-9a-f]{8})\s+(.*)$")

try:
    import rabbitizer as _rabbitizer
    _VFPU_CATEGORY = _rabbitizer.InstrCategory.R4000ALLEGREX
    _HAS_RABBITIZER = True
except (ImportError, AttributeError) as _rabbitizer_err:
    _HAS_RABBITIZER = False
    _RABBITIZER_IMPORT_ERROR = _rabbitizer_err


def fix_vfpu_disassembly(objdump_output):
    """Post-process objdump output to replace mis-decoded VFPU instructions.

    mipsel-linux-gnu-objdump doesn't know VFPU and shows garbage MIPS64 mnemonics.
    This uses rabbitizer (with R4000ALLEGREX mode) to decode them correctly.

    Raises RuntimeError if rabbitizer is not available — VFPU disassembly is
    required for correct output, not optional.
    """
    if not _HAS_RABBITIZER:
        raise RuntimeError(
            f"rabbitizer is required for correct VFPU disassembly but failed to import: "
            f"{_RABBITIZER_IMPORT_ERROR}. Install it: pip install rabbitizer"
        )

    lines = objdump_output.split("\n")
    result = []

    for line in lines:
        m = _VFPU_INSN_RE.match(line)
        if m:
            prefix = m.group(1)  # "   addr:\t"
            hex_word = m.group(2)
            word = int(hex_word, 16)
            opcode = (word >> 26) & 0x3F

            if opcode in _VFPU_OPCODES:
                instr = _rabbitizer.Instruction(word, category=_VFPU_CATEGORY)
                decoded = instr.disassemble()
                result.append(f"{prefix}{hex_word} \t{decoded}")
                continue

        result.append(line)

    return "\n".join(result)


# MIPS relocation types and their byte masks.
# For each type, we specify which bytes of the 4-byte instruction word
# are affected by the relocation (and should be masked during comparison).
RELOC_MASKS = {
    4: bytes([0x00, 0x00, 0x00, 0xfc]),  # R_MIPS_26: mask bits [25:0] (jal target)
    5: bytes([0x00, 0x00, 0xff, 0xff]),  # R_MIPS_HI16: mask bits [15:0]
    6: bytes([0x00, 0x00, 0xff, 0xff]),  # R_MIPS_LO16: mask bits [15:0]
}


def get_text_relocations(o_path):
    """Get .text section relocations from a compiled .o file.

    Returns a list of (offset, reloc_type) tuples.
    """
    import subprocess
    result = subprocess.run(
        [READELF, "-r", o_path], capture_output=True, text=True
    )
    if result.returncode != 0:
        raise RuntimeError(
            f"readelf failed on {o_path} (exit {result.returncode}):\n{result.stderr}"
        )

    if "no relocations" in result.stdout.lower():
        return []

    relocs = []
    in_text = False
    for line in result.stdout.split("\n"):
        if ".rel.text" in line:
            in_text = True
            continue
        if in_text and line.strip() == "":
            break
        if in_text and line.strip() and not line.startswith("Relocation") and not line.startswith(" Offset"):
            parts = line.split()
            if len(parts) >= 3:
                offset = int(parts[0], 16)
                info = int(parts[1], 16)
                reloc_type = info & 0xFF
                relocs.append((offset, reloc_type))
    return relocs


def mask_relocation_bytes(data, relocations):
    """Apply relocation masks to a byte array, zeroing relocated fields."""
    masked = bytearray(data)
    for offset, reloc_type in relocations:
        if reloc_type not in RELOC_MASKS:
            raise ValueError(
                f"Unhandled MIPS reloc type {reloc_type} at offset {offset:#x} — "
                f"add it to RELOC_MASKS"
            )
        if offset + 4 > len(masked):
            raise IndexError(
                f"Reloc offset {offset:#x} out of bounds for "
                f"{len(masked)}-byte .text section"
            )
        mask = RELOC_MASKS[reloc_type]
        for i in range(4):
            masked[offset + i] &= mask[i]
    return bytes(masked)
