#!/usr/bin/env python3
"""Cluster original Read prologue byte prefixes from the function DB."""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from common import EBOOT_PATH, TEXT_FILE_OFFSET, load_db  # noqa: E402


DEFAULT_METHOD = "Read(cFile &, cMemPool *)"


def _read_prefix(func: dict, words: int) -> list[str]:
    size = int(func["size"])
    byte_count = min(size, words * 4)
    addr = int(func["address"], 16)
    with open(EBOOT_PATH, "rb") as f:
        f.seek(TEXT_FILE_OFFSET + addr)
        data = f.read(byte_count)
    return [
        f"0x{int.from_bytes(data[i:i + 4], 'little'):08x}"
        for i in range(0, len(data) - (len(data) % 4), 4)
    ]


def _filter_funcs(args: argparse.Namespace) -> list[dict]:
    funcs = []
    statuses = set(args.status or [])
    for func in load_db():
        if args.method and args.method not in func.get("name", ""):
            continue
        if args.size is not None and int(func["size"]) != args.size:
            continue
        if statuses and func.get("match_status") not in statuses:
            continue
        funcs.append(func)
    return funcs


def build_clusters(args: argparse.Namespace) -> list[dict]:
    grouped: dict[tuple[str, ...], list[dict]] = defaultdict(list)
    for func in _filter_funcs(args):
        grouped[tuple(_read_prefix(func, args.words))].append(func)

    clusters = []
    for prefix, funcs in grouped.items():
        status_counts = Counter(func.get("match_status") for func in funcs)
        clusters.append({
            "count": len(funcs),
            "statuses": dict(sorted(status_counts.items())),
            "prefix": list(prefix),
            "examples": [
                {
                    "address": func["address"],
                    "size": func["size"],
                    "status": func.get("match_status"),
                    "name": func["name"],
                    "src_file": func.get("src_file"),
                }
                for func in funcs[:args.examples]
            ],
        })

    clusters.sort(key=lambda row: (-row["count"], row["prefix"]))
    return clusters


def _write_markdown(clusters: list[dict], args: argparse.Namespace, path: Path) -> None:
    lines = [
        "# Read Prologue Clusters",
        "",
        f"- Method filter: `{args.method}`",
        f"- Size filter: `{args.size if args.size is not None else '(any)'}`",
        f"- Status filter: `{args.status or '(any)'}`",
        f"- Prefix words: `{args.words}`",
        f"- Cluster count: `{len(clusters)}`",
        "",
    ]
    for i, cluster in enumerate(clusters[:args.limit], 1):
        lines.append(f"## Cluster {i}: {cluster['count']} rows")
        lines.append("")
        lines.append(f"- Statuses: `{cluster['statuses']}`")
        lines.append("- Prefix:")
        lines.append("")
        lines.append("```text")
        for offset, word in enumerate(cluster["prefix"]):
            lines.append(f"0x{offset * 4:02x}: {word}")
        lines.append("```")
        lines.append("")
        lines.append("| Address | Size | Status | Function | Source |")
        lines.append("|---|---:|---|---|---|")
        for example in cluster["examples"]:
            lines.append(
                f"| `{example['address']}` | `{example['size']}` | "
                f"`{example['status']}` | `{example['name']}` | "
                f"`{example['src_file'] or ''}` |"
            )
        lines.append("")
    path.write_text("\n".join(lines))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--method", default=DEFAULT_METHOD,
                        help=f"Function-name substring (default: {DEFAULT_METHOD}).")
    parser.add_argument("--size", type=int,
                        help="Exact function size filter.")
    parser.add_argument("--status", action="append",
                        help="Match status filter. Can be repeated.")
    parser.add_argument("--words", type=int, default=12,
                        help="Number of 32-bit prologue words to group by.")
    parser.add_argument("--examples", type=int, default=12,
                        help="Examples to include per cluster.")
    parser.add_argument("--limit", type=int, default=20,
                        help="Clusters to print/write.")
    parser.add_argument("--json-out",
                        help="Optional JSON output path.")
    parser.add_argument("--md-out",
                        help="Optional Markdown output path.")
    args = parser.parse_args()

    clusters = build_clusters(args)
    if args.json_out:
        Path(args.json_out).write_text(json.dumps(clusters, indent=2) + "\n")
    if args.md_out:
        _write_markdown(clusters, args, Path(args.md_out))

    print(f"clustered {sum(c['count'] for c in clusters)} functions into {len(clusters)} prefixes")
    for i, cluster in enumerate(clusters[:args.limit], 1):
        names = ", ".join(example["address"] for example in cluster["examples"][:5])
        print(f"{i:2d}. {cluster['count']:3d} rows statuses={cluster['statuses']} examples={names}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
