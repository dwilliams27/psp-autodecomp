#!/usr/bin/env python3
"""Extract call graph from the original binary.

Scans JAL instructions in .text to build caller→callee relationships.
Enriches config/functions.json with callees, callers, and is_leaf fields.

Usage:
    python3 tools/call_graph.py build           # scan binary, update functions.json
    python3 tools/call_graph.py show QUERY       # show callees + callers for a function
    python3 tools/call_graph.py leaves           # list all leaf functions
    python3 tools/call_graph.py leaves --size-max 64  # small leaf functions
"""

import argparse
import os
import sys

from common import (DB_PATH, EBOOT_PATH, TEXT_FILE_OFFSET,
                    load_db, save_db, find_function, build_addr_map, filter_functions)


def build_range_lookup(functions):
    """Build sorted list of (start, end, func) for range lookups."""
    ranges = []
    for f in functions:
        addr = int(f["address"], 16)
        if f["size"] > 0:
            ranges.append((addr, addr + f["size"], f))
    ranges.sort()
    return ranges


def find_containing_func(ranges, addr):
    """Find which function contains a given address using binary search."""
    lo, hi = 0, len(ranges) - 1
    while lo <= hi:
        mid = (lo + hi) // 2
        start, end, func = ranges[mid]
        if addr < start:
            hi = mid - 1
        elif addr >= end:
            lo = mid + 1
        else:
            return func
    return None


def scan_jal_instructions(functions):
    """Scan the .text section for JAL instructions by reading raw bytes.

    MIPS JAL opcode = 0x03 in bits 31-26. Target = (immediate << 2).
    This is ~100x faster than disassembling via objdump and uses ~4MB RAM.

    Returns dict: caller_address → set of callee_addresses
    """
    text_end = max(int(f["address"], 16) + f["size"] for f in functions if f["size"] > 0)

    print(f"Scanning {text_end / 1024 / 1024:.1f} MB of .text for JAL instructions...", file=sys.stderr)

    with open(EBOOT_PATH, "rb") as f:
        f.seek(TEXT_FILE_OFFSET)
        text_data = f.read(text_end)

    ranges = build_range_lookup(functions)
    caller_to_callees = {}

    for offset in range(0, len(text_data) - 3, 4):
        word = int.from_bytes(text_data[offset:offset + 4], "little")
        opcode = (word >> 26) & 0x3F
        if opcode != 0x03:  # JAL opcode
            continue

        callee_addr = (word & 0x03FFFFFF) << 2
        caller_func = find_containing_func(ranges, offset)
        if caller_func:
            func_addr = int(caller_func["address"], 16)
            if func_addr not in caller_to_callees:
                caller_to_callees[func_addr] = set()
            caller_to_callees[func_addr].add(callee_addr)

    return caller_to_callees


def cmd_build(args):
    """Scan binary and update functions.json with call graph data."""
    functions = load_db()
    addr_map = build_addr_map(functions)

    caller_to_callees = scan_jal_instructions(functions)

    # Build callee → callers reverse map
    callee_to_callers = {}
    for caller_addr, callee_addrs in caller_to_callees.items():
        for callee_addr in callee_addrs:
            if callee_addr not in callee_to_callers:
                callee_to_callers[callee_addr] = set()
            callee_to_callers[callee_addr].add(caller_addr)

    # Update functions
    leaf_count = 0
    for f in functions:
        addr = int(f["address"], 16)
        callees = sorted(caller_to_callees.get(addr, set()))
        callers = sorted(callee_to_callers.get(addr, set()))
        f["callees"] = [f"0x{a:08x}" for a in callees]
        f["callers"] = [f"0x{a:08x}" for a in callers]
        f["is_leaf"] = len(callees) == 0
        if f["is_leaf"] and f["size"] > 0:
            leaf_count += 1

    save_db(functions)

    total = sum(1 for f in functions if f["size"] > 0)
    with_calls = sum(1 for f in functions if f.get("callees") and len(f["callees"]) > 0)
    print(f"Updated {DB_PATH}:")
    print(f"  {total} functions analyzed")
    print(f"  {with_calls} functions make calls")
    print(f"  {leaf_count} leaf functions (no calls)")


def cmd_show(args):
    """Show call graph for a function."""
    functions = load_db()
    func = find_function(functions, args.query)
    if not func:
        print(f"Error: function not found: {args.query}", file=sys.stderr)
        sys.exit(1)

    addr_map = build_addr_map(functions)

    print(f"{func['name']} ({func['address']}, {func['size']}B)")
    print(f"  Leaf: {func.get('is_leaf', 'unknown')}")

    callees = func.get("callees", [])
    if callees:
        print(f"\n  Calls ({len(callees)}):")
        for addr in callees:
            target = addr_map.get(addr)
            name = target["name"] if target else "???"
            print(f"    → {addr}  {name}")
    else:
        print("\n  Calls: none (leaf function)")

    callers = func.get("callers", [])
    if callers:
        print(f"\n  Called by ({len(callers)}):")
        for addr in callers:
            caller = addr_map.get(addr)
            name = caller["name"] if caller else "???"
            print(f"    ← {addr}  {name}")
    else:
        print("\n  Called by: none")


def cmd_leaves(args):
    """List leaf functions (optionally filtered by size)."""
    functions = load_db()
    results = filter_functions(functions, size_min=args.size_min or 1, size_max=args.size_max,
                               obj=args.obj, leaf=True, limit=args.limit)

    print(f"{len(results)} leaf functions:\n")
    for f in results:
        status = f["match_status"][0].upper()
        print(f"  [{status}] {f['address']}  {f['size']:>6}B  {f['name']}")


def main():
    parser = argparse.ArgumentParser(description="Call graph extraction")
    sub = parser.add_subparsers(dest="command")

    sub.add_parser("build", help="Scan binary and update functions.json")

    s = sub.add_parser("show", help="Show call graph for a function")
    s.add_argument("query", help="Function name or address")

    l = sub.add_parser("leaves", help="List leaf functions")
    l.add_argument("--size-min", type=int)
    l.add_argument("--size-max", type=int)
    l.add_argument("--obj", help="Filter by .obj file")
    l.add_argument("--limit", type=int)

    args = parser.parse_args()
    if not args.command:
        parser.print_help()
        sys.exit(1)

    {"build": cmd_build, "show": cmd_show, "leaves": cmd_leaves}[args.command](args)


if __name__ == "__main__":
    main()
