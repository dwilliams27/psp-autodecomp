#!/usr/bin/env python3
"""Build a small string-xref index for extern/snc/pspcor.exe.

The original compiler-internals notes referenced a temporary string index.
This script regenerates the useful subset on demand using only system tools:
`strings` for candidate text and `objdump` for code references.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from pathlib import Path


DEFAULT_EXE = Path("extern/snc/pspcor.exe")
DEFAULT_PATTERNS = [
    r"CG_",
    r"\bsched\b",
    r"CG_Schedule",
    r"pre_sched",
    r"post_sched",
    r"zp_pick",
    r"\bLRA\b",
    r"LRA_",
    r"<lra",
    r"\bGRA\b",
    r"GRA\(",
    r"<gra",
    r"asmstart",
    r"asmend",
    r"OP_MACHINE",
    r"OP_INITCALL",
    r"OP_PUSHARG",
    r"OP_CALL",
]


def _run(cmd: list[str]) -> str:
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or result.stdout.strip())
    return result.stdout


def _image_base(exe: Path) -> int:
    out = _run(["objdump", "-x", str(exe)])
    m = re.search(r"ImageBase\s+([0-9a-fA-F]+)", out)
    if not m:
        raise RuntimeError("could not find ImageBase in objdump -x output")
    return int(m.group(1), 16)


def _strings(exe: Path, patterns: list[str], image_base: int) -> list[dict]:
    combined = re.compile("|".join(f"(?:{pat})" for pat in patterns), re.IGNORECASE)
    out = _run(["strings", "-a", "-t", "x", str(exe)])
    rows = []
    for line in out.splitlines():
        m = re.match(r"\s*([0-9a-fA-F]+)\s+(.*)", line)
        if not m:
            continue
        text = m.group(2)
        if not combined.search(text):
            continue
        file_off = int(m.group(1), 16)
        rows.append({
            "file_offset": f"0x{file_off:06x}",
            "va": f"0x{image_base + file_off:08x}",
            "text": text,
        })
    return rows


def _disassembly(exe: Path) -> list[str]:
    return _run(["objdump", "-d", str(exe)]).splitlines()


def _xrefs(disasm_lines: list[str], va: str, context: int) -> list[dict]:
    needle = f"{int(va, 16):x}"
    pattern = re.compile(rf"\b(?:0x)?0*{re.escape(needle)}\b", re.IGNORECASE)
    hits = []
    for i, line in enumerate(disasm_lines):
        if not pattern.search(line):
            continue
        start = max(0, i - context)
        end = min(len(disasm_lines), i + context + 1)
        hits.append({
            "line": i + 1,
            "text": line,
            "context": disasm_lines[start:end],
        })
    return hits


def build_index(exe: Path, patterns: list[str], context: int) -> dict:
    image_base = _image_base(exe)
    string_rows = _strings(exe, patterns, image_base)
    disasm_lines = _disassembly(exe)
    for row in string_rows:
        row["xrefs"] = _xrefs(disasm_lines, row["va"], context)
    return {
        "exe": str(exe),
        "image_base": f"0x{image_base:08x}",
        "patterns": patterns,
        "strings": string_rows,
    }


def write_markdown(index: dict, path: Path) -> None:
    lines = [
        "# pspcor String Xrefs",
        "",
        f"- Executable: `{index['exe']}`",
        f"- Image base: `{index['image_base']}`",
        f"- Patterns: `{index['patterns']}`",
        "",
    ]
    for row in index["strings"]:
        lines.append(f"## {row['va']} `{row['text']}`")
        lines.append("")
        lines.append(f"- File offset: `{row['file_offset']}`")
        lines.append(f"- Xrefs: `{len(row['xrefs'])}`")
        lines.append("")
        for hit in row["xrefs"]:
            lines.append(f"### objdump line {hit['line']}")
            lines.append("")
            lines.append("```asm")
            lines.extend(hit["context"])
            lines.append("```")
            lines.append("")
    path.write_text("\n".join(lines))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", default=str(DEFAULT_EXE),
                        help=f"pspcor.exe path (default: {DEFAULT_EXE})")
    parser.add_argument("--pattern", action="append", default=[],
                        help="Regex string pattern. Can be repeated.")
    parser.add_argument("--context", type=int, default=2,
                        help="Objdump lines of context around each xref.")
    parser.add_argument("--json-out",
                        help="Optional JSON output path.")
    parser.add_argument("--md-out",
                        help="Optional Markdown output path.")
    args = parser.parse_args()

    patterns = args.pattern or DEFAULT_PATTERNS
    index = build_index(Path(args.exe), patterns, args.context)

    if args.json_out:
        Path(args.json_out).write_text(json.dumps(index, indent=2) + "\n")
    if args.md_out:
        write_markdown(index, Path(args.md_out))

    print(f"indexed {len(index['strings'])} strings from {args.exe}")
    with_xrefs = sum(1 for row in index["strings"] if row["xrefs"])
    print(f"strings with code xrefs: {with_xrefs}")
    for row in index["strings"]:
        if row["xrefs"]:
            print(f"  {row['va']} {row['text']} ({len(row['xrefs'])} xref(s))")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
