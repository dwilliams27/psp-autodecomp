#!/usr/bin/env python3
"""Generate and test source-level cFactory::Read prologue variants.

This keeps the real `src/cFactory.cpp` untouched while recording source-shape
experiments that might otherwise be lost as manual edits.
"""

from __future__ import annotations

import argparse
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path


TARGET_ADDR = "0x0000ab98"
BASE_SOURCE = Path("src/cFactory.cpp")
DEFAULT_OUT = Path("build/research/read_prologue/source_variants")


@dataclass(frozen=True)
class Variant:
    name: str
    body: str
    prefix: str = ""


READ_TAIL = """\
    if ((unsigned int)rb._data[3] == 1 && this->cObject::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
"""


VARIANTS = [
    Variant(
        "current",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
""" + READ_TAIL,
    ),
    Variant(
        "asm_before_ctor",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
""" + READ_TAIL,
    ),
    Variant(
        "asm_after_ctor_memory",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result) : : "memory");
""" + READ_TAIL,
    ),
    Variant(
        "asm_before_ctor_memory",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result) : : "memory");
    cReadBlock rb(file, 1, true);
""" + READ_TAIL,
    ),
    Variant(
        "plain_before_ctor",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
""" + READ_TAIL,
    ),
    Variant(
        "plain_after_ctor",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result = 1;
""" + READ_TAIL,
    ),
    Variant(
        "volatile_before_ctor",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    volatile int result = 1;
    cReadBlock rb(file, 1, true);
""" + READ_TAIL,
    ),
    Variant(
        "current_sched1",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
""" + READ_TAIL,
        prefix="#pragma control sched=1\n",
    ),
    Variant(
        "asm_before_ctor_sched1",
        """\
int cFactory::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
""" + READ_TAIL,
        prefix="#pragma control sched=1\n",
    ),
]


def _replace_read(source: str, variant: Variant) -> str:
    start = source.index("// \u2500\u2500 cFactory::Read")
    replacement = (
        "// cFactory::Read(cFile &, cMemPool *) -- generated research variant\n"
        f"// Variant: {variant.name}\n"
        f"{variant.prefix}"
        f"{variant.body}\n\n"
    )
    return source[:start] + replacement


def _diff_summary(report: Path) -> tuple[str, str, str]:
    text = report.read_text()
    compiled_size = re.search(r"Compiled size: `([^`]+)`", text)
    raw = re.search(r"Raw byte diffs: `([^`]+)`", text)
    masked = re.search(r"Verification-masked byte diffs: `([^`]+)`", text)
    return (
        compiled_size.group(1) if compiled_size else "unknown",
        raw.group(1) if raw else "unknown",
        masked.group(1) if masked else "unknown",
    )


def _run_variant(variant: Variant, source_path: Path, out_dir: Path) -> tuple[str, str, str, Path]:
    cmd = [
        "python3",
        "tools/research/read_prologue_harness.py",
        TARGET_ADDR,
        "--source",
        f"{TARGET_ADDR}={source_path}",
        "--out-dir",
        str(out_dir),
        "--variant",
        variant.name,
        "--direct-compile",
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    report = out_dir / TARGET_ADDR[2:] / variant.name / "report.md"
    if result.returncode != 0:
        failure = (result.stderr or result.stdout or "").strip().replace("\n", " ")
        return "compile_failed", "compile_failed", failure[:160], report
    compiled_size, raw, masked = _diff_summary(report)
    return compiled_size, raw, masked, report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out-dir", default=str(DEFAULT_OUT),
                        help=f"Output directory (default: {DEFAULT_OUT}).")
    parser.add_argument("--keep-going", action="store_true",
                        help="Continue after compile failures.")
    args = parser.parse_args()

    out_dir = Path(args.out_dir)
    src_dir = out_dir / "src"
    src_dir.mkdir(parents=True, exist_ok=True)

    base = BASE_SOURCE.read_text()
    rows = []
    for variant in VARIANTS:
        source_path = src_dir / f"cFactory_{variant.name}.cpp"
        source_path.write_text(_replace_read(base, variant))
        compiled_size, raw, masked, report = _run_variant(variant, source_path, out_dir)
        rows.append((variant.name, compiled_size, raw, masked, report))
        if compiled_size == "compile_failed" and not args.keep_going:
            break

    summary = ["# cFactory Read Source Variant Sweep", ""]
    summary.append("| Variant | Compiled Size | Raw Diffs | Masked Diffs | Report |")
    summary.append("|---|---:|---:|---:|---|")
    for name, compiled_size, raw, masked, report in rows:
        summary.append(f"| `{name}` | `{compiled_size}` | `{raw}` | `{masked}` | `{report}` |")
    (out_dir / "summary.md").write_text("\n".join(summary) + "\n")

    print("cFactory::Read source variant sweep:")
    for name, compiled_size, raw, masked, report in rows:
        print(
            f"  {name:24s} size={compiled_size:>5s} "
            f"raw={raw:>10s} masked={masked:>10s} report={report}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
