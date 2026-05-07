#!/usr/bin/env python3
"""Reproduce cReadBlock Read prologue scheduling differences.

This is the first ML2 harness: it extracts original EBOOT bytes for selected
`Read(cFile &, cMemPool *)` functions, compiles the current source candidate,
and writes side-by-side prologue disassembly artifacts.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from byte_match import CompileFailed, compile_src, symbols_with_bytes_and_relocs  # noqa: E402
from common import EBOOT_PATH, OBJDUMP, TEXT_FILE_OFFSET, load_db, mask_relocation_bytes  # noqa: E402


DEFAULT_ADDRS = ["0x0005dccc", "0x0000ab98"]
DEFAULT_OUT = Path("build/research/read_prologue")


def _norm_addr(addr: str) -> str:
    return f"0x{int(addr, 16):08x}"


def _find_func(addr: str) -> dict:
    needle = _norm_addr(addr)
    for func in load_db():
        if func["address"] == needle:
            return func
    raise SystemExit(f"no DB function at {addr}")


def _source_override_map(items: list[str]) -> dict[str, str]:
    out: dict[str, str] = {}
    for item in items:
        if "=" not in item:
            raise SystemExit("--source entries must be ADDR=PATH")
        addr, path = item.split("=", 1)
        out[_norm_addr(addr)] = path
    return out


def _infer_source(func: dict, overrides: dict[str, str]) -> str | None:
    addr = func["address"]
    if addr in overrides:
        return overrides[addr]

    src_file = func.get("src_file")
    if src_file and Path(src_file).exists():
        return src_file

    cls = func.get("class_name")
    if cls:
        candidate = Path("src") / f"{cls}.cpp"
        if candidate.exists():
            return str(candidate)

    notes = func.get("failure_notes") or []
    for note in reversed(notes):
        for key in ("snapshot", "src_file"):
            val = note.get(key)
            if val and Path(val).exists():
                return val

    return None


def _expected_bytes(func: dict) -> bytes:
    addr = int(func["address"], 16)
    size = int(func["size"])
    with open(EBOOT_PATH, "rb") as f:
        f.seek(TEXT_FILE_OFFSET + addr)
        data = f.read(size)
    if len(data) != size:
        raise RuntimeError(f"short EBOOT read for {func['address']}: {len(data)} != {size}")
    return data


def _disassemble_raw(data: bytes, out_bin: Path) -> str:
    out_bin.parent.mkdir(parents=True, exist_ok=True)
    out_bin.write_bytes(data)
    result = subprocess.run(
        [OBJDUMP, "-D", "-b", "binary", "-m", "mips:allegrex", "-EL", str(out_bin)],
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or result.stdout.strip())
    return result.stdout


def _compile_source(src_file: str) -> str:
    try:
        return compile_src(src_file)
    except CompileFailed as exc:
        raise SystemExit(str(exc)) from exc


def _symbol_bytes(o_path: str, func: dict) -> tuple[str, bytes, list[tuple[int, int]]]:
    symbols = symbols_with_bytes_and_relocs(o_path)
    preferred = [
        func.get("symbol_name"),
        func.get("mangled_symbol"),
    ]
    for name in preferred:
        if name and name in symbols:
            sym_bytes, _sym_off, relocs = symbols[name]
            return name, sym_bytes, relocs

    method = func.get("method_name") or ""
    candidates = [name for name in symbols if method and method in name]
    if len(candidates) == 1:
        sym_bytes, _sym_off, relocs = symbols[candidates[0]]
        return candidates[0], sym_bytes, relocs

    preview = "\n".join(sorted(symbols)[:20])
    raise RuntimeError(
        f"could not locate compiled symbol for {func['address']} {func['name']}\n"
        f"preferred={preferred}\nfirst symbols:\n{preview}"
    )


def _objdump_symbol(o_path: str, sym_name: str) -> str:
    result = subprocess.run([OBJDUMP, "-dr", o_path], capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr.strip() or result.stdout.strip())

    lines = result.stdout.splitlines()
    start = None
    sym_pat = f"<{sym_name}>:"
    for i, line in enumerate(lines):
        if sym_pat in line:
            start = i
            break
    if start is None:
        return f"(symbol {sym_name} not found in objdump output)\n"

    out = []
    for line in lines[start:]:
        if out and re.match(r"^[0-9a-fA-F]+\s+<[^>]+>:", line):
            break
        out.append(line)
    return "\n".join(out) + "\n"


def _word_diff(expected: bytes, compiled: bytes, limit_bytes: int) -> list[str]:
    lines = []
    n = min(limit_bytes, len(expected), len(compiled))
    for off in range(0, n - (n % 4), 4):
        ew = int.from_bytes(expected[off:off + 4], "little")
        cw = int.from_bytes(compiled[off:off + 4], "little")
        marker = "==" if ew == cw else "!="
        lines.append(f"0x{off:02x}: 0x{ew:08x} {marker} 0x{cw:08x}")
    return lines


def _masked_for_diff(expected: bytes, compiled: bytes,
                     relocs: list[tuple[int, int]]) -> tuple[bytes, bytes, list[tuple[int, int]]]:
    cmp_len = min(len(expected), len(compiled))
    func_relocs = [(off, rtype) for off, rtype in relocs if 0 <= off < cmp_len]
    if not func_relocs:
        return expected[:cmp_len], compiled[:cmp_len], []
    return (
        mask_relocation_bytes(expected[:cmp_len], func_relocs),
        mask_relocation_bytes(compiled[:cmp_len], func_relocs),
        func_relocs,
    )


def analyze_address(func: dict, src_file: str | None, out_dir: Path,
                    window_bytes: int, compile_source: bool) -> Path:
    addr = func["address"]
    safe = addr[2:]
    case_dir = out_dir / safe
    case_dir.mkdir(parents=True, exist_ok=True)

    expected = _expected_bytes(func)
    expected_disasm = _disassemble_raw(expected[:window_bytes], case_dir / "expected_prologue.bin")
    (case_dir / "expected_prologue.disasm").write_text(expected_disasm)
    (case_dir / "expected_full.bin").write_bytes(expected)

    compiled = b""
    masked_expected = b""
    masked_compiled = b""
    relocs: list[tuple[int, int]] = []
    sym_name = None
    o_path = None
    compiled_disasm = "(not compiled)\n"
    object_disasm = "(not compiled)\n"
    if compile_source:
        if not src_file:
            raise SystemExit(f"no source candidate found for {addr} {func['name']}")
        o_path = _compile_source(src_file)
        sym_name, compiled, relocs = _symbol_bytes(o_path, func)
        masked_expected, masked_compiled, relocs = _masked_for_diff(expected, compiled, relocs)
        compiled_disasm = _disassemble_raw(
            compiled[:window_bytes],
            case_dir / "compiled_prologue.bin",
        )
        object_disasm = _objdump_symbol(o_path, sym_name)
        (case_dir / "compiled_prologue.disasm").write_text(compiled_disasm)
        (case_dir / "compiled_full.bin").write_bytes(compiled)
        (case_dir / "compiled_object_symbol.disasm").write_text(object_disasm)

    report = []
    report.append(f"# Read Prologue Harness: {addr}\n")
    report.append(f"- Function: `{func['name']}`")
    report.append(f"- Status: `{func.get('match_status')}`")
    report.append(f"- Size: `{func.get('size')}`")
    report.append(f"- Source: `{src_file or '(none)'}`")
    report.append(f"- Object: `{o_path or '(not compiled)'}`")
    report.append(f"- Symbol: `{sym_name or func.get('mangled_symbol') or '(unknown)'}`")
    if relocs:
        report.append(f"- Function-relative relocations: `{relocs}`")
    report.append("")
    if compiled:
        report.append("## Raw Word Diff")
        report.append("")
        report.extend(f"    {line}" for line in _word_diff(expected, compiled, window_bytes))
        report.append("")
        report.append("## Verification-Masked Word Diff")
        report.append("")
        report.append(
            "Uses the compiled object's relocation offsets, matching verifier behavior. "
            "When a call is scheduled at a different offset, the raw diff is the clearer "
            "instruction-order view."
        )
        report.append("")
        report.extend(
            f"    {line}"
            for line in _word_diff(masked_expected, masked_compiled, window_bytes)
        )
        report.append("")
    report.append("## Expected Prologue")
    report.append("")
    report.append("```")
    report.append(expected_disasm.strip())
    report.append("```")
    report.append("")
    report.append("## Compiled Prologue")
    report.append("")
    report.append("```")
    report.append(compiled_disasm.strip())
    report.append("```")
    report.append("")
    report.append("## Compiled Object Symbol")
    report.append("")
    report.append("```")
    report.append(object_disasm.strip())
    report.append("```")
    report.append("")

    report_path = case_dir / "report.md"
    report_path.write_text("\n".join(report))
    return report_path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addresses", nargs="*", default=DEFAULT_ADDRS,
                        help="Function addresses to analyze.")
    parser.add_argument("--source", action="append", default=[],
                        help="Source override as ADDR=PATH. Can be repeated.")
    parser.add_argument("--out-dir", default=str(DEFAULT_OUT),
                        help=f"Artifact directory (default: {DEFAULT_OUT}).")
    parser.add_argument("--window-bytes", type=lambda s: int(s, 0), default=0x60,
                        help="Initial byte window to disassemble/diff.")
    parser.add_argument("--no-compile", action="store_true",
                        help="Only extract/disassemble original EBOOT bytes.")
    args = parser.parse_args()

    overrides = _source_override_map(args.source)
    out_dir = Path(args.out_dir)

    reports = []
    for addr in args.addresses:
        func = _find_func(addr)
        src_file = _infer_source(func, overrides)
        reports.append(analyze_address(
            func,
            src_file,
            out_dir,
            args.window_bytes,
            compile_source=not args.no_compile,
        ))

    print("Read prologue harness reports:")
    for path in reports:
        print(f"  {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
