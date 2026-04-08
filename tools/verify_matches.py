#!/usr/bin/env python3
"""Verify all matched functions are byte-exact against the original binary.

Recompiles all source files, extracts per-symbol bytes from the compiled .o
files, and compares against the original EBOOT bytes for every function
marked as 'matched' in the database.

Usage:
    python3 tools/verify_matches.py              # verify all matched functions
    python3 tools/verify_matches.py --verbose     # show each function as it's checked
    python3 tools/verify_matches.py --fix         # reset mismatched functions to 'untried'
"""

import argparse
import os
import subprocess
import sys
import tempfile

from common import (EBOOT_PATH, TEXT_FILE_OFFSET, OBJCOPY, NM,
                    load_db, save_db)


def get_original_bytes(eboot_fh, address, size):
    """Read function bytes directly from an open EBOOT file handle."""
    eboot_fh.seek(address + TEXT_FILE_OFFSET)
    return eboot_fh.read(size)


def get_compiled_symbols(o_path):
    """Extract all text symbols from a .o file with their raw bytes.

    Returns dict: {symbol_name: bytes} or raises on failure.
    """
    result = subprocess.run([NM, o_path], capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"nm failed on {o_path}: {result.stderr.strip()}")

    symbols = []
    for line in result.stdout.strip().split("\n"):
        parts = line.split()
        if len(parts) == 3 and parts[1] == "T":
            symbols.append((int(parts[0], 16), parts[2]))
    symbols.sort()

    if not symbols:
        return {}

    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tmp:
        tmp_path = tmp.name
    try:
        r = subprocess.run(
            [OBJCOPY, "-O", "binary", "-j", ".text", o_path, tmp_path],
            capture_output=True, text=True
        )
        if r.returncode != 0:
            raise RuntimeError(f"objcopy failed on {o_path}: {r.stderr.strip()}")
        with open(tmp_path, "rb") as f:
            text_bytes = f.read()
    finally:
        if os.path.exists(tmp_path):
            os.unlink(tmp_path)

    result = {}
    for i, (offset, name) in enumerate(symbols):
        end = symbols[i + 1][0] if i + 1 < len(symbols) else len(text_bytes)
        result[name] = text_bytes[offset:end]

    return result


def find_all_compiled_symbols():
    """Scan all .o files in build/src/ and return {symbol_name: (bytes, o_path)}."""
    build_dir = "build/src"
    if not os.path.isdir(build_dir):
        raise FileNotFoundError(
            f"{build_dir} does not exist — nothing compiled to verify against"
        )

    all_symbols = {}
    errors = 0
    for root, _, files in os.walk(build_dir):
        for fname in files:
            if not fname.endswith(".o"):
                continue
            o_path = os.path.join(root, fname)
            try:
                symbols = get_compiled_symbols(o_path)
                for name, data in symbols.items():
                    all_symbols[name] = (data, o_path)
            except RuntimeError as e:
                print(f"  ERROR reading {o_path}: {e}", file=sys.stderr)
                errors += 1

    if errors > 0:
        print(f"  {errors} .o files had errors", file=sys.stderr)

    return all_symbols


def verify_all(verbose=False, fix=False):
    """Verify every matched function against the original binary."""
    functions = load_db()
    matched = [f for f in functions if f["match_status"] == "matched"]

    if not matched:
        print("No matched functions in the database.")
        return 0, 0, 0, 0

    # Recompile all source files
    src_files = sorted(
        os.path.join("src", f) for f in os.listdir("src")
        if f.endswith((".c", ".cpp"))
    )
    print(f"Recompiling {len(src_files)} source files...")
    compile_errors = 0
    for src in src_files:
        o_path = src.replace("src/", "build/src/") + ".o"
        result = subprocess.run(["make", o_path], capture_output=True, text=True)
        if result.returncode != 0:
            print(f"  compile error: {src}")
            compile_errors += 1
    if compile_errors > 0:
        print(f"  {compile_errors} files failed to compile")

    # Build symbol map from all compiled .o files
    print("Scanning compiled symbols...")
    all_symbols = find_all_compiled_symbols()
    print(f"Found {len(all_symbols)} compiled symbols\n")

    verified = 0
    mismatched = 0
    not_compiled = 0
    problem_addrs = set()

    with open(EBOOT_PATH, "rb") as eboot:
        for func in matched:
            addr = int(func["address"], 16)
            size = func["size"]
            expected = get_original_bytes(eboot, addr, size)

            # Look up by scanning all symbols for matching bytes at this size.
            # We can't use name-based lookup because the DB has safe_names while
            # the .o files have SNC-mangled names — no mapping between them yet.
            # Instead, search for any compiled symbol whose first `size` bytes
            # match the original exactly.
            found = False
            for sym_name, (compiled_bytes, o_file) in all_symbols.items():
                if len(compiled_bytes) >= size and compiled_bytes[:size] == expected:
                    verified += 1
                    found = True
                    if verbose:
                        print(f"  ✓ {func['address']}  {size:>4}B  {func['name']}")
                    break

            if not found:
                problem_addrs.add(func["address"])
                # Distinguish: is there a symbol with this exact size that DOESN'T match?
                size_matches = [n for n, (b, _) in all_symbols.items()
                                if len(b) == size and b != expected]
                if size_matches:
                    mismatched += 1
                    print(f"  ✗ {func['address']}  {size:>4}B  {func['name']} — BYTE MISMATCH")
                else:
                    not_compiled += 1
                    print(f"  ? {func['address']}  {size:>4}B  {func['name']} — not found in compiled .o files")

    total = len(matched)
    print(f"\n{'='*60}")
    print(f"VERIFICATION RESULTS")
    print(f"{'='*60}")
    print(f"Total matched in DB:    {total}")
    print(f"Byte-exact verified:    {verified}")
    print(f"Byte mismatch:          {mismatched}")
    print(f"Not found in compiled:  {not_compiled}")
    print(f"{'='*60}")

    if verified == total:
        print(f"\nAll {total} matches verified byte-exact.")
    else:
        print(f"\n{total - verified} functions need attention.")

    if fix and problem_addrs:
        print(f"\n--fix: resetting {len(problem_addrs)} functions to 'untried'...")
        for f in functions:
            if f["address"] in problem_addrs:
                f["match_status"] = "untried"
        save_db(functions)
        print(f"Reset {len(problem_addrs)} functions to 'untried'.")

    return verified, mismatched, not_compiled, total


def main():
    parser = argparse.ArgumentParser(description="Verify all matched functions are byte-exact")
    parser.add_argument("--verbose", "-v", action="store_true", help="Show each function")
    parser.add_argument("--fix", action="store_true",
                        help="Reset mismatched/missing functions to 'untried'")
    args = parser.parse_args()
    verify_all(verbose=args.verbose, fix=args.fix)


if __name__ == "__main__":
    main()
