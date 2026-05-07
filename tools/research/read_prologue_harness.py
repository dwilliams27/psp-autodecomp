#!/usr/bin/env python3
"""Reproduce cReadBlock Read prologue scheduling differences.

This is the first ML2 harness: it extracts original EBOOT bytes for selected
`Read(cFile &, cMemPool *)` functions, compiles the current source candidate,
and writes side-by-side prologue disassembly artifacts.
"""

from __future__ import annotations

import argparse
import re
import shlex
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from byte_match import CompileFailed, compile_src, symbols_with_bytes_and_relocs  # noqa: E402
from common import EBOOT_PATH, OBJDUMP, TEXT_FILE_OFFSET, load_db, mask_relocation_bytes  # noqa: E402


DEFAULT_ADDRS = ["0x0005dccc", "0x0000ab98"]
DEFAULT_OUT = Path("build/research/read_prologue")
TRACE_SWEEP_VARIANTS = [
    ("trace-tr1", ["-keeptemp", "-tr1"]),
    ("trace-tr2", ["-keeptemp", "-tr2"]),
    ("trace-tr3", ["-keeptemp", "-tr3"]),
    ("trace-tr4", ["-keeptemp", "-tr4"]),
    ("trace-tr5", ["-keeptemp", "-tr5"]),
    ("trace-tr6", ["-keeptemp", "-tr6"]),
    ("trace-tr7", ["-keeptemp", "-tr7"]),
    ("trace-tr8", ["-keeptemp", "-tr8"]),
    ("trace-tr9", ["-keeptemp", "-tr9"]),
    ("trace-tr10", ["-keeptemp", "-tr10"]),
    ("trace-tr11", ["-keeptemp", "-tr11"]),
    ("trace-tt10-1", ["-keeptemp", "-tt10,1"]),
    ("trace-tt10-511", ["-keeptemp", "-tt10,511"]),
    ("trace-tt14-1", ["-keeptemp", "-tt14,1"]),
    ("trace-tt15-1", ["-keeptemp", "-tt15,1"]),
    ("trace-tt20-1", ["-keeptemp", "-tt20,1"]),
    ("trace-tt25-1", ["-keeptemp", "-tt25,1"]),
    ("trace-tt26-1", ["-keeptemp", "-tt26,1"]),
    ("trace-tt26-511", ["-keeptemp", "-tt26,511"]),
    ("sched1-direct", ["-Xsched=1"]),
    ("sched0-direct", ["-Xsched=0"]),
]
REPO_ROOT = Path(__file__).resolve().parents[2]
WIBO = REPO_ROOT / "extern/wibo"
SNC = REPO_ROOT / "extern/snc/pspsnc.exe"
DIRECT_CFLAGS = [
    "-c",
    "-O2",
    "-G0",
    "-Xsched=2",
    "-Xvfpumatrix=1",
    "-Xvfpuscalar=8",
    f"-I{REPO_ROOT / 'extern/include'}",
    f"-I{REPO_ROOT / 'include'}",
]


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


def _clean_direct_outputs(case_dir: Path) -> None:
    for name in (
        "compiled.o",
        "compile_command.txt",
        "compile_stdout.txt",
        "compile_stderr.txt",
        "trace_summary.md",
    ):
        path = case_dir / name
        if path.exists():
            path.unlink()
    for pattern in ("SNC*_0.B", "SNC*_0.I", "SNC*_0.D", "SNC*_0.s"):
        for path in case_dir.glob(pattern):
            path.unlink()


def _direct_compile_source(src_file: str, case_dir: Path,
                           extra_flags: list[str]) -> tuple[str, list[str]]:
    case_dir = case_dir.resolve()
    src_path = Path(src_file)
    if not src_path.is_absolute():
        src_path = REPO_ROOT / src_path
    src_path = src_path.resolve()

    o_path = case_dir / "compiled.o"
    _clean_direct_outputs(case_dir)
    cmd = [
        str(WIBO),
        str(SNC),
        *DIRECT_CFLAGS,
        *extra_flags,
        "-o",
        "compiled.o",
        str(src_path),
    ]
    result = subprocess.run(
        cmd,
        cwd=case_dir,
        capture_output=True,
        text=True,
    )
    (case_dir / "compile_command.txt").write_text(shlex.join(cmd) + "\n")
    (case_dir / "compile_stdout.txt").write_text(result.stdout)
    (case_dir / "compile_stderr.txt").write_text(result.stderr)
    if result.returncode != 0:
        detail = (result.stderr or result.stdout or "").strip()
        raise SystemExit(f"direct compile failed for {src_file}: {detail[:1000]}")
    if not o_path.exists():
        raise RuntimeError(f"direct compile succeeded but {o_path} not found")
    return str(o_path), cmd


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


def _byte_diff_count(left: bytes, right: bytes) -> int:
    return sum(1 for a, b in zip(left, right) if a != b) + abs(len(left) - len(right))


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


def _line_hits(lines: list[str], needle: str) -> list[int]:
    return [i + 1 for i, line in enumerate(lines) if needle in line]


def _write_trace_summary(case_dir: Path, func: dict, sym_name: str | None) -> None:
    stdout_path = case_dir / "compile_stdout.txt"
    stderr_path = case_dir / "compile_stderr.txt"
    if not stdout_path.exists() and not stderr_path.exists():
        return

    stdout_lines = stdout_path.read_text(errors="replace").splitlines()
    stderr_lines = stderr_path.read_text(errors="replace").splitlines() if stderr_path.exists() else []
    temp_files = sorted(path.name for path in case_dir.glob("SNC*_0.*"))

    summary = [
        f"# Trace Summary: {func['address']}",
        "",
        f"- Function: `{func['name']}`",
        f"- Symbol: `{sym_name or func.get('mangled_symbol') or '(unknown)'}`",
        f"- stdout lines: `{len(stdout_lines)}`",
        f"- stderr lines: `{len(stderr_lines)}`",
        f"- kept temp files: `{temp_files}`",
        "",
        "## Marker Lines",
        "",
    ]

    markers = [
        ("symbol", sym_name or func.get("mangled_symbol") or ""),
        ("ReadBlock ctor", "__0oKcReadBlockctR6FcFileUib"),
        ("ACIR ReadBlock call", 'OP_CALL ("__0oKcReadBlockctR6FcFileUib")'),
        ("inline asm machine op", "OP_MACHINE"),
        ("inline asm text", ".i4.asm.gnu"),
        ("optimizer entry", "Entrypoint:"),
        ("GRA summary", "<gra>"),
        ("CG expansion", "<cgex>"),
        ("LRA block", "Local Register Allocation for BB:"),
        ("CFG dump", "CONTROL FLOW GRAPH"),
    ]
    for label, needle in markers:
        if not needle:
            continue
        hits = _line_hits(stdout_lines, needle)
        preview = ", ".join(str(n) for n in hits[:12])
        suffix = " ..." if len(hits) > 12 else ""
        summary.append(f"- {label}: `{preview or '(none)'}{suffix}`")

    if stdout_lines:
        summary.extend(["", "## Stdout Head", "", "```"])
        summary.extend(stdout_lines[:40])
        summary.append("```")

    if stderr_lines:
        summary.extend(["", "## Stderr Head", "", "```"])
        summary.extend(stderr_lines[:40])
        summary.append("```")

    (case_dir / "trace_summary.md").write_text("\n".join(summary) + "\n")


def analyze_address(func: dict, src_file: str | None, out_dir: Path,
                    window_bytes: int, compile_source: bool,
                    direct_compile: bool, extra_flags: list[str],
                    variant: str | None) -> Path:
    addr = func["address"]
    safe = addr[2:]
    case_dir = out_dir / safe
    if variant:
        case_dir = case_dir / variant
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
    compile_cmd: list[str] = []
    compiled_disasm = "(not compiled)\n"
    object_disasm = "(not compiled)\n"
    if compile_source:
        if not src_file:
            raise SystemExit(f"no source candidate found for {addr} {func['name']}")
        if direct_compile:
            o_path, compile_cmd = _direct_compile_source(src_file, case_dir, extra_flags)
        else:
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
        if direct_compile:
            _write_trace_summary(case_dir, func, sym_name)

    report = []
    report.append(f"# Read Prologue Harness: {addr}\n")
    report.append(f"- Function: `{func['name']}`")
    report.append(f"- Status: `{func.get('match_status')}`")
    report.append(f"- Size: `{func.get('size')}`")
    report.append(f"- Source: `{src_file or '(none)'}`")
    report.append(f"- Object: `{o_path or '(not compiled)'}`")
    report.append(f"- Symbol: `{sym_name or func.get('mangled_symbol') or '(unknown)'}`")
    if compiled:
        report.append(f"- Compiled size: `{len(compiled)}`")
    report.append(f"- Compile mode: `{'direct' if direct_compile else 'make'}`")
    if compile_cmd:
        report.append(f"- Compiler command: `{shlex.join(compile_cmd)}`")
        report.append("- Compiler stdout: `compile_stdout.txt`")
        report.append("- Compiler stderr: `compile_stderr.txt`")
    if relocs:
        report.append(f"- Function-relative relocations: `{relocs}`")
    if compiled:
        report.append(
            f"- Raw byte diffs: `{_byte_diff_count(expected, compiled)}/{len(expected)}`"
        )
        report.append(
            f"- Verification-masked byte diffs: "
            f"`{_byte_diff_count(masked_expected, masked_compiled)}/{len(expected)}`"
        )
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
    parser.add_argument("--direct-compile", action="store_true",
                        help="Compile directly through SNC instead of make.")
    parser.add_argument("--extra-flag", action="append", default=[],
                        help="Extra SNC flag for direct compilation. Can be repeated.")
    parser.add_argument("--variant",
                        help="Subdirectory name under each address for artifacts.")
    parser.add_argument("--trace-sweep", action="store_true",
                        help="Run the standard direct-compile trace/scheduler variant sweep.")
    args = parser.parse_args()

    overrides = _source_override_map(args.source)
    out_dir = Path(args.out_dir)
    direct_compile = args.direct_compile or bool(args.extra_flag)
    variant = args.variant
    if direct_compile and not variant:
        variant = "direct"

    reports = []
    for addr in args.addresses:
        func = _find_func(addr)
        src_file = _infer_source(func, overrides)
        if args.trace_sweep:
            if args.no_compile:
                raise SystemExit("--trace-sweep requires compilation")
            for sweep_variant, sweep_flags in TRACE_SWEEP_VARIANTS:
                reports.append(analyze_address(
                    func,
                    src_file,
                    out_dir,
                    args.window_bytes,
                    compile_source=True,
                    direct_compile=True,
                    extra_flags=sweep_flags,
                    variant=sweep_variant,
                ))
        else:
            reports.append(analyze_address(
                func,
                src_file,
                out_dir,
                args.window_bytes,
                compile_source=not args.no_compile,
                direct_compile=direct_compile,
                extra_flags=args.extra_flag,
                variant=variant,
            ))

    print("Read prologue harness reports:")
    for path in reports:
        print(f"  {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
