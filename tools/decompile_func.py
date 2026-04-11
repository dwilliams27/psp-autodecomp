#!/usr/bin/env python3
"""Decompile a function using m2c (MIPS-to-C).

Extracts disassembly from the original binary, converts to m2c's expected
format, runs m2c, and outputs initial C.

Usage:
    python3 tools/decompile_func.py "eWorld::LockWorld"
    python3 tools/decompile_func.py 0x0002066c
    python3 tools/decompile_func.py 0x0002066c --context include/eWorld.h
    python3 tools/decompile_func.py 0x0002066c -o src/eWorld.cpp
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile

from common import EBOOT_PATH, OBJDUMP, load_db, find_function, fix_vfpu_disassembly

M2C_PATH = "extern/m2c/m2c.py"

# MIPS register name mapping (objdump names → m2c $ names)
REG_MAP = {
    "zero": "$zero", "at": "$at",
    "v0": "$v0", "v1": "$v1",
    "a0": "$a0", "a1": "$a1", "a2": "$a2", "a3": "$a3",
    "t0": "$t0", "t1": "$t1", "t2": "$t2", "t3": "$t3",
    "t4": "$t4", "t5": "$t5", "t6": "$t6", "t7": "$t7",
    "s0": "$s0", "s1": "$s1", "s2": "$s2", "s3": "$s3",
    "s4": "$s4", "s5": "$s5", "s6": "$s6", "s7": "$s7",
    "t8": "$t8", "t9": "$t9",
    "k0": "$k0", "k1": "$k1",
    "gp": "$gp", "sp": "$sp", "fp": "$fp", "ra": "$ra",
}


def build_addr_to_name(functions):
    """Build address → function name lookup for JAL resolution."""
    lookup = {}
    for f in functions:
        addr = int(f["address"], 16)
        # Create a safe label for m2c
        name = f["safe_name"]
        # Shorten to just the readable part (drop address suffix)
        parts = name.rsplit("__", 1)
        if len(parts) == 2 and len(parts[1]) == 8:
            name = parts[0]
        lookup[addr] = name
    return lookup


def disassemble_function(func):
    """Disassemble a function from the original binary using objdump."""
    addr = int(func["address"], 16)
    end = addr + func["size"]

    result = subprocess.run(
        [OBJDUMP, "-d", "-j", ".text",
         f"--start-address=0x{addr:x}",
         f"--stop-address=0x{end:x}",
         EBOOT_PATH],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print(f"Error disassembling: {result.stderr}", file=sys.stderr)
        return None
    return fix_vfpu_disassembly(result.stdout)


def convert_to_m2c_asm(objdump_output, func, addr_lookup):
    """Convert objdump disassembly to m2c-compatible .s format."""
    func_addr = int(func["address"], 16)
    func_name = func["safe_name"]
    # Shorten name
    parts = func_name.rsplit("__", 1)
    if len(parts) == 2 and len(parts[1]) == 8:
        func_name = parts[0]

    lines = objdump_output.strip().split("\n")
    asm_lines = []
    branch_targets = set()

    # First pass: find all branch targets
    for line in lines:
        line = line.strip()
        if not line or ":" not in line:
            continue
        # Match instruction lines: "addr: hex  mnemonic operands"
        m = re.match(r"\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)", line)
        if not m:
            continue
        insn = m.group(3).strip()
        # Find branch targets (beqz, bnez, b, beq, bne, etc.)
        branch_m = re.search(r"\b(?:b|beq|bne|beqz|bnez|bgez|bgtz|blez|bltz|bgezal|bltzal|beql|bnel|beqzl|bnezl|bc1f|bc1t|bc1fl|bc1tl)\s+.*?0x([0-9a-f]+)", insn)
        if branch_m:
            target = int(branch_m.group(1), 16)
            if func_addr <= target < func_addr + func["size"]:
                branch_targets.add(target)

    # Second pass: build m2c assembly
    asm_lines.append(".set noat")
    asm_lines.append(".set noreorder")
    asm_lines.append("")
    asm_lines.append(f"glabel {func_name}")

    for line in lines:
        line = line.strip()
        if not line or ":" not in line:
            continue

        m = re.match(r"\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(.+)", line)
        if not m:
            continue

        addr_val = int(m.group(1), 16)
        hex_bytes = m.group(2).upper()
        insn = m.group(3).strip()

        # Insert label if this is a branch target
        if addr_val in branch_targets:
            asm_lines.append(f".L{addr_val:08X}:")

        # Convert register names
        converted = convert_registers(insn)

        # Resolve JAL targets to names
        converted = resolve_jal(converted, addr_lookup)

        # Convert branch targets to labels
        converted = convert_branch_targets(converted, func_addr, func["size"])

        # Format as m2c expects
        asm_lines.append(f"/* {addr_val:05X} {addr_val:08X} {hex_bytes} */  {converted}")

    return "\n".join(asm_lines) + "\n"


def convert_registers(insn):
    """Convert objdump register names to $ notation."""
    # Split into mnemonic and operands
    parts = insn.split(None, 1)
    if len(parts) < 2:
        return insn
    mnemonic = parts[0]
    operands = parts[1]

    # Replace register names with $ prefix
    # Be careful not to replace partial matches (e.g., "at" in "data")
    # Process register tokens in operands
    result = []
    i = 0
    while i < len(operands):
        matched = False
        for reg_name, reg_replacement in sorted(REG_MAP.items(), key=lambda x: -len(x[0])):
            if operands[i:].startswith(reg_name):
                before_ok = (i == 0 or (not operands[i-1].isalnum() and operands[i-1] != "$"))
                after_pos = i + len(reg_name)
                after_ok = (after_pos >= len(operands) or
                           operands[after_pos] in ",()+-: \t" or
                           not operands[after_pos].isalnum())
                if before_ok and after_ok:
                    result.append(reg_replacement)
                    i += len(reg_name)
                    matched = True
                    break
        if not matched:
            result.append(operands[i])
            i += 1

    return f"{mnemonic} {''.join(result)}"


def resolve_jal(insn, addr_lookup):
    """Replace jal 0xADDR with jal func_name."""
    m = re.match(r"(jal\s+)0x([0-9a-fA-F]+)", insn)
    if m:
        target = int(m.group(2), 16)
        if target in addr_lookup:
            return f"{m.group(1)}{addr_lookup[target]}"
        return f"{m.group(1)}func_{target:08X}"
    return insn


def convert_branch_targets(insn, func_addr, func_size):
    """Convert branch target addresses to .L labels."""
    # Match branch instructions with hex targets
    def replace_target(match):
        target = int(match.group(1), 16)
        if func_addr <= target < func_addr + func_size:
            return f".L{target:08X}"
        return match.group(0)

    return re.sub(r"0x([0-9a-fA-F]+)$", replace_target, insn)


def run_m2c(asm_text, target="mipsel-gcc-c", context=None):
    """Run m2c on the assembly text and return C output."""
    with tempfile.NamedTemporaryFile(mode="w", suffix=".s", delete=False) as tmp:
        tmp.write(asm_text)
        tmp_path = tmp.name

    try:
        cmd = ["python3", M2C_PATH, "-t", target, tmp_path]
        if context:
            for ctx in context:
                cmd.extend(["--context", ctx])

        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"m2c error: {result.stderr}", file=sys.stderr)
            return None
        return result.stdout
    finally:
        os.unlink(tmp_path)


def main():
    parser = argparse.ArgumentParser(description="Decompile a function using m2c")
    parser.add_argument("query", help="Function name (substring) or address (0x...)")
    parser.add_argument("--context", action="append", help="C context file for struct defs")
    parser.add_argument("-o", "--output", help="Write output to file")
    parser.add_argument("--asm-only", action="store_true", help="Output converted asm only (no m2c)")
    parser.add_argument("--target", default="mipsel-gcc-c",
                        help="m2c target triple (default: mipsel-gcc-c)")

    args = parser.parse_args()
    functions = load_db()
    func = find_function(functions, args.query)

    if func["size"] == 0:
        print(f"Error: function has zero size", file=sys.stderr)
        sys.exit(1)

    # Disassemble
    objdump_out = disassemble_function(func)
    if not objdump_out:
        sys.exit(1)

    # Convert to m2c format
    addr_lookup = build_addr_to_name(functions)
    asm_text = convert_to_m2c_asm(objdump_out, func, addr_lookup)

    if args.asm_only:
        output = asm_text
    else:
        output = run_m2c(asm_text, args.target, args.context)
        if output is None:
            sys.exit(1)

    # Add header comment
    header = f"// Decompiled: {func['name']}\n"
    header += f"// Address: {func['address']}, Size: {func['size']}B\n"
    header += f"// Obj: {func['obj_file']}\n\n"
    output = header + output

    if args.output:
        os.makedirs(os.path.dirname(args.output) or ".", exist_ok=True)
        with open(args.output, "w") as f:
            f.write(output)
        print(f"Wrote {args.output}", file=sys.stderr)
    else:
        print(output)


if __name__ == "__main__":
    main()
