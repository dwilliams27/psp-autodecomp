#!/usr/bin/env python3
"""Compare compiled .o against expected function bytes from the original EBOOT.

Compiles a source file with SNC, then for each .text symbol, reads the
original bytes from EBOOT.BIN.dec for all same-size function candidates,
applies relocation masking, and reports byte-exact match or closest mismatch.

Usage:
    python3 tools/compare_func.py src/eWorld.cpp           # compare all symbols
    python3 tools/compare_func.py src/eWorld.cpp --symbol __0fGeWorldJLockWorldbK
    python3 tools/compare_func.py --batch src/              # all files in src/
"""

import argparse
from collections import defaultdict
import json
import os
import subprocess
import sys
import tempfile

from common import (EBOOT_PATH, TEXT_FILE_OFFSET,
                    OBJCOPY, NM,
                    load_db, save_db,
                    get_text_relocations, mask_relocation_bytes)


def load_eboot():
    """Read the entire EBOOT .text section into memory.

    Returns bytes. Raises on missing or unreadable EBOOT.
    """
    with open(EBOOT_PATH, "rb") as f:
        return f.read()


def compile_source(src_path):
    """Compile a source file using the Makefile. Returns the .o path or None."""
    o_path = src_path.replace("src/", "build/src/") + ".o"

    result = subprocess.run(
        ["make", o_path],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print(f"Compilation failed for {src_path}:", file=sys.stderr)
        print(result.stderr, file=sys.stderr)
        if result.stdout:
            print(result.stdout, file=sys.stderr)
        return None
    return o_path


def get_symbol_layout(o_path, text_size):
    """Get text symbol offsets and sizes from a .o file via nm.

    text_size is the total .text section size (from extract_text_bytes).
    Returns list of (offset, size, name) sorted by offset.
    Raises RuntimeError if nm fails.
    """
    result = subprocess.run([NM, o_path], capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(
            f"nm failed on {o_path} (rc={result.returncode}): {result.stderr.strip()}"
        )
    symbols = []
    for line in result.stdout.strip().split("\n"):
        parts = line.split()
        if len(parts) == 3 and parts[1] == "T":
            symbols.append((int(parts[0], 16), parts[2]))
    symbols.sort()

    layout = []
    for i, (off, name) in enumerate(symbols):
        next_off = symbols[i + 1][0] if i + 1 < len(symbols) else text_size
        size = next_off - off
        layout.append((off, size, name))
    return layout


def extract_text_bytes(o_path):
    """Extract raw .text section bytes from a .o file."""
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tmp:
        tmp_path = tmp.name
    try:
        result = subprocess.run(
            [OBJCOPY, "-O", "binary", "-j", ".text", o_path, tmp_path],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            return None
        with open(tmp_path, "rb") as f:
            return f.read()
    finally:
        if os.path.exists(tmp_path):
            os.unlink(tmp_path)


def build_size_index(functions):
    """Build size -> [func_records] index for candidate lookup."""
    index = defaultdict(list)
    for f in functions:
        if f["size"] > 0:
            index[f["size"]].append(f)
    return index


def find_func_and_diff(compiled_bytes, func_relocs, sym_size, size_index, eboot_data):
    """Find the database function whose EBOOT bytes match the compiled bytes.

    Single pass over same-size candidates: returns the byte-exact match if found,
    otherwise returns the closest mismatch with diff count.

    Returns (func_or_None, diff_count). diff_count is 0 for exact match.
    """
    candidates = size_index.get(sym_size, [])
    compiled_masked = mask_relocation_bytes(compiled_bytes, func_relocs) if func_relocs else compiled_bytes

    best_func = None
    best_diff = float("inf")

    for f in candidates:
        func_addr = int(f["address"], 16)
        start = func_addr + TEXT_FILE_OFFSET
        expected = eboot_data[start:start + f["size"]]

        if len(expected) != f["size"]:
            raise RuntimeError(
                f"Short EBOOT read for {f['name']} at {f['address']}: "
                f"got {len(expected)}/{f['size']}B. EBOOT may be truncated."
            )

        expected_masked = mask_relocation_bytes(expected, func_relocs) if func_relocs else expected

        if compiled_masked == expected_masked:
            return f, 0

        dc = sum(1 for a, b in zip(compiled_masked, expected_masked) if a != b)
        if dc < best_diff:
            best_diff = dc
            best_func = f

    return best_func, best_diff


def compare_file(src_path, symbol_filter=None, functions=None, eboot_data=None):
    """Compile and compare all symbols in a source file against EBOOT bytes.

    Returns list of (sym_name, matched, func_record, message).
    """
    if functions is None:
        functions = load_db()
    if eboot_data is None:
        eboot_data = load_eboot()

    o_path = compile_source(src_path)
    if not o_path:
        return []

    text_bytes = extract_text_bytes(o_path)
    if text_bytes is None:
        print(f"Could not extract .text from {o_path}", file=sys.stderr)
        return []

    layout = get_symbol_layout(o_path, len(text_bytes))
    if symbol_filter:
        layout = [(off, sz, name) for off, sz, name in layout if name == symbol_filter]

    if not layout:
        print(f"No text symbols found in {o_path}")
        return []

    relocations = get_text_relocations(o_path)
    size_index = build_size_index(functions)

    results = []
    for sym_off, sym_size, sym_name in layout:
        compiled = text_bytes[sym_off:sym_off + sym_size]
        func_relocs = [(off - sym_off, rtype) for off, rtype in relocations
                       if sym_off <= off < sym_off + sym_size]

        func, diff_count = find_func_and_diff(
            compiled, func_relocs, sym_size, size_index, eboot_data
        )

        if func is not None and diff_count == 0:
            reloc_note = f" (with {len(func_relocs)} reloc(s) masked)" if func_relocs else ""
            msg = f"MATCH{reloc_note}"
            print(f"  \u2713 {sym_name} \u2014 {msg} ({func['name']}, {func['size']}B)")
            results.append((sym_name, True, func, msg))
        elif func is not None:
            msg = f"MISMATCH: {diff_count}/{func['size']} bytes differ ({func['name']})"
            print(f"  \u2717 {sym_name} \u2014 {msg}")
            results.append((sym_name, False, func, msg))
        else:
            msg = f"NO MATCH: no function with size {sym_size}B in database"
            print(f"  \u2717 {sym_name} \u2014 {msg}")
            results.append((sym_name, False, None, msg))

    return results


def main():
    parser = argparse.ArgumentParser(description="Compare compiled .o against EBOOT function bytes")
    parser.add_argument("source", nargs="?", help="Source file to compile and compare")
    parser.add_argument("--symbol", help="Compare only this symbol")
    parser.add_argument("--batch", action="store_true", help="Compare all source files in directory")
    parser.add_argument("--update-db", action="store_true", default=True,
                        help="Update match status in functions.json (default: true)")

    args = parser.parse_args()

    if not args.source:
        parser.print_help()
        sys.exit(1)

    functions = load_db()
    eboot_data = load_eboot()
    addr_index = {f["address"]: f for f in functions}

    def update_matched(func):
        if func and args.update_db and func["address"] in addr_index:
            addr_index[func["address"]]["match_status"] = "matched"

    if args.batch:
        src_dir = args.source.rstrip("/")
        sources = []
        for root, _, files in os.walk(src_dir):
            for f in files:
                if f.endswith((".c", ".cpp")):
                    sources.append(os.path.join(root, f))
        total_matched = 0
        total_compared = 0
        for src in sorted(sources):
            print(f"\n{src}:")
            results = compare_file(src, functions=functions, eboot_data=eboot_data)
            for sym, matched, func, diff in results:
                total_compared += 1
                if matched:
                    total_matched += 1
                    update_matched(func)
        print(f"\nTotal: {total_matched}/{total_compared} matched")
    else:
        results = compare_file(args.source, args.symbol, functions, eboot_data)
        for sym, matched, func, diff in results:
            if matched:
                update_matched(func)

    if args.update_db and functions:
        save_db(functions)


if __name__ == "__main__":
    main()
