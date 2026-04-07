#!/usr/bin/env python3
"""Extract function bytes from original binary and create expected .o files.

Uses .incbin to wrap raw bytes, avoiding VFPU assembly issues.

Usage:
    python3 tools/extract_func.py "eWorld::LockWorld"     # by name (substring)
    python3 tools/extract_func.py 0x2066c                  # by address
    python3 tools/extract_func.py --batch --size-max 8     # batch: all trivial
    python3 tools/extract_func.py --batch --class mSphere  # batch: all mSphere
"""

import argparse
import os
import subprocess
import sys
import tempfile

from common import (DB_PATH, EBOOT_PATH, TEXT_FILE_OFFSET, AS, AS_FLAGS,
                    load_db, find_function, filter_functions)

BIN_DIR = "expected/bin"


def extract_bytes(func):
    """Extract function's raw bytes from the EBOOT binary."""
    addr = int(func["address"], 16)
    size = func["size"]
    if size == 0:
        return None

    file_offset = addr + TEXT_FILE_OFFSET
    os.makedirs(BIN_DIR, exist_ok=True)
    bin_path = os.path.join(BIN_DIR, f"{addr:08x}.bin")

    with open(EBOOT_PATH, "rb") as f:
        f.seek(file_offset)
        data = f.read(size)

    if len(data) != size:
        print(f"Error: could only read {len(data)}/{size} bytes for {func['name']}", file=sys.stderr)
        return None

    with open(bin_path, "wb") as f:
        f.write(data)

    return bin_path


def create_expected_o(func, bin_path):
    """Create an expected .o file from extracted bytes using .incbin."""
    addr = int(func["address"], 16)
    symbol = func["safe_name"]
    o_path = f"expected/build/func/{addr:08x}.o"
    os.makedirs(os.path.dirname(o_path), exist_ok=True)

    # Write temporary .s file
    abs_bin = os.path.abspath(bin_path)
    asm_content = f""".set noat
.set noreorder
.text
.globl {symbol}
{symbol}:
.incbin "{abs_bin}"
"""

    with tempfile.NamedTemporaryFile(mode="w", suffix=".s", delete=False) as tmp:
        tmp.write(asm_content)
        tmp_path = tmp.name

    try:
        result = subprocess.run(
            [AS] + AS_FLAGS + ["-o", o_path, tmp_path],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            print(f"Error assembling expected .o for {func['name']}:", file=sys.stderr)
            print(result.stderr, file=sys.stderr)
            return None
    finally:
        os.unlink(tmp_path)

    return o_path


def extract_one(functions, query, verbose=True):
    """Extract a single function."""
    func = find_function(functions, query)
    if func["size"] == 0:
        if verbose:
            print(f"Skipping zero-size function: {func['name']}")
        return None

    bin_path = extract_bytes(func)
    if not bin_path:
        return None

    o_path = create_expected_o(func, bin_path)
    if not o_path:
        return None

    if verbose:
        print(f"Extracted: {func['name']}")
        print(f"  Address: {func['address']}, Size: {func['size']}B")
        print(f"  Binary:  {bin_path}")
        print(f"  Object:  {o_path}")
        print(f"  Symbol:  {func['safe_name']}")

    return o_path


def extract_batch(functions, args):
    """Extract multiple functions matching filters."""
    results = filter_functions(functions, class_name=args.class_name, name=args.name,
                               obj=args.obj, size_min=args.size_min, size_max=args.size_max,
                               limit=args.limit)

    extracted = 0
    skipped = 0
    for func in results:
        if func["size"] == 0:
            skipped += 1
            continue
        bin_path = extract_bytes(func)
        if bin_path:
            o_path = create_expected_o(func, bin_path)
            if o_path:
                extracted += 1
            else:
                skipped += 1
        else:
            skipped += 1

    print(f"Extracted {extracted} functions, skipped {skipped}")


def main():
    parser = argparse.ArgumentParser(description="Extract function bytes and create expected .o files")
    parser.add_argument("query", nargs="?", help="Function name (substring) or address (0x...)")
    parser.add_argument("--batch", action="store_true", help="Batch mode with filters")
    parser.add_argument("--class", dest="class_name", help="Filter by class name")
    parser.add_argument("--name", help="Filter by function name")
    parser.add_argument("--obj", help="Filter by .obj file")
    parser.add_argument("--size-min", type=int, help="Minimum size in bytes")
    parser.add_argument("--size-max", type=int, help="Maximum size in bytes")
    parser.add_argument("--limit", type=int, help="Max functions to extract")

    args = parser.parse_args()

    if not os.path.exists(EBOOT_PATH):
        print(f"Error: {EBOOT_PATH} not found.", file=sys.stderr)
        sys.exit(1)

    functions = load_db()

    if args.batch:
        extract_batch(functions, args)
    elif args.query:
        extract_one(functions, args.query)
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
