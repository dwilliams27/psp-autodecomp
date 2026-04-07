#!/usr/bin/env python3
"""Parse SN Systems linker map into splat-compatible symbol address file.

Usage:
    python3 tools/map_parser.py extern/extracted_symbols/Game-dvd.map config/symbol_addrs.txt
"""

import re
import sys


# Symbol line: 8-char hex addr, space, 8-char hex size, spaces, then name
# These are indented with lots of spaces (function/data symbols at deepest indent)
SYMBOL_RE = re.compile(
    r"^([0-9a-f]{8})\s+([0-9a-f]{8})\s+\d+\s{20,}(\S.+)$"
)

# Compilation unit line: indented path ending in .obj
OBJ_RE = re.compile(r"^\s+[a-zA-Z]:\\.*\.obj$")

# Section header line: addr size align then section name (no deep indent)
SECTION_RE = re.compile(r"^[0-9a-f]{8}\s+[0-9a-f]{8}\s+\d+\s+\.\w")


ILLEGAL_FILENAME_CHARS = set('<>:"/\\|?*()&,; ')


def make_safe_filename(name, addr):
    """Convert a C++ demangled name to a filesystem-safe filename.

    Replaces illegal characters and truncates to 253 bytes. Uses the hex
    address as a suffix to guarantee uniqueness when names collide after
    sanitization (e.g., overloaded functions that differ only in parameter types).
    """
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
    safe = safe.replace("(", "_")
    safe = safe.replace(")", "")
    safe = safe.replace(",", "_")
    safe = safe.replace(" ", "")
    safe = safe.replace("*", "ptr")
    safe = safe.replace("&", "ref")
    safe = safe.replace("<", "_")
    safe = safe.replace(">", "_")

    # Final sweep for anything we missed
    safe = "".join(c if c not in ILLEGAL_FILENAME_CHARS else "_" for c in safe)

    # Truncate if over filesystem limit, keeping address suffix for uniqueness
    if len(safe.encode("utf-8")) > 240:
        safe = safe[:220] + f"_{addr:08X}"

    return safe


def parse_map(map_path):
    """Parse .map file, yield (addr, size, name, section) tuples."""
    current_section = None

    with open(map_path, "r") as f:
        for line in f:
            line = line.rstrip()

            # Track which section we're in
            section_match = SECTION_RE.match(line)
            if section_match:
                # Extract section name (last token on the line)
                parts = line.split()
                if len(parts) >= 4:
                    current_section = parts[3]
                continue

            # Skip compilation unit headers
            if OBJ_RE.match(line):
                continue

            # Match symbol lines
            sym_match = SYMBOL_RE.match(line)
            if sym_match:
                addr = int(sym_match.group(1), 16)
                size = int(sym_match.group(2), 16)
                name = sym_match.group(3).strip()

                # Skip unnamed/internal symbols
                if name.startswith("__NO_NAME"):
                    continue

                yield addr, size, name, current_section


def classify_symbol(name, section):
    """Determine if a symbol is a function or data.

    Returns a splat-compatible type string. splat recognizes 'func' for
    functions and specific data types like 's32', 'u32', etc. for data.
    We use 'label' for generic data symbols since it's the most permissive
    splat data type — it creates a label without assuming a specific size/format.
    """
    if "virtual table" in name:
        return "label"
    if section and section in (".data", ".rodata", ".bss", ".linkonce.d"):
        return "label"
    # Linker-generated symbols
    if name in ("_gp", "_edata", "__bss_start", "_fbss", "_end"):
        return "label"
    return "func"


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <map_file> <output_file>", file=sys.stderr)
        sys.exit(1)

    map_path = sys.argv[1]
    output_path = sys.argv[2]

    raw_symbols = list(parse_map(map_path))

    # Deduplicate by address. splat requires unique addresses. When two symbols
    # share an address, keep the one with a larger size (more informative), or
    # the one with a more specific name (not a section-level marker like "RDATA.").
    by_addr = {}
    for addr, size, name, section in raw_symbols:
        if addr in by_addr:
            prev_size, prev_name, prev_section = by_addr[addr]
            # Prefer larger size (more informative symbol)
            if size > prev_size:
                by_addr[addr] = (size, name, section)
            # At equal size, prefer names starting with a letter (human-readable)
            # over section markers like "$LNst.0" or "RDATA."
            elif size == prev_size and name[0].isalpha() and not prev_name[0].isalpha():
                by_addr[addr] = (size, name, section)
        else:
            by_addr[addr] = (size, name, section)

    symbols = [(addr, size, name, section) for addr, (size, name, section) in sorted(by_addr.items())]
    deduped = len(raw_symbols) - len(symbols)

    func_count = 0
    data_count = 0
    skipped = 0

    with open(output_path, "w") as f:
        for addr, size, name, section in symbols:
            # Skip zero-address zero-size entries (section markers)
            if addr == 0 and size == 0:
                skipped += 1
                continue

            sym_type = classify_symbol(name, section)

            # splat's symbol_addrs format uses '=' as the name/address delimiter,
            # so symbol names containing '=' break parsing. C++ operator= (assignment)
            # is the only case in this binary (184 symbols). We mangle to operator_eq.
            #
            # Any tool that converts these names back to C++ source must reverse this:
            #   "operator_eq" -> "operator="
            # This is a permanent convention, not a TODO — splat cannot be changed.
            # splat writes each symbol to its own file, so the name must be a
            # valid filename (≤253 bytes, no <>:"/\|?* characters). C++ demangled
            # names routinely contain these (::, (), *, &, const). Generate a safe
            # filename and attach it via splat's filename: attribute.
            #
            # We also need unique names for splat's symbol deduplication. The same
            # C++ signature can appear at multiple addresses (e.g., template
            # instantiations or duplicate symbols across compilation units). Append
            # the address to make each entry unique.
            filename = make_safe_filename(name, addr)
            unique_name = f"{filename}__{addr:08X}"
            attrs = f"type:{sym_type} size:0x{size:X} filename:{filename}"

            f.write(f"{unique_name} = 0x{addr:08X}; // {attrs}\n")
            if sym_type == "func":
                func_count += 1
            else:
                data_count += 1

    print(f"Wrote {func_count} functions + {data_count} data symbols to {output_path}")
    print(f"Skipped {skipped} zero-address entries, deduped {deduped} duplicate addresses")


if __name__ == "__main__":
    main()
