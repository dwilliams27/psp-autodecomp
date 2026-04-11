#!/usr/bin/env python3
"""Function database for decompilation project.

Parses the SN Systems linker map into a structured JSON file that agents
can query by class, size, .obj file, etc.

Usage:
    python3 tools/func_db.py build
    python3 tools/func_db.py query [--class NAME] [--size-min N] [--size-max N]
                                   [--obj FILE] [--name SUBSTR] [--status STATUS]
                                   [--leaf] [--limit N]
    python3 tools/func_db.py stats
    python3 tools/func_db.py set-status ADDRESS STATUS
"""

import argparse
import json
import os
import re
import sys

from common import DB_PATH, MAP_PATH, load_db, save_db, filter_functions, build_addr_map

# Same regexes as map_parser.py
SYMBOL_RE = re.compile(
    r"^([0-9a-f]{8})\s+([0-9a-f]{8})\s+\d+\s{20,}(\S.+)$"
)
OBJ_RE = re.compile(
    r"^([0-9a-f]{8})\s+([0-9a-f]{8})\s+\d+\s{16,}(c:\\.*\.obj)$"
)
# Match section header lines (both output and input sections)
# e.g. "00000000 0036bab4     4 .text" or "001c3a98 001a801c     4         .gnu.linkonce.t.*"
SECTION_RE = re.compile(r"^[0-9a-f]{8}\s+[0-9a-f]{8}\s+\d+\s+\.[\w.*]")
# Also match input section lines with deeper indentation
INPUT_SECTION_RE = re.compile(r"^[0-9a-f]{8}\s+[0-9a-f]{8}\s+\d+\s{9,}\.([\w.*]+)")

DATA_SECTIONS = frozenset({
    ".data", ".rodata", ".bss", ".linkonce.d",
    ".ctors", ".dtors", ".cplinit", ".eh_frame",
    ".lib.ent", ".lib.ent.top", ".lib.ent.btm",
    ".lib.stub", ".lib.stub.top", ".lib.stub.btm",
})


def extract_class_method(name):
    """Extract class name and method name from a C++ demangled name.

    Returns (class_name, method_name) or (None, name) for free functions.
    """
    # Handle operator overloads with :: before splitting
    # e.g. "gcDoReadFile::operator=(const gcDoReadFile &)"
    # Find the last :: that's NOT inside operator
    parts = name.split("::")
    if len(parts) < 2:
        return None, name

    # Rejoin — the class is everything before the last ::
    # But handle nested namespaces: std::exception::what → class=std::exception, method=what
    method = parts[-1]
    class_name = "::".join(parts[:-1])

    # Strip parameter list from method for cleaner display
    paren = method.find("(")
    if paren >= 0:
        method = method[:paren]

    return class_name, method


def make_safe_name(name, addr):
    """Convert demangled name to a safe identifier. Mirrors map_parser.py logic."""
    safe = name.replace("::", "__")
    safe = safe.replace("operator=", "operator_eq")
    safe = safe.replace("operator<<", "operator_shl")
    safe = safe.replace("operator>>", "operator_shr")
    safe = safe.replace("operator<", "operator_lt")
    safe = safe.replace("operator>", "operator_gt")
    safe = safe.replace("operator()", "operator_call")
    safe = safe.replace("operator[]", "operator_idx")
    safe = safe.replace("operator+", "operator_add")
    safe = safe.replace("operator-", "operator_sub")
    safe = safe.replace("operator*", "operator_mul")
    safe = safe.replace("operator/", "operator_div")
    safe = safe.replace("operator&", "operator_and")
    safe = safe.replace("operator|", "operator_or")
    safe = safe.replace("~", "_dtor_")
    safe = safe.replace("(", "_")
    safe = safe.replace(")", "")
    safe = safe.replace(",", "_")
    safe = safe.replace(" ", "")
    safe = safe.replace("*", "ptr")
    safe = safe.replace("&", "ref")
    safe = safe.replace("<", "_")
    safe = safe.replace(">", "_")
    safe = safe.replace("[", "_arr_")
    safe = safe.replace("]", "")
    safe = re.sub(r'[<>:"/\\|?*]', "_", safe)
    if len(safe.encode("utf-8")) > 240:
        safe = safe[:220] + f"_{addr:08X}"
    return f"{safe}__{addr:08X}"


def parse_map_to_functions(map_path):
    """Parse linker map into function records with .obj tracking."""
    current_section = None
    current_obj = None
    functions = []
    seen_addrs = {}

    with open(map_path, "r") as f:
        for line in f:
            line = line.rstrip()

            # Track section (both output sections and input subsections)
            section_match = SECTION_RE.match(line)
            if section_match:
                parts = line.split()
                if len(parts) >= 4:
                    current_section = parts[3]
                current_obj = None
                continue

            input_match = INPUT_SECTION_RE.match(line)
            if input_match:
                current_section = "." + input_match.group(1)
                continue

            # Track .obj file
            obj_match = OBJ_RE.match(line)
            if obj_match:
                obj_path = obj_match.group(3)
                current_obj = obj_path.split("\\")[-1]  # just the filename
                continue

            # Match symbol lines
            sym_match = SYMBOL_RE.match(line)
            if not sym_match:
                continue

            addr = int(sym_match.group(1), 16)
            size = int(sym_match.group(2), 16)
            name = sym_match.group(3).strip()

            if name.startswith("__NO_NAME"):
                continue
            # PSP module metadata — linker-generated, not real code
            if name.startswith("__PSPEXP__") or name.startswith("__PSPREN__"):
                continue

            is_func = True
            if "virtual table" in name:
                is_func = False
            elif current_section and current_section in DATA_SECTIONS:
                is_func = False
            elif current_section and ".rodata" in current_section:
                is_func = False
            elif current_section and ".data" in current_section:
                is_func = False
            elif current_section and ".bss" in current_section:
                is_func = False
            elif current_section and "linkonce.d" in current_section:
                is_func = False
            elif current_section and "linkonce.b" in current_section:
                is_func = False
            elif current_section and "linkonce.r" in current_section:
                is_func = False

            if not is_func:
                continue

            # Deduplicate by address (keep larger/better name)
            if addr in seen_addrs:
                prev = seen_addrs[addr]
                if size > prev["size"]:
                    seen_addrs[addr] = None  # will be replaced
                elif size == prev["size"] and name[0].isalpha() and not prev["name"][0].isalpha():
                    seen_addrs[addr] = None
                else:
                    continue

            class_name, method_name = extract_class_method(name)
            is_static = name.rstrip().endswith(" static")

            record = {
                "address": f"0x{addr:08x}",
                "size": size,
                "name": name,
                "safe_name": make_safe_name(name, addr),
                "class_name": class_name,
                "method_name": method_name,
                "obj_file": current_obj,
                "section": current_section,
                "is_static": is_static,
                "match_status": "untried",
            }

            seen_addrs[addr] = record
            functions.append(record)

    # Remove duplicates that were superseded
    functions = [f for f in functions if seen_addrs.get(int(f["address"], 16)) is f]
    functions.sort(key=lambda f: int(f["address"], 16))
    return functions


def cmd_build(args):
    """Parse map file and create functions.json."""
    if not os.path.exists(MAP_PATH):
        print(f"Error: {MAP_PATH} not found.", file=sys.stderr)
        sys.exit(1)

    # If DB exists, preserve match_status and failure_notes
    old_data = {}
    if os.path.exists(DB_PATH):
        old = load_db()
        old_data = build_addr_map(old)

    functions = parse_map_to_functions(MAP_PATH)

    # Restore preserved fields from previous build
    for func in functions:
        old_func = old_data.get(func["address"])
        if old_func:
            func["match_status"] = old_func.get("match_status", "untried")
            if "failure_notes" in old_func:
                func["failure_notes"] = old_func["failure_notes"]

    save_db(functions)

    total = len(functions)
    with_class = sum(1 for f in functions if f["class_name"])
    unique_classes = len(set(f["class_name"] for f in functions if f["class_name"]))
    obj_files = set(f["obj_file"] for f in functions if f["obj_file"])

    print(f"Built {DB_PATH}: {total} functions")
    print(f"  {with_class} class methods across {unique_classes} classes")
    print(f"  {total - with_class} free functions")
    print(f"  {len(obj_files)} compilation units: {', '.join(sorted(obj_files))}")


def cmd_query(args):
    """Query functions with filters."""
    functions = load_db()

    results = filter_functions(functions, class_name=args.class_name, name=args.name,
                               obj=args.obj, size_min=args.size_min, size_max=args.size_max,
                               status=args.status, leaf=args.leaf, limit=args.limit)

    if args.json:
        print(json.dumps(results, indent=2))
    else:
        print(f"{len(results)} functions found:\n")
        for f in results:
            status = f["match_status"][0].upper()  # U/M/F/I
            print(f"  [{status}] {f['address']}  {f['size']:>6}B  {f['name']}")
            if f["obj_file"]:
                print(f"       obj: {f['obj_file']}")


def cmd_stats(args):
    """Print summary statistics."""
    functions = load_db()

    total = len(functions)
    total_bytes = sum(f["size"] for f in functions)

    # Size buckets
    buckets = [
        ("≤8B", 0, 8),
        ("9-64B", 9, 64),
        ("65-256B", 65, 256),
        ("257-1024B", 257, 1024),
        (">1024B", 1025, 999999999),
    ]

    print(f"Total: {total} functions, {total_bytes:,} bytes ({total_bytes / 1024 / 1024:.2f} MB)\n")
    print("Size distribution:")
    for label, lo, hi in buckets:
        count = sum(1 for f in functions if lo <= f["size"] <= hi)
        bytes_ = sum(f["size"] for f in functions if lo <= f["size"] <= hi)
        print(f"  {label:>10}: {count:>5} ({count*100/total:5.1f}%)  {bytes_:>10,} bytes ({bytes_*100/total_bytes:5.1f}%)")

    # Match status
    statuses = {}
    for f in functions:
        s = f["match_status"]
        statuses[s] = statuses.get(s, 0) + 1
    print(f"\nMatch status:")
    for s, count in sorted(statuses.items()):
        print(f"  {s}: {count}")

    # Top classes
    class_counts = {}
    for f in functions:
        if f["class_name"]:
            class_counts[f["class_name"]] = class_counts.get(f["class_name"], 0) + 1
    top = sorted(class_counts.items(), key=lambda x: -x[1])[:10]
    print(f"\nTop 10 classes by method count:")
    for cls, count in top:
        print(f"  {cls}: {count}")

    # .obj files
    obj_counts = {}
    for f in functions:
        if f["obj_file"]:
            obj_counts[f["obj_file"]] = obj_counts.get(f["obj_file"], 0) + 1
    print(f"\nCompilation units:")
    for obj, count in sorted(obj_counts.items(), key=lambda x: -x[1]):
        print(f"  {obj}: {count}")


def cmd_set_status(args):
    """Update match status for a function."""
    functions = load_db()
    addr = args.address.lower()
    if not addr.startswith("0x"):
        addr = "0x" + addr

    found = False
    for f in functions:
        if f["address"] == addr:
            old = f["match_status"]
            f["match_status"] = args.status
            save_db(functions)
            print(f"Updated {f['name']}: {old} → {args.status}")
            found = True
            break

    if not found:
        print(f"Error: no function at address {addr}", file=sys.stderr)
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(description="Function database for decompilation")
    sub = parser.add_subparsers(dest="command")

    sub.add_parser("build", help="Parse linker map into functions.json")

    q = sub.add_parser("query", help="Query functions")
    q.add_argument("--class", dest="class_name", help="Filter by class name (substring)")
    q.add_argument("--name", help="Filter by function name (substring)")
    q.add_argument("--obj", help="Filter by .obj file (substring)")
    q.add_argument("--size-min", type=int, help="Minimum size in bytes")
    q.add_argument("--size-max", type=int, help="Maximum size in bytes")
    q.add_argument("--status", help="Filter by match status")
    q.add_argument("--leaf", action="store_true", help="Only leaf functions")
    q.add_argument("--limit", type=int, help="Max results")
    q.add_argument("--json", action="store_true", help="Output as JSON")

    sub.add_parser("stats", help="Print summary statistics")

    ss = sub.add_parser("set-status", help="Update match status")
    ss.add_argument("address", help="Function address (hex)")
    ss.add_argument("status", choices=["untried", "in_progress", "matched", "failed"],
                    help="New status")

    args = parser.parse_args()
    if not args.command:
        parser.print_help()
        sys.exit(1)

    {"build": cmd_build, "query": cmd_query, "stats": cmd_stats, "set-status": cmd_set_status}[args.command](args)


if __name__ == "__main__":
    main()
