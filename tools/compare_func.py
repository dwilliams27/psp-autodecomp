#!/usr/bin/env python3
"""Compare compiled .o against expected function bytes.

Compiles a source file with SNC, then compares each function's .text bytes
against the expected .o extracted from the original binary. Reports match
or mismatch with instruction-level diff.

Usage:
    python3 tools/compare_func.py src/eWorld.cpp           # compare all symbols
    python3 tools/compare_func.py src/eWorld.cpp --symbol __0fGeWorldJLockWorldbK
    python3 tools/compare_func.py --batch src/              # all files in src/
"""

import argparse
import json
import os
import subprocess
import sys
from collections import defaultdict

from common import OBJDUMP, OBJCOPY, NM, load_db, save_db


def compile_source(src_path):
    """Compile a source file using the Makefile. Returns the .o path or None."""
    ext = os.path.splitext(src_path)[1]
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


def get_symbols(o_path):
    """Get all text symbols from an .o file using nm."""
    result = subprocess.run(
        [NM, o_path], capture_output=True, text=True
    )
    if result.returncode != 0:
        return []
    symbols = []
    for line in result.stdout.strip().split("\n"):
        parts = line.split()
        if len(parts) == 3 and parts[1] == "T":
            symbols.append(parts[2])
    return symbols


def extract_raw_text(o_path, max_bytes=None):
    """Extract raw .text section bytes from a .o file using objcopy."""
    import tempfile
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
            data = f.read()
        if max_bytes and len(data) > max_bytes:
            data = data[:max_bytes]
        return data
    finally:
        if os.path.exists(tmp_path):
            os.unlink(tmp_path)


def get_disasm(o_path):
    """Get disassembly text from a .o file."""
    result = subprocess.run(
        [OBJDUMP, "-d", "-j", ".text", o_path],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        return ""
    return result.stdout


def find_func_by_address_range(functions, addr, size):
    """Find a function in the database that covers the given address range."""
    for f in functions:
        f_addr = int(f["address"], 16)
        if f_addr == addr and f["size"] == size:
            return f
    return None


def build_size_index(functions):
    """Build a size → [func_records] index for fast lookup."""
    index = defaultdict(list)
    for f in functions:
        if f["size"] > 0:
            index[f["size"]].append(f)
    return index


def compare_symbol(compiled_o, symbol, functions, size_index=None):
    """Compare one symbol from the compiled .o against its expected bytes.

    Extracts raw .text bytes from both .o files and compares directly.
    Uses size_index for O(1) candidate lookup instead of scanning all functions.

    Returns (matched: bool, func_record or None, diff_text: str)
    """
    our_bytes = extract_raw_text(compiled_o)
    if our_bytes is None or len(our_bytes) == 0:
        return False, None, f"Could not extract .text bytes from {compiled_o}"

    our_size = len(our_bytes)

    if size_index is None:
        size_index = build_size_index(functions)

    candidates = size_index.get(our_size, [])

    best_match = None
    best_score = float("inf")

    for f in candidates:
        expected_path = f"expected/build/func/{int(f['address'], 16):08x}.o"
        if not os.path.exists(expected_path):
            continue

        exp_bytes = extract_raw_text(expected_path, max_bytes=f["size"])
        if exp_bytes is None:
            continue

        cmp_size = min(len(our_bytes), len(exp_bytes), f["size"])
        diff_count = sum(1 for a, b in zip(our_bytes[:cmp_size], exp_bytes[:cmp_size]) if a != b)
        if diff_count < best_score:
            best_score = diff_count
            best_match = (f, expected_path)

    if best_match is None:
        return False, None, f"No expected function with size {our_size} bytes"

    f, exp_path = best_match
    if best_score == 0:
        return True, f, "MATCH"

    diff = f"MISMATCH: {best_score}/{f['size']} bytes differ\n"
    diff += f"Expected: {f['name']} at {f['address']}\n\n"
    diff += "--- Expected ---\n"
    diff += get_disasm(exp_path)
    diff += "\n--- Compiled ---\n"
    diff += get_disasm(compiled_o)

    return False, f, diff


def compare_file(src_path, symbol_filter=None, functions=None):
    """Compile and compare all symbols in a source file."""
    if functions is None:
        functions = load_db()

    o_path = compile_source(src_path)
    if not o_path:
        return []

    symbols = get_symbols(o_path)
    if symbol_filter:
        symbols = [s for s in symbols if s == symbol_filter]

    if not symbols:
        print(f"No text symbols found in {o_path}")
        return []

    size_index = build_size_index(functions)
    results = []
    for sym in symbols:
        matched, func, diff = compare_symbol(o_path, sym, functions, size_index)
        status = "MATCH" if matched else "MISMATCH"
        results.append((sym, matched, func, diff))

        if matched:
            print(f"  ✓ {sym} — MATCH", end="")
            if func:
                print(f" ({func['name']}, {func['size']}B)")
            else:
                print()
        else:
            print(f"  ✗ {sym} — {diff}")

    return results


def main():
    parser = argparse.ArgumentParser(description="Compare compiled .o against expected function bytes")
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
            results = compare_file(src, functions=functions)
            for sym, matched, func, diff in results:
                total_compared += 1
                if matched:
                    total_matched += 1
                    update_matched(func)
        print(f"\nTotal: {total_matched}/{total_compared} matched")
    else:
        results = compare_file(args.source, args.symbol, functions)
        for sym, matched, func, diff in results:
            if matched:
                update_matched(func)

    if args.update_db and functions:
        save_db(functions)


if __name__ == "__main__":
    main()
