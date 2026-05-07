#!/usr/bin/env python3
"""Prepare opt-in patched SNC toolchain copies.

This tool never modifies `extern/snc` in place. It copies the SNC directory to
an ignored sibling directory and applies pspcor.exe byte patches there. The
actual Read-prologue patch bytes are intentionally not registered until the
CG_Emit patch point is validated.
"""

from __future__ import annotations

import argparse
import json
import shutil
import struct
from dataclasses import dataclass
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE_DIR = REPO_ROOT / "extern/snc"
DEFAULT_OUTPUT_DIR = REPO_ROOT / "extern/snc-read-prologue"
PSPCOR = "pspcor.exe"


@dataclass(frozen=True)
class Section:
    name: str
    virtual_size: int
    virtual_address: int
    raw_size: int
    raw_pointer: int

    def contains_va(self, va: int) -> bool:
        size = max(self.virtual_size, self.raw_size)
        return self.virtual_address <= va < self.virtual_address + size

    def va_to_file_offset(self, image_base: int, va: int) -> int:
        rva = va - image_base
        if not self.contains_va(rva):
            raise ValueError(f"VA 0x{va:08x} is outside section {self.name}")
        return self.raw_pointer + (rva - self.virtual_address)


@dataclass(frozen=True)
class PeInfo:
    image_base: int
    sections: tuple[Section, ...]

    def va_to_file_offset(self, va: int) -> int:
        for section in self.sections:
            if section.contains_va(va - self.image_base):
                return section.va_to_file_offset(self.image_base, va)
        raise ValueError(f"VA 0x{va:08x} is outside mapped sections")


@dataclass(frozen=True)
class BytePatch:
    name: str
    va: int
    before: bytes
    after: bytes
    note: str


READ_PROLOGUE_PATCHES: tuple[BytePatch, ...] = ()


def read_pe_info(exe: Path) -> PeInfo:
    data = exe.read_bytes()
    if data[:2] != b"MZ":
        raise ValueError(f"{exe} is not a PE/MZ executable")
    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_off:pe_off + 4] != b"PE\0\0":
        raise ValueError(f"{exe} does not have a PE signature")

    coff = pe_off + 4
    section_count = struct.unpack_from("<H", data, coff + 2)[0]
    optional_size = struct.unpack_from("<H", data, coff + 16)[0]
    optional = coff + 20
    magic = struct.unpack_from("<H", data, optional)[0]
    if magic != 0x10B:
        raise ValueError(f"expected PE32 optional header, got 0x{magic:04x}")
    image_base = struct.unpack_from("<I", data, optional + 28)[0]

    section_table = optional + optional_size
    sections: list[Section] = []
    for index in range(section_count):
        off = section_table + index * 40
        raw_name = data[off:off + 8].split(b"\0", 1)[0]
        name = raw_name.decode("ascii", errors="replace")
        virtual_size, virtual_address, raw_size, raw_pointer = struct.unpack_from(
            "<IIII",
            data,
            off + 8,
        )
        sections.append(Section(
            name=name,
            virtual_size=virtual_size,
            virtual_address=virtual_address,
            raw_size=raw_size,
            raw_pointer=raw_pointer,
        ))

    return PeInfo(image_base=image_base, sections=tuple(sections))


def copy_toolchain(source_dir: Path, output_dir: Path) -> None:
    if not source_dir.exists():
        raise FileNotFoundError(source_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    for src in source_dir.iterdir():
        if src.is_dir():
            continue
        dst = output_dir / src.name
        shutil.copy2(src, dst)


def apply_patch(exe: Path, pe: PeInfo, patch: BytePatch) -> None:
    data = bytearray(exe.read_bytes())
    off = pe.va_to_file_offset(patch.va)
    actual = bytes(data[off:off + len(patch.before)])
    if actual == patch.after:
        return
    if actual != patch.before:
        raise RuntimeError(
            f"{patch.name}: expected {patch.before.hex()} at VA 0x{patch.va:08x}, "
            f"found {actual.hex()}"
        )
    data[off:off + len(patch.after)] = patch.after
    exe.write_bytes(data)


def prepare_read_prologue(source_dir: Path, output_dir: Path,
                          apply_functional_patch: bool) -> dict:
    copy_toolchain(source_dir, output_dir)
    exe = output_dir / PSPCOR
    pe = read_pe_info(exe)

    applied: list[dict] = []
    if apply_functional_patch:
        for patch in READ_PROLOGUE_PATCHES:
            apply_patch(exe, pe, patch)
            applied.append({
                "name": patch.name,
                "va": f"0x{patch.va:08x}",
                "before": patch.before.hex(),
                "after": patch.after.hex(),
                "note": patch.note,
            })

    manifest = {
        "source_dir": str(source_dir),
        "output_dir": str(output_dir),
        "pspcor": str(exe),
        "image_base": f"0x{pe.image_base:08x}",
        "patches_applied": applied,
        "status": (
            "functional patches applied"
            if applied else
            "stock copy; no Read-prologue patch registered yet"
        ),
    }
    (output_dir / "pspcor-read-prologue-manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n"
    )
    return manifest


def cmd_inspect(args: argparse.Namespace) -> int:
    pe = read_pe_info(Path(args.exe))
    print(f"ImageBase: 0x{pe.image_base:08x}")
    for section in pe.sections:
        print(
            f"{section.name:8} "
            f"VA=0x{pe.image_base + section.virtual_address:08x} "
            f"VSZ=0x{section.virtual_size:x} "
            f"RAW=0x{section.raw_pointer:x} "
            f"RSZ=0x{section.raw_size:x}"
        )
    return 0


def cmd_prepare_read_prologue(args: argparse.Namespace) -> int:
    manifest = prepare_read_prologue(
        Path(args.source_dir),
        Path(args.output_dir),
        apply_functional_patch=not args.stock_copy,
    )
    print(f"Prepared: {manifest['output_dir']}")
    print(f"Status: {manifest['status']}")
    print("Use with: make USE_READ_PROLOGUE_PSPCOR=1 <target>")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)

    inspect = sub.add_parser("inspect", help="Print pspcor.exe PE section mapping.")
    inspect.add_argument("exe", nargs="?", default=str(DEFAULT_SOURCE_DIR / PSPCOR))
    inspect.set_defaults(func=cmd_inspect)

    prep = sub.add_parser(
        "prepare-read-prologue",
        help="Create extern/snc-read-prologue from extern/snc.",
    )
    prep.add_argument("--source-dir", default=str(DEFAULT_SOURCE_DIR))
    prep.add_argument("--output-dir", default=str(DEFAULT_OUTPUT_DIR))
    prep.add_argument(
        "--stock-copy",
        action="store_true",
        help="Only copy the toolchain; do not apply registered patches.",
    )
    prep.set_defaults(func=cmd_prepare_read_prologue)

    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
