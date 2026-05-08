#!/usr/bin/env python3
"""Prototype the final Read-prologue list transform on emitted assembly.

This is a research harness, not a build-system fix. It proves that the high
value 188B Read drift can be repaired by a narrow final prologue reorder:
move the saved `s3` store earlier, and move the inline-asm result assignment
before the `cReadBlock` constructor call.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from byte_match import symbols_with_bytes_and_relocs  # noqa: E402
from common import EBOOT_PATH, TEXT_FILE_OFFSET, load_db, mask_relocation_bytes  # noqa: E402


BAD_CFACTORY_HYBRID = [
    "\tsubu\t$sp,$sp,48",
    "\tsw\t$16,20($sp)",
    "\tsw\t$18,28($sp)",
    "\tor\t$16,$6,$0",
    "\tor\t$18,$4,$0",
    "\tsw\t$17,24($sp)",
    "\tor\t$17,$5,$0",
    "\tor\t$4,$sp,$0",
    "\tor\t$6,$0,1",
    "\tsw\t$19,32($sp)",
    "\tsw\t$31,36($sp)",
    "\tjal\t__0oKcReadBlockctR6FcFileUib",
    "\tor\t$7,$0,1",
    "\t#asm{",
    ".set reorder",
    "\tori $19,$0,0x00000001",
    ".set noreorder",
    "\t#}asm",
]

GOOD_CFACTORY_HYBRID = [
    "\tsubu\t$sp,$sp,48",
    "\tsw\t$16,20($sp)",
    "\tsw\t$18,28($sp)",
    "\tor\t$16,$6,$0",
    "\tor\t$18,$4,$0",
    "\tsw\t$17,24($sp)",
    "\tsw\t$19,32($sp)",
    "\tor\t$17,$5,$0",
    "\t#asm{",
    ".set reorder",
    "\tori $19,$0,0x00000001",
    ".set noreorder",
    "\t#}asm",
    "\tor\t$4,$sp,$0",
    "\tor\t$6,$0,1",
    "\tsw\t$31,36($sp)",
    "\tjal\t__0oKcReadBlockctR6FcFileUib",
    "\tor\t$7,$0,1",
]


def _norm_addr(addr: str) -> str:
    return f"0x{int(addr, 16):08x}"


def _find_func(addr: str) -> dict:
    needle = _norm_addr(addr)
    for func in load_db():
        if func["address"] == needle:
            return func
    raise SystemExit(f"no DB function at {addr}")


def transform_cfactory_hybrid(lines: list[str]) -> tuple[list[str], int]:
    hits = []
    width = len(BAD_CFACTORY_HYBRID)
    for index in range(0, len(lines) - width + 1):
        if lines[index:index + width] == BAD_CFACTORY_HYBRID:
            hits.append(index)
    if len(hits) != 1:
        raise SystemExit(
            "expected exactly one cFactory hybrid prologue pattern, "
            f"found {len(hits)}"
        )
    index = hits[0]
    return (
        lines[:index] + GOOD_CFACTORY_HYBRID + lines[index + width:],
        index + 1,
    )


def _assemble(asm_path: Path, obj_path: Path) -> None:
    result = subprocess.run(
        [
            "mipsel-linux-gnu-as",
            "-march=allegrex",
            "-mabi=eabi",
            "-EL",
            "-Iinclude",
            "-o",
            str(obj_path),
            str(asm_path),
        ],
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        raise SystemExit(result.stderr.strip() or result.stdout.strip())


def _expected_bytes(func: dict) -> bytes:
    addr = int(func["address"], 16)
    size = int(func["size"])
    with open(EBOOT_PATH, "rb") as f:
        f.seek(TEXT_FILE_OFFSET + addr)
        return f.read(size)


def _compare(func: dict, obj_path: Path, symbol: str) -> tuple[int, int, list[tuple[int, int]]]:
    symbols = symbols_with_bytes_and_relocs(str(obj_path))
    if symbol not in symbols:
        preview = "\n".join(sorted(symbols)[:20])
        raise SystemExit(f"symbol {symbol} not found in {obj_path}\n{preview}")

    compiled, _sym_off, relocs = symbols[symbol]
    expected = _expected_bytes(func)
    cmp_len = min(len(expected), len(compiled))
    func_relocs = [(off, typ) for off, typ in relocs if 0 <= off < cmp_len]
    masked_expected = mask_relocation_bytes(expected[:cmp_len], func_relocs)
    masked_compiled = mask_relocation_bytes(compiled[:cmp_len], func_relocs)
    diff_count = (
        sum(a != b for a, b in zip(masked_expected, masked_compiled))
        + abs(len(masked_expected) - len(masked_compiled))
    )
    return len(compiled), diff_count, func_relocs


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("asm", help="SNC-generated assembly file to transform.")
    parser.add_argument("--addr", default="0x0000ab98",
                        help="DB function address to compare.")
    parser.add_argument("--symbol",
                        help="Mangled symbol to compare; defaults from DB.")
    parser.add_argument("--out-dir",
                        default="build/research/read_prologue/asm_transform",
                        help="Output directory for transformed assembly/object/report.")
    args = parser.parse_args()

    func = _find_func(args.addr)
    symbol = (
        args.symbol
        or func.get("symbol_name")
        or func.get("mangled_symbol")
    )
    if not symbol:
        raise SystemExit(f"DB row for {args.addr} has no symbol")

    asm_path = Path(args.asm)
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    patched_asm = out_dir / f"{func['address'][2:]}-patched.s"
    patched_obj = out_dir / f"{func['address'][2:]}-patched.o"
    report_path = out_dir / f"{func['address'][2:]}-report.md"

    lines = asm_path.read_text(errors="replace").splitlines()
    patched_lines, line_no = transform_cfactory_hybrid(lines)
    patched_asm.write_text("\n".join(patched_lines) + "\n")
    _assemble(patched_asm, patched_obj)
    compiled_size, masked_diffs, relocs = _compare(func, patched_obj, symbol)

    report = [
        f"# Read Prologue Assembly Transform: {func['address']}",
        "",
        f"- Function: `{func['name']}`",
        f"- Input assembly: `{asm_path}`",
        f"- Patched assembly: `{patched_asm}`",
        f"- Patched object: `{patched_obj}`",
        f"- Pattern line: `{line_no}`",
        f"- Compiled size: `{compiled_size}`",
        f"- Verification-masked byte diffs: `{masked_diffs}/{func['size']}`",
        f"- Function-relative relocations: `{relocs}`",
        "",
        "This validates the exact final instruction-list transform to reproduce",
        "inside `pspcor.exe`; it is not a build-system workaround.",
        "",
    ]
    report_path.write_text("\n".join(report))
    print(f"wrote {report_path}")
    print(f"masked diffs: {masked_diffs}/{func['size']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
