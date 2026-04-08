#!/usr/bin/env python3
"""Shared utilities for decompilation tools."""

import json
import os
import sys

DB_PATH = "config/functions.json"
EBOOT_PATH = "extern/iso_extract/PSP_GAME/SYSDIR/EBOOT.BIN.dec"
MAP_PATH = "extern/extracted_symbols/Game-dvd.map"
TEXT_FILE_OFFSET = 0xA0  # .text section file offset in the PRX

OBJDUMP = "mipsel-linux-gnu-objdump"
OBJCOPY = "mipsel-linux-gnu-objcopy"
NM = "mipsel-linux-gnu-nm"
AS = "mipsel-linux-gnu-as"
AS_FLAGS = ["-march=allegrex", "-mabi=eabi", "-EL"]
CLAUDE = "/usr/local/bin/claude"


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
