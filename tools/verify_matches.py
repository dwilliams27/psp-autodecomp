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
                    load_db, save_db,
                    get_text_relocations, mask_relocation_bytes)


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
        # Store (bytes, symbol_offset_in_text) so callers can adjust relocations
        result[name] = (text_bytes[offset:end], offset)

    return result


def find_all_compiled_symbols():
    """Scan all .o files in build/src/ and return {symbol_name: (bytes, o_path)}."""
    build_dir = "build/src"
    if not os.path.isdir(build_dir):
        raise FileNotFoundError(
            f"{build_dir} does not exist — nothing compiled to verify against"
        )

    all_symbols = {}
    errors = []
    for root, _, files in os.walk(build_dir):
        for fname in files:
            if not fname.endswith(".o"):
                continue
            o_path = os.path.join(root, fname)
            try:
                symbols = get_compiled_symbols(o_path)
                for name, (data, sym_offset) in symbols.items():
                    all_symbols[name] = (data, o_path, sym_offset)
            except RuntimeError as e:
                errors.append((o_path, str(e)))

    if errors:
        # Same reasoning as the compile-error raise: partial symbol data
        # means the audit can't decide verification correctly.
        details = "; ".join(f"{p}: {m}" for p, m in errors[:5])
        raise RuntimeError(
            f"{len(errors)} .o files could not be read: {details}"
            f"{'...' if len(errors) > 5 else ''}"
        )

    return all_symbols


# SNC operator code table (docs/research/snc-name-mangling.md §6.1).
# Member operators are mangled __0o<class><op_code><params>; global new/delete
# as __0O<op_code><params>. If an operator isn't in this table we can't
# verify it — that's a tool gap, not a silent pass.
_OP_CODES = {
    "operator+": "pl", "operator-": "mi", "operator*": "ml", "operator/": "dv",
    "operator%": "md", "operator=": "as", "operator==": "eq", "operator!=": "ne",
    "operator<": "lt", "operator>": "gt", "operator<=": "le", "operator>=": "ge",
    "operator+=": "apl", "operator-=": "ami", "operator*=": "amu",
    "operator/=": "adv", "operator%=": "amd", "operator&": "an", "operator|": "or",
    "operator^": "er", "operator~": "co", "operator!": "nt", "operator&&": "aa",
    "operator||": "oo", "operator<<": "ls", "operator>>": "rs",
    "operator<<=": "als", "operator>>=": "ars", "operator()": "cl",
    "operator[]": "vc", "operator->": "rf", "operator++": "pp",
    "operator--": "mm", "operator,": "cm",
    "operator new": "nw", "operator delete": "dl",
    "operator new[]": "nwa", "operator delete[]": "dla",
}


def _sym_matches_name(sym_name, cls, method):
    """True iff sym_name encodes the (cls, method) pair per SNC mangling
    rules in docs/research/snc-name-mangling.md. Handles ctors, dtors,
    operators, free functions, and regular instance/static methods.

    Returns False (not raising) when the check can't be decided — these
    show up as NO MATCHING SYMBOL, not as silently-verified matches.
    """
    if not method:
        return False

    # Dtor: method_name = "~<class>" → __0o<classLen><class>dt<params>
    if method.startswith("~"):
        return bool(cls) and method[1:] == cls and (cls + "dt") in sym_name

    # Ctor: method_name == class_name → __0o<classLen><class>ct<params>
    if cls and method == cls:
        return (cls + "ct") in sym_name

    # Operator: member __0o<class><op><params>, global __0O<op><params>
    if method.startswith("operator"):
        op_code = _OP_CODES.get(method)
        if not op_code:
            return False
        if cls:
            return (cls + op_code) in sym_name
        return sym_name.startswith(f"__0O{op_code}")

    # Regular instance / static method:
    # __0f<classLen><class><methodLen><method><params>[T for static]
    if cls:
        ci = sym_name.find(cls)
        if ci < 0:
            return False
        mi = sym_name.find(method, ci + len(cls))
        if mi < 0:
            return False
        # Gap is the length-prefix letter(s): 1 char for names 1-51, 2 chars
        # for 52-103, 3 for longer. See §2.1 of the mangling doc.
        gap = mi - (ci + len(cls))
        return 1 <= gap <= 3

    # Free function: __0F<methodLen><method><params>
    return method in sym_name


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
    compile_errors = []
    for src in src_files:
        o_path = src.replace("src/", "build/src/") + ".o"
        result = subprocess.run(["make", o_path], capture_output=True, text=True)
        if result.returncode != 0:
            print(f"  compile error: {src}")
            compile_errors.append(src)
    if compile_errors:
        # A compile failure means we have no .o for that src, so any
        # claimed matches whose real symbol lives there will show up as
        # NO MATCHING SYMBOL even though they might have been real at
        # commit time. --fix would then destroy those records.
        # Refuse to --fix while compile is broken; warn otherwise.
        if fix:
            raise RuntimeError(
                f"{len(compile_errors)} source files failed to compile. "
                f"Refusing to --fix while any src is broken — a compile "
                f"failure causes false NO MATCHING SYMBOL reports and "
                f"would destroy real matches. Broken files: "
                f"{', '.join(compile_errors[:5])}"
                f"{'...' if len(compile_errors) > 5 else ''}"
            )
        print(f"  WARNING: {len(compile_errors)} src files failed to compile; "
              f"their matches will show as NO MATCHING SYMBOL below.")
        print(f"           Do NOT run --fix until these are fixed:")
        for src in compile_errors:
            print(f"             {src}")

    # Build symbol map from all compiled .o files
    print("Scanning compiled symbols...")
    all_symbols = find_all_compiled_symbols()
    print(f"Found {len(all_symbols)} compiled symbols\n")

    verified = 0
    mismatched = 0
    problem_addrs = set()

    # Pre-compute relocations for all .o files (keyed by o_path)
    reloc_cache = {}
    for sym_name, (_, o_file, _) in all_symbols.items():
        if o_file not in reloc_cache:
            reloc_cache[o_file] = get_text_relocations(o_file)

    with open(EBOOT_PATH, "rb") as eboot:
        for func in matched:
            addr = int(func["address"], 16)
            size = func["size"]
            expected = get_original_bytes(eboot, addr, size)
            cls = func.get("class_name") or ""
            method = func.get("method_name") or ""

            # Only symbols whose mangled form actually encodes this
            # function's (class, method) pair are considered. Eliminates
            # the historical first-byte-match-wins false positive where
            # e.g. an 8-byte wrapper matched an unrelated symbol in an
            # unrelated .o file just by byte-pattern coincidence.
            name_candidates = [
                (sn, cb, of, so)
                for sn, (cb, of, so) in all_symbols.items()
                if _sym_matches_name(sn, cls, method)
            ]

            found = False
            for sym_name, compiled_bytes, o_file, sym_offset in name_candidates:
                if len(compiled_bytes) < size:
                    continue

                compiled_func = compiled_bytes[:size]
                expected_func = expected

                # Filter relocations to those within this symbol's range,
                # adjusting offsets to be relative to the symbol start
                all_relocs = reloc_cache.get(o_file, [])
                func_relocs = []
                for off, rtype in all_relocs:
                    relative = off - sym_offset
                    if 0 <= relative < size:
                        func_relocs.append((relative, rtype))

                if func_relocs:
                    compiled_func = mask_relocation_bytes(compiled_func, func_relocs)
                    expected_func = mask_relocation_bytes(expected_func, func_relocs)

                if compiled_func == expected_func:
                    verified += 1
                    found = True
                    if verbose:
                        reloc_note = f" ({len(func_relocs)} relocs masked)" if func_relocs else ""
                        print(f"  ✓ {func['address']}  {size:>4}B  {func['name']}{reloc_note}")
                    break

            if not found:
                problem_addrs.add(func["address"])
                mismatched += 1
                if not name_candidates:
                    reason = "NO MATCHING SYMBOL"
                else:
                    reason = "BYTE MISMATCH"
                print(f"  ✗ {func['address']}  {size:>4}B  {func['name']} — {reason}")

    total = len(matched)
    print(f"\n{'='*60}")
    print(f"VERIFICATION RESULTS")
    print(f"{'='*60}")
    print(f"Total matched in DB:    {total}")
    print(f"Byte-exact verified:    {verified}")
    print(f"Byte mismatch:          {mismatched}")
    print(f"Not found in compiled:  0")
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

    return verified, mismatched, total


def main():
    parser = argparse.ArgumentParser(description="Verify all matched functions are byte-exact")
    parser.add_argument("--verbose", "-v", action="store_true", help="Show each function")
    parser.add_argument("--fix", action="store_true",
                        help="Reset mismatched/missing functions to 'untried'")
    args = parser.parse_args()
    verify_all(verbose=args.verbose, fix=args.fix)


if __name__ == "__main__":
    main()
