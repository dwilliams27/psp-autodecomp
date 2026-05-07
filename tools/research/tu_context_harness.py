#!/usr/bin/env python3
"""Phase 1 TU-context register-allocation research harness.

Compiles one candidate C/C++ translation unit with SNC into a research temp
directory, extracts one exact target symbol, and scores it against EBOOT bytes.
The harness appends one JSON row per run and never writes the function DB.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
from datetime import datetime, timezone


REPO_ROOT = Path(__file__).resolve().parents[2]
TOOLS_DIR = REPO_ROOT / "tools"
if str(TOOLS_DIR) not in sys.path:
    sys.path.insert(0, str(TOOLS_DIR))

from byte_match import (  # noqa: E402
    build_byte_diffs,
    read_eboot_bytes,
    symbols_with_bytes_and_relocs,
)
from common import mask_relocation_bytes  # noqa: E402


WIBO = "extern/wibo"
SNC = "extern/snc/pspsnc.exe"
BASE_CFLAGS = [
    "-c",
    "-O2",
    "-G0",
    "-Xvfpumatrix=1",
    "-Xvfpuscalar=8",
    "-Iextern/include",
    "-Iinclude",
]
DEFAULT_OUT = "logs/research/tu_context_regalloc/results.jsonl"
DEFAULT_TMP_ROOT = "logs/research/tu_context_regalloc/tmp"


def load_functions() -> list[dict]:
    with open(REPO_ROOT / "config/functions.json", "r") as f:
        return json.load(f)


def normalize_addr(text: str) -> str | None:
    if re.fullmatch(r"0x[0-9a-fA-F]+", text):
        return f"0x{int(text, 16):08x}"
    if re.fullmatch(r"[0-9a-fA-F]{1,8}", text):
        return f"0x{int(text, 16):08x}"
    return None


def resolve_target(target: str, functions: list[dict]) -> dict:
    addr = normalize_addr(target)
    if addr is not None:
        matches = [f for f in functions if f.get("address") == addr]
        if len(matches) != 1:
            raise ValueError(f"target address {addr} matched {len(matches)} DB rows")
        func = dict(matches[0])
    else:
        matches = [
            f for f in functions
            if target in (f.get("mangled_symbol"), f.get("symbol_name"))
        ]
        if len(matches) != 1:
            raise ValueError(
                f"target symbol {target!r} matched {len(matches)} DB rows"
            )
        func = dict(matches[0])

    symbol = func.get("mangled_symbol") or func.get("symbol_name")
    if not symbol:
        raise ValueError(
            f"{func.get('address')} {func.get('name')} has no exact symbol; "
            "this harness does not fall back to same-size symbols"
        )
    func["_target_symbol"] = symbol
    return func


def experiment_id(target: str, source: Path, label: str, flags: list[str]) -> str:
    h = hashlib.sha1()
    h.update(target.encode())
    h.update(b"\0")
    h.update(str(source.resolve()).encode())
    h.update(b"\0")
    h.update(label.encode())
    h.update(b"\0")
    h.update("\0".join(flags).encode())
    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    return f"{stamp}-{h.hexdigest()[:10]}"


def compile_source(src_path: Path, o_path: Path, cflags: list[str]) -> tuple[str, str]:
    cmd = [WIBO, SNC] + cflags + ["-o", str(o_path), str(src_path)]
    try:
        result = subprocess.run(
            cmd,
            cwd=REPO_ROOT,
            capture_output=True,
            text=True,
            timeout=60,
        )
    except subprocess.TimeoutExpired as e:
        return "timeout", (e.stderr or e.stdout or "SNC timed out")[:2000]

    output = (result.stderr or result.stdout or "").strip()
    if result.returncode != 0:
        return "failed", output[:2000]
    if not o_path.exists():
        return "failed", f"SNC succeeded but did not produce {o_path}"
    return "ok", output[:2000]


def first_diff_summary(
    compiled: bytes,
    expected: bytes,
    relocs: list[tuple[int, int]],
    addr: int,
) -> tuple[int, list[dict]]:
    usable_relocs = [
        (off, rtype)
        for off, rtype in relocs
        if off >= 0 and off + 4 <= len(compiled) and off + 4 <= len(expected)
    ]
    if usable_relocs:
        compiled = mask_relocation_bytes(compiled, usable_relocs)
        expected = mask_relocation_bytes(expected, usable_relocs)

    overlap = min(len(compiled), len(expected))
    byte_diff_count = sum(
        1 for i in range(overlap) if compiled[i] != expected[i]
    ) + abs(len(compiled) - len(expected))
    return byte_diff_count, build_byte_diffs(compiled, expected, addr)


def display_path(path: Path) -> str:
    try:
        return str(path.relative_to(REPO_ROOT))
    except ValueError:
        return str(path)


def base_row(args: argparse.Namespace, func: dict, exp_id: str,
             workdir: Path, src_snapshot: Path, o_path: Path,
             cflags: list[str]) -> dict:
    return {
        "experiment_id": exp_id,
        "timestamp_utc": datetime.now(timezone.utc).isoformat(),
        "label": args.label,
        "target": {
            "query": args.target,
            "address": func.get("address"),
            "name": func.get("name"),
            "symbol": func["_target_symbol"],
            "size": int(func["size"]),
        },
        "seed_description": args.label,
        "source": str(Path(args.source)),
        "source_snapshot": display_path(src_snapshot),
        "workdir": display_path(workdir),
        "o_file": display_path(o_path),
        "sched": int(args.sched),
        "flags": cflags,
    }


def append_jsonl(path: Path, row: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "a") as f:
        f.write(json.dumps(row, sort_keys=True) + "\n")


def run(args: argparse.Namespace) -> int:
    os.chdir(REPO_ROOT)
    functions = load_functions()
    func = resolve_target(args.target, functions)

    source = Path(args.source)
    if not source.exists():
        raise FileNotFoundError(f"source not found: {source}")
    if source.suffix not in (".c", ".cpp", ".cc", ".cxx"):
        raise ValueError(f"source must be C/C++: {source}")

    cflags = BASE_CFLAGS + [f"-Xsched={args.sched}"] + list(args.cflag)
    exp_id = experiment_id(args.target, source, args.label, cflags)
    tmp_root = Path(args.tmp_root)
    workdir = tmp_root / exp_id
    workdir.mkdir(parents=True, exist_ok=True)

    src_snapshot = workdir / source.name
    shutil.copy2(source, src_snapshot)
    cflags_for_compile = cflags + [f"-I{source.resolve().parent}"]
    o_path = workdir / f"{source.name}.o"

    row = base_row(args, func, exp_id, workdir, src_snapshot, o_path,
                   cflags_for_compile)
    compile_status, compile_output = compile_source(
        src_snapshot, o_path, cflags_for_compile
    )
    row["compile_status"] = compile_status
    if compile_output:
        row["compile_output"] = compile_output

    if compile_status != "ok":
        append_jsonl(Path(args.out), row)
        print(json.dumps(row, sort_keys=True))
        return 2

    try:
        syms = symbols_with_bytes_and_relocs(str(o_path))
        target_symbol = func["_target_symbol"]
        row["symbol_found"] = target_symbol in syms
        if target_symbol not in syms:
            row["available_symbols"] = sorted(syms)[:50]
            row["score_status"] = "missing_exact_symbol"
            append_jsonl(Path(args.out), row)
            print(json.dumps(row, sort_keys=True))
            return 3

        sym_bytes, _sym_off, relocs = syms[target_symbol]
        expected_size = int(func["size"])
        addr = int(func["address"], 16)
        expected = read_eboot_bytes(addr, expected_size)
        compiled = sym_bytes[:expected_size]
        if len(sym_bytes) < expected_size:
            compiled = sym_bytes

        diff_count, diffs = first_diff_summary(compiled, expected, relocs, addr)
        row.update({
            "score_status": "scored",
            "symbol_found": True,
            "symbol_name": target_symbol,
            "symbol_size": len(sym_bytes),
            "target_size": expected_size,
            "size_matches": len(sym_bytes) == expected_size,
            "diff_count": diff_count,
            "first_diff_summary": diffs,
            "relocations_masked": len([
                1 for off, _rtype in relocs
                if off >= 0 and off + 4 <= len(compiled)
                and off + 4 <= len(expected)
            ]),
            "exact_match": diff_count == 0 and len(sym_bytes) == expected_size,
        })
    except Exception as e:
        row["score_status"] = "tooling_error"
        row["error"] = str(e)
        append_jsonl(Path(args.out), row)
        print(json.dumps(row, sort_keys=True))
        return 4

    append_jsonl(Path(args.out), row)
    print(json.dumps(row, sort_keys=True))
    return 0


def normalize_argv(argv: list[str]) -> list[str]:
    out: list[str] = []
    i = 0
    while i < len(argv):
        if argv[i] == "--cflag" and i + 1 < len(argv):
            out.append(f"--cflag={argv[i + 1]}")
            i += 2
            continue
        out.append(argv[i])
        i += 1
    return out


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        "--target",
        required=True,
        help="Target DB address, e.g. 0x0007ae64, or exact mangled symbol.",
    )
    parser.add_argument(
        "--source",
        required=True,
        help="Candidate C/C++ translation unit to compile.",
    )
    parser.add_argument(
        "--sched",
        choices=("1", "2"),
        default="2",
        help="SNC scheduler flag value for -Xsched=N (default: 2).",
    )
    parser.add_argument(
        "--cflag",
        action="append",
        default=[],
        help="Extra SNC flag; repeat for multiple flags.",
    )
    parser.add_argument(
        "--label",
        default="unnamed",
        help="Short seed/experiment label recorded in JSONL.",
    )
    parser.add_argument(
        "--out",
        default=DEFAULT_OUT,
        help=f"JSONL output path (default: {DEFAULT_OUT}).",
    )
    parser.add_argument(
        "--tmp-root",
        default=DEFAULT_TMP_ROOT,
        help=f"Temporary compile root (default: {DEFAULT_TMP_ROOT}).",
    )
    return parser.parse_args(normalize_argv(argv))


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv or sys.argv[1:])
    try:
        return run(args)
    except Exception as e:
        print(f"error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
