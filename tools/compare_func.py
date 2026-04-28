#!/usr/bin/env python3
"""Compile a source file and report which DB functions each emitted symbol
matches (agent-facing direction: src → DB lookup).

Sits on top of tools/byte_match.py for the compile/nm/objcopy work.
Unlike the canonical `check_byte_match` (DB → src direction, name-gated),
this tool is exploratory: an agent has written src and wants to know what
it just matched, so we compare each emitted symbol's bytes against every
same-size DB candidate.

Usage:
    python3 tools/compare_func.py src/eWorld.cpp
    python3 tools/compare_func.py src/eWorld.cpp --symbol __0fGeWorldJLockWorldbK
    python3 tools/compare_func.py --batch src/
"""

import argparse
import os
import sys

from common import (EBOOT_PATH, TEXT_FILE_OFFSET,
                    load_db, save_db,
                    mask_relocation_bytes)
from byte_match import (
    CompileFailed,
    compile_src,
    find_db_func_for_sym,
    symbols_with_bytes_and_relocs,
)


def load_eboot():
    with open(EBOOT_PATH, "rb") as f:
        return f.read()


def compare_sym_bytes(compiled_bytes, func_relocs, func, eboot_data):
    """Compare compiled bytes (sized to `func.size`) against EBOOT bytes at
    func.address, with relocation masking.

    Returns diff_count — 0 on exact match. Raises on short EBOOT read.
    """
    size = func["size"]
    func_addr = int(func["address"], 16)
    start = func_addr + TEXT_FILE_OFFSET
    expected = eboot_data[start:start + size]
    if len(expected) != size:
        raise RuntimeError(
            f"Short EBOOT read for {func['name']} at {func['address']}: "
            f"got {len(expected)}/{size}B. EBOOT may be truncated."
        )
    compiled = compiled_bytes[:size]
    if func_relocs:
        compiled = mask_relocation_bytes(compiled, func_relocs)
        expected = mask_relocation_bytes(expected, func_relocs)
    if compiled == expected:
        return 0
    return sum(1 for a, b in zip(compiled, expected) if a != b)


def compare_file(src_path, symbol_filter=None, functions=None, eboot_data=None,
                 batch_mode=False):
    """Compile src_path, for each emitted symbol find the matching DB function.

    Returns list of (sym_name, matched, func_record, message). In batch
    mode a compile failure is reported and the file is skipped (returning
    []), so one bad file doesn't abort a full-tree sweep. In single-file
    mode compile failures propagate so the shell exit code reflects them.
    """
    if functions is None:
        functions = load_db()
    if eboot_data is None:
        eboot_data = load_eboot()

    try:
        o_path = compile_src(src_path)
    except CompileFailed as e:
        if not batch_mode:
            raise
        print(f"Compilation failed for {src_path}: {e}", file=sys.stderr)
        return []

    # Use the unified symbol extractor that handles both T (.text) and
    # W (.gnu.linkonce.t.*) symbols — the latter are template instantiations.
    all_syms = symbols_with_bytes_and_relocs(o_path)

    if symbol_filter:
        all_syms = {n: v for n, v in all_syms.items() if n == symbol_filter}

    if not all_syms:
        print(f"No text symbols found in {o_path}")
        return []

    results = []
    for sym_name, (sym_bytes, sym_off, sym_relocs) in all_syms.items():
        sym_size = len(sym_bytes)
        # Name-first: resolve the emitted sym to ONE DB entry via mangling.
        # Without this step, 8-byte trivial wrappers byte-match any other
        # 8-byte trivial wrapper and we'd report false matches.
        func = find_db_func_for_sym(sym_name, functions)
        if func is None:
            msg = "NO DB ENTRY: no database function has this mangled name"
            print(f"  \u2717 {sym_name} \u2014 {msg}")
            results.append((sym_name, False, None, msg))
            continue

        if func["size"] > sym_size:
            msg = (f"SIZE MISMATCH: DB has {func['name']} at "
                   f"{func['size']}B but sym is {sym_size}B")
            print(f"  \u2717 {sym_name} \u2014 {msg}")
            results.append((sym_name, False, func, msg))
            continue

        compiled = sym_bytes[:func["size"]]
        # sym_relocs are already function-relative; filter to func size.
        func_relocs = [(off, rtype) for off, rtype in sym_relocs
                       if 0 <= off < func["size"]]
        diff_count = compare_sym_bytes(compiled, func_relocs, func, eboot_data)

        if diff_count == 0:
            reloc_note = f" (with {len(func_relocs)} reloc(s) masked)" if func_relocs else ""
            msg = f"MATCH{reloc_note}"
            print(f"  \u2713 {sym_name} \u2014 {msg} ({func['name']}, {func['size']}B)")
            results.append((sym_name, True, func, msg))
        else:
            msg = f"MISMATCH: {diff_count}/{func['size']} bytes differ ({func['name']})"
            print(f"  \u2717 {sym_name} \u2014 {msg}")
            results.append((sym_name, False, func, msg))

    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
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

    def update_matched(func, src_path, sym_name):
        if func and args.update_db and func["address"] in addr_index:
            entry = addr_index[func["address"]]
            entry["match_status"] = "matched"
            # Record provenance so downstream audits don't have to guess.
            entry["src_file"] = src_path
            entry["symbol_name"] = sym_name

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
            results = compare_file(src, functions=functions, eboot_data=eboot_data,
                                   batch_mode=True)
            for sym, matched, func, _diff in results:
                total_compared += 1
                if matched:
                    total_matched += 1
                    update_matched(func, src, sym)
        print(f"\nTotal: {total_matched}/{total_compared} matched")
    else:
        results = compare_file(args.source, args.symbol, functions, eboot_data,
                               batch_mode=False)
        for sym, matched, func, _diff in results:
            if matched:
                update_matched(func, args.source, sym)

    if args.update_db and functions:
        save_db(functions)


if __name__ == "__main__":
    main()
