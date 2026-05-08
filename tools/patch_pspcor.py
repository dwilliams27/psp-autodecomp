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
    characteristics: int
    header_offset: int

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
    pe_offset: int
    coff_offset: int
    optional_offset: int
    section_table_offset: int
    section_count: int
    section_alignment: int
    file_alignment: int
    size_of_image: int
    size_of_code: int
    sections: tuple[Section, ...]

    def va_to_file_offset(self, va: int) -> int:
        for section in self.sections:
            if section.contains_va(va - self.image_base):
                return section.va_to_file_offset(self.image_base, va)
        raise ValueError(f"VA 0x{va:08x} is outside mapped sections")

    def section(self, name: str) -> Section:
        for section in self.sections:
            if section.name == name:
                return section
        raise ValueError(f"section {name!r} not found")


@dataclass(frozen=True)
class BytePatch:
    name: str
    va: int
    before: bytes
    after: bytes
    note: str


READ_PROLOGUE_PATCHES: tuple[BytePatch, ...] = ()


def align_up(value: int, alignment: int) -> int:
    if alignment <= 0:
        raise ValueError(f"invalid alignment {alignment}")
    return (value + alignment - 1) // alignment * alignment


def rel32_jump(src_va: int, dst_va: int) -> bytes:
    rel = dst_va - (src_va + 5)
    if not -(1 << 31) <= rel < (1 << 31):
        raise ValueError(f"jmp from 0x{src_va:08x} to 0x{dst_va:08x} is out of range")
    return b"\xe9" + struct.pack("<i", rel)


def rel32_call(src_va: int, dst_va: int) -> bytes:
    rel = dst_va - (src_va + 5)
    if not -(1 << 31) <= rel < (1 << 31):
        raise ValueError(f"call from 0x{src_va:08x} to 0x{dst_va:08x} is out of range")
    return b"\xe8" + struct.pack("<i", rel)


def next_section_va(pe: PeInfo) -> int:
    last_va_end = max(
        section.virtual_address + align_up(
            max(section.virtual_size, section.raw_size),
            pe.section_alignment,
        )
        for section in pe.sections
    )
    return pe.image_base + align_up(last_va_end, pe.section_alignment)


def add_section(
    exe: Path,
    name: str,
    contents: bytes,
    characteristics: int = 0x60000020,
) -> int:
    """Append a PE section and return its VA.

    pspcor.exe has `.rdata` immediately after `.text`, so adding a new section
    is safer than growing `.text` and shifting existing sections.
    """
    if not contents:
        raise ValueError("cannot add an empty section")
    raw_name = name.encode("ascii")
    if len(raw_name) > 8:
        raise ValueError("PE section names are limited to 8 bytes")

    data = bytearray(exe.read_bytes())
    pe = read_pe_info(exe)
    first_raw = min(section.raw_pointer for section in pe.sections if section.raw_pointer)
    new_header = pe.section_table_offset + pe.section_count * 40
    if new_header + 40 > first_raw:
        raise RuntimeError("no room for another PE section header")

    new_rva = next_section_va(pe) - pe.image_base
    new_raw = align_up(len(data), pe.file_alignment)
    raw_size = align_up(len(contents), pe.file_alignment)

    if len(data) < new_raw:
        data.extend(b"\0" * (new_raw - len(data)))
    data.extend(contents)
    data.extend(b"\0" * (raw_size - len(contents)))

    header = bytearray(40)
    header[:len(raw_name)] = raw_name
    struct.pack_into(
        "<IIIIIIHHI",
        header,
        8,
        len(contents),
        new_rva,
        raw_size,
        new_raw,
        0,
        0,
        0,
        0,
        characteristics,
    )
    data[new_header:new_header + 40] = header

    struct.pack_into("<H", data, pe.coff_offset + 2, pe.section_count + 1)
    struct.pack_into(
        "<I",
        data,
        pe.optional_offset + 56,
        align_up(new_rva + len(contents), pe.section_alignment),
    )
    if characteristics & 0x20:
        struct.pack_into("<I", data, pe.optional_offset + 4, pe.size_of_code + raw_size)

    exe.write_bytes(data)
    return pe.image_base + new_rva


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
    size_of_code = struct.unpack_from("<I", data, optional + 4)[0]
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    section_alignment = struct.unpack_from("<I", data, optional + 32)[0]
    file_alignment = struct.unpack_from("<I", data, optional + 36)[0]
    size_of_image = struct.unpack_from("<I", data, optional + 56)[0]

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
        characteristics = struct.unpack_from("<I", data, off + 36)[0]
        sections.append(Section(
            name=name,
            virtual_size=virtual_size,
            virtual_address=virtual_address,
            raw_size=raw_size,
            raw_pointer=raw_pointer,
            characteristics=characteristics,
            header_offset=off,
        ))

    return PeInfo(
        image_base=image_base,
        pe_offset=pe_off,
        coff_offset=coff,
        optional_offset=optional,
        section_table_offset=section_table,
        section_count=section_count,
        section_alignment=section_alignment,
        file_alignment=file_alignment,
        size_of_image=size_of_image,
        size_of_code=size_of_code,
        sections=tuple(sections),
    )


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
    replace_bytes(exe, pe, patch.va, patch.before, patch.after, patch.name)


def replace_bytes(exe: Path, pe: PeInfo, va: int, before: bytes, after: bytes,
                  name: str) -> None:
    data = bytearray(exe.read_bytes())
    off = pe.va_to_file_offset(va)
    actual = bytes(data[off:off + len(before)])
    if actual == after:
        return
    if actual != before:
        raise RuntimeError(
            f"{name}: expected {before.hex()} at VA 0x{va:08x}, "
            f"found {actual.hex()}"
        )
    data[off:off + len(after)] = after
    exe.write_bytes(data)


def install_cgemit_noop_hook(exe: Path) -> dict:
    """Install a behavior-preserving trampoline at the CG_Emit prologue window."""
    pe = read_pe_info(exe)
    hook_va = 0x004175ed
    resume_va = 0x004175f3
    original = bytes.fromhex("53 e8 7d eb ff ff")
    stub_va = next_section_va(pe)
    stub = (
        b"\x53"
        + rel32_call(stub_va + 1, 0x00416170)
        + rel32_jump(stub_va + 6, resume_va)
    )
    actual_stub_va = add_section(exe, ".rphook", stub)
    if actual_stub_va != stub_va:
        raise RuntimeError(
            f"computed stub VA 0x{stub_va:08x}, got 0x{actual_stub_va:08x}"
        )
    pe = read_pe_info(exe)
    replace_bytes(
        exe,
        pe,
        hook_va,
        original,
        rel32_jump(hook_va, stub_va) + b"\x90",
        "cgemit-noop-hook",
    )
    return {
        "name": "cgemit-noop-hook",
        "hook_va": f"0x{hook_va:08x}",
        "stub_va": f"0x{stub_va:08x}",
        "resume_va": f"0x{resume_va:08x}",
        "note": "Behavior-preserving CG_Emit trampoline validation hook.",
    }


def prepare_read_prologue(source_dir: Path, output_dir: Path,
                          apply_functional_patch: bool,
                          install_noop_hook: bool) -> dict:
    copy_toolchain(source_dir, output_dir)
    exe = output_dir / PSPCOR
    pe = read_pe_info(exe)

    applied: list[dict] = []
    functional_count = 0
    if apply_functional_patch:
        for patch in READ_PROLOGUE_PATCHES:
            apply_patch(exe, pe, patch)
            functional_count += 1
            applied.append({
                "name": patch.name,
                "va": f"0x{patch.va:08x}",
                "before": patch.before.hex(),
                "after": patch.after.hex(),
                "note": patch.note,
            })
    if install_noop_hook:
        applied.append(install_cgemit_noop_hook(exe))

    manifest = {
        "source_dir": str(source_dir),
        "output_dir": str(output_dir),
        "pspcor": str(exe),
        "image_base": f"0x{pe.image_base:08x}",
        "patches_applied": applied,
        "status": (
            "functional patches applied"
            if functional_count else
            "validation hook applied; no functional Read-prologue patch registered yet"
            if install_noop_hook else
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
    if args.stock_copy and args.noop_cgemit_hook:
        raise SystemExit("--stock-copy and --noop-cgemit-hook are mutually exclusive")
    manifest = prepare_read_prologue(
        Path(args.source_dir),
        Path(args.output_dir),
        apply_functional_patch=not args.stock_copy,
        install_noop_hook=args.noop_cgemit_hook,
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
    prep.add_argument(
        "--noop-cgemit-hook",
        action="store_true",
        help="Install a behavior-preserving CG_Emit trampoline for validation.",
    )
    prep.set_defaults(func=cmd_prepare_read_prologue)

    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
