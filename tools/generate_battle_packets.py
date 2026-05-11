#!/usr/bin/env python3
"""Generate Markdown battle packets for matching targets."""

from __future__ import annotations

import argparse
import json
import sys
from datetime import date
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from battle_packets import packet_filename, render_packet  # noqa: E402
from common import build_addr_map, load_db  # noqa: E402
from failure_classifier import required_size, required_text, validate_address  # noqa: E402
from path_guards import repo_output_path  # noqa: E402


def _load_targets(path: str | None) -> list[dict]:
    if not path:
        return []
    with open(path) as f:
        data = json.load(f)
    if not isinstance(data, list):
        raise SystemExit(f"{path} must be a JSON array")
    return data


def _select_functions(functions: list[dict], args) -> tuple[list[dict], dict[str, dict]]:
    addr_map = build_addr_map(functions)
    target_meta: dict[str, dict] = {}
    ordered: list[dict] = []

    for idx, target in enumerate(_load_targets(args.targets), 1):
        if not isinstance(target, dict):
            raise SystemExit(f"{args.targets}: row {idx} must be an object")
        if "address" not in target:
            raise SystemExit(f"{args.targets}: row {idx} missing address")
        addr = validate_address(target["address"], context=f"{args.targets}: row {idx} address")
        func = addr_map.get(addr)
        if not func:
            raise SystemExit(f"{args.targets}: row {idx} address {addr} not found in DB")
        if addr in target_meta:
            raise SystemExit(f"{args.targets}: duplicate address {addr} at row {idx}")
        target_meta[addr] = target
        ordered.append(func)

    cli_addresses = args.address if args.address is not None else []
    for addr in cli_addresses:
        addr = validate_address(addr, context="--address")
        func = addr_map.get(addr)
        if not func:
            raise SystemExit(f"address {addr} not found in DB")
        if addr not in target_meta:
            target_meta[addr] = {}
            ordered.append(func)

    if not ordered and args.status:
        ordered = [f for f in functions if required_text(f, "match_status") == args.status]

    if args.only_failed:
        ordered = [f for f in ordered if required_text(f, "match_status") == "failed"]
    if args.only_untried:
        ordered = [f for f in ordered if required_text(f, "match_status") == "untried"]
    if args.size_min is not None:
        ordered = [f for f in ordered if required_size(f) >= args.size_min]
    if args.size_max is not None:
        ordered = [f for f in ordered if required_size(f) <= args.size_max]

    if args.limit is not None:
        ordered = ordered[:args.limit]
    return ordered, target_meta


def _write_index(output_dir: Path, packets: list[tuple[dict, Path]]) -> None:
    lines = [
        "# Battle Packets",
        "",
        f"Generated: {date.today().isoformat()}",
        "",
        "| Address | Size | Status | Function | Packet |",
        "|---|---:|---|---|---|",
    ]
    for func, path in packets:
        rel = path.name
        lines.append(
            f"| `{validate_address(required_text(func, 'address'))}` | {required_size(func)} | "
            f"`{required_text(func, 'match_status')}` | `{required_text(func, 'name')}` | "
            f"[packet]({rel}) |"
        )
    output_dir.joinpath("index.md").write_text("\n".join(lines) + "\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--targets", help="Target JSON file; order is preserved.")
    parser.add_argument("--address", action="append",
                        help="Address to packetize; may be repeated.")
    parser.add_argument("--status", choices=["failed", "untried", "matched"],
                        help="Packetize DB rows by status when no targets/address given.")
    parser.add_argument("--only-failed", action="store_true")
    parser.add_argument("--only-untried", action="store_true")
    parser.add_argument("--size-min", type=int)
    parser.add_argument("--size-max", type=int)
    parser.add_argument("--limit", type=int, default=20)
    parser.add_argument("--output-dir", default=f"docs/research/battle-packets/{date.today():%Y%m%d}")
    parser.add_argument("--stdout", action="store_true",
                        help="Print one packet to stdout instead of writing files.")
    parser.add_argument("--no-disasm", action="store_true")
    parser.add_argument("--no-m2c", action="store_true")
    parser.add_argument("--no-exemplars", action="store_true")
    parser.add_argument("--no-header", action="store_true")
    args = parser.parse_args()

    if not (args.targets or args.address or args.status):
        raise SystemExit("provide --targets, --address, or --status")
    if args.only_failed and args.only_untried:
        raise SystemExit("--only-failed and --only-untried are mutually exclusive")

    functions = load_db()
    selected, target_meta = _select_functions(functions, args)
    if not selected:
        raise SystemExit("no functions selected")

    if args.stdout:
        if len(selected) != 1:
            raise SystemExit("--stdout requires exactly one selected function")
        print(render_packet(
            selected[0],
            functions,
            target_metadata=target_meta.get(validate_address(selected[0]["address"])),
            include_disasm=not args.no_disasm,
            include_m2c=not args.no_m2c,
            include_exemplars=not args.no_exemplars,
            include_header=not args.no_header,
        ))
        return 0

    output_dir = repo_output_path(
        args.output_dir,
        allowed_roots=("docs/research/battle-packets",),
        label="--output-dir",
    )
    output_dir.mkdir(parents=True, exist_ok=True)
    written: list[tuple[dict, Path]] = []
    for func in selected:
        path = output_dir / packet_filename(func)
        path.write_text(render_packet(
            func,
            functions,
            target_metadata=target_meta.get(validate_address(func["address"])),
            include_disasm=not args.no_disasm,
            include_m2c=not args.no_m2c,
            include_exemplars=not args.no_exemplars,
            include_header=not args.no_header,
        ))
        written.append((func, path))
    _write_index(output_dir, written)
    print(f"Wrote {len(written)} packet(s) to {output_dir}")
    print(f"Index: {output_dir / 'index.md'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
