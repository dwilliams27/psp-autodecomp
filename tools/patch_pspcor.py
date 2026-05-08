#!/usr/bin/env python3
"""Prepare opt-in patched SNC toolchain copies.

This tool never modifies `extern/snc` in place. It copies the SNC directory to
an ignored sibling directory and applies pspcor.exe byte patches there. The
functional Read-prologue transform is installed only when explicitly requested.
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


class X86Builder:
    """Small x86 encoder for pspcor trampoline stubs.

    This intentionally supports only the instruction forms used below. Keeping
    the encoder local makes the patch reproducible without requiring nasm/yasm.
    """

    EAX = 0
    ECX = 1
    EDX = 2
    EBX = 3
    ESP = 4
    EBP = 5
    ESI = 6
    EDI = 7

    def __init__(self, base_va: int) -> None:
        self.base_va = base_va
        self.code = bytearray()
        self.labels: dict[str, int] = {}
        self.rel_fixups: list[tuple[int, str]] = []
        self.abs_fixups: list[tuple[int, str]] = []

    @property
    def offset(self) -> int:
        return len(self.code)

    def va(self, offset: int | None = None) -> int:
        return self.base_va + (self.offset if offset is None else offset)

    def label(self, name: str) -> None:
        if name in self.labels:
            raise ValueError(f"duplicate label {name}")
        self.labels[name] = self.offset

    def emit(self, data: bytes) -> None:
        self.code.extend(data)

    def emit_u8(self, value: int) -> None:
        self.code.append(value & 0xff)

    def emit_i8(self, value: int) -> None:
        self.code.extend(struct.pack("<b", value))

    def emit_u32(self, value: int) -> None:
        self.code.extend(struct.pack("<I", value & 0xffffffff))

    def modrm(self, mod: int, reg: int, rm: int) -> int:
        return ((mod & 0x3) << 6) | ((reg & 0x7) << 3) | (rm & 0x7)

    def _mem_modrm(self, reg_field: int, base: int, disp: int = 0) -> bytes:
        if base == self.ESP:
            if -128 <= disp <= 127:
                return bytes([self.modrm(1, reg_field, 4), 0x24]) + struct.pack("<b", disp)
            return bytes([self.modrm(2, reg_field, 4), 0x24]) + struct.pack("<i", disp)
        if disp == 0 and base != self.EBP:
            return bytes([self.modrm(0, reg_field, base)])
        if -128 <= disp <= 127:
            return bytes([self.modrm(1, reg_field, base)]) + struct.pack("<b", disp)
        return bytes([self.modrm(2, reg_field, base)]) + struct.pack("<i", disp)

    def add_esp(self, value: int) -> None:
        if -128 <= value <= 127:
            self.emit(bytes([0x83, 0xc4]))
            self.emit_i8(value)
        else:
            self.emit(bytes([0x81, 0xc4]))
            self.emit_u32(value)

    def sub_esp(self, value: int) -> None:
        if 0 <= value <= 127:
            self.emit(bytes([0x83, 0xec, value]))
        else:
            self.emit(bytes([0x81, 0xec]))
            self.emit_u32(value)

    def push_reg(self, reg: int) -> None:
        self.emit_u8(0x50 + reg)

    def pop_reg(self, reg: int) -> None:
        self.emit_u8(0x58 + reg)

    def push_imm32(self, value: int) -> None:
        self.emit_u8(0x68)
        self.emit_u32(value)

    def push_label_abs(self, label: str) -> None:
        self.emit_u8(0x68)
        self.abs_fixups.append((self.offset, label))
        self.emit_u32(0)

    def push_abs_mem(self, va: int) -> None:
        self.emit(bytes([0xff, 0x35]))
        self.emit_u32(va)

    def pushad(self) -> None:
        self.emit_u8(0x60)

    def popad(self) -> None:
        self.emit_u8(0x61)

    def ret(self) -> None:
        self.emit_u8(0xc3)

    def mov_reg_reg(self, dst: int, src: int) -> None:
        self.emit(bytes([0x8b, self.modrm(3, dst, src)]))

    def mov_reg_imm32(self, reg: int, value: int) -> None:
        self.emit_u8(0xb8 + reg)
        self.emit_u32(value)

    def mov_reg_label_abs(self, reg: int, label: str) -> None:
        self.emit_u8(0xb8 + reg)
        self.abs_fixups.append((self.offset, label))
        self.emit_u32(0)

    def mov_reg_mem(self, dst: int, base: int, disp: int = 0) -> None:
        self.emit_u8(0x8b)
        self.emit(self._mem_modrm(dst, base, disp))

    def mov_mem_reg(self, base: int, disp: int, src: int) -> None:
        self.emit_u8(0x89)
        self.emit(self._mem_modrm(src, base, disp))

    def mov_mem_imm32(self, base: int, disp: int, value: int) -> None:
        self.emit_u8(0xc7)
        self.emit(self._mem_modrm(0, base, disp))
        self.emit_u32(value)

    def xor_reg_reg(self, dst: int, src: int) -> None:
        self.emit(bytes([0x31, self.modrm(3, src, dst)]))

    def test_reg_reg(self, left: int, right: int) -> None:
        self.emit(bytes([0x85, self.modrm(3, right, left)]))

    def inc_reg(self, reg: int) -> None:
        self.emit_u8(0x40 + reg)

    def cmp_mem8_imm8(self, base: int, disp: int, value: int) -> None:
        self.emit_u8(0x80)
        self.emit(self._mem_modrm(7, base, disp))
        self.emit_u8(value)

    def call_abs(self, target_va: int) -> None:
        src_va = self.va()
        self.emit(rel32_call(src_va, target_va))

    def call_label(self, label: str) -> None:
        self.emit_u8(0xe8)
        self.rel_fixups.append((self.offset, label))
        self.emit_u32(0)

    def jmp_label(self, label: str) -> None:
        self.emit_u8(0xe9)
        self.rel_fixups.append((self.offset, label))
        self.emit_u32(0)

    def jz_label(self, label: str) -> None:
        self.emit(bytes([0x0f, 0x84]))
        self.rel_fixups.append((self.offset, label))
        self.emit_u32(0)

    def jnz_label(self, label: str) -> None:
        self.emit(bytes([0x0f, 0x85]))
        self.rel_fixups.append((self.offset, label))
        self.emit_u32(0)

    def align(self, alignment: int) -> None:
        while self.offset % alignment:
            self.emit_u8(0x90)

    def ascii_label(self, label: str, text: str) -> None:
        self.label(label)
        self.emit(text.encode("ascii") + b"\0")

    def finish(self) -> bytes:
        for off, label in self.rel_fixups:
            if label not in self.labels:
                raise ValueError(f"unknown label {label}")
            src_next = self.base_va + off + 4
            dst = self.base_va + self.labels[label]
            rel = dst - src_next
            struct.pack_into("<i", self.code, off, rel)
        for off, label in self.abs_fixups:
            if label not in self.labels:
                raise ValueError(f"unknown label {label}")
            struct.pack_into("<I", self.code, off, self.base_va + self.labels[label])
        return bytes(self.code)


def build_read_prologue_transform_stub(stub_va: int, debug_dump: bool) -> bytes:
    b = X86Builder(stub_va)

    # Hook replacement for the call at 0x418351. The caller has just loaded
    # EAX=ESI (the current basic block). Preserve original behavior, then try
    # the narrow Read prologue transform before the block is emitted.
    b.call_abs(0x004169ca)
    b.pushad()
    b.mov_reg_reg(b.EBP, b.ESI)
    b.call_label("transform")
    b.popad()
    b.ret()

    b.label("find_next")
    b.push_reg(b.EBX)
    b.push_reg(b.ECX)
    b.label("find_loop")
    b.test_reg_reg(b.ESI, b.ESI)
    b.jz_label("find_fail")
    b.push_reg(b.ESI)
    b.push_imm32(2)
    b.push_reg(b.ESI)
    b.call_abs(0x00416d72)
    b.add_esp(8)
    b.mov_reg_reg(b.EBX, b.EAX)
    b.label("find_trim")
    b.cmp_mem8_imm8(b.EBX, 0, 9)
    b.jz_label("find_trim_one")
    b.cmp_mem8_imm8(b.EBX, 0, 32)
    b.jnz_label("find_compare")
    b.label("find_trim_one")
    b.inc_reg(b.EBX)
    b.jmp_label("find_trim")
    b.label("find_compare")
    b.push_reg(b.EDI)
    b.push_reg(b.EBX)
    b.call_abs(0x004a3290)
    b.add_esp(8)
    b.pop_reg(b.ESI)
    b.test_reg_reg(b.EAX, b.EAX)
    b.jz_label("find_hit")
    b.mov_reg_mem(b.ESI, b.ESI, 0)
    b.jmp_label("find_loop")
    b.label("find_hit")
    b.mov_reg_reg(b.EAX, b.ESI)
    b.mov_reg_mem(b.ESI, b.ESI, 0)
    b.pop_reg(b.ECX)
    b.pop_reg(b.EBX)
    b.ret()
    b.label("find_fail")
    b.xor_reg_reg(b.EAX, b.EAX)
    b.pop_reg(b.ECX)
    b.pop_reg(b.EBX)
    b.ret()

    b.label("move_range_after")
    b.test_reg_reg(b.EBX, b.EBX)
    b.jz_label("move_done")
    b.test_reg_reg(b.EDX, b.EDX)
    b.jz_label("move_done")
    b.test_reg_reg(b.EDI, b.EDI)
    b.jz_label("move_done")
    b.mov_reg_mem(b.EAX, b.EBX, 4)      # before range
    b.mov_reg_mem(b.ECX, b.EDX, 0)      # after range
    b.test_reg_reg(b.EAX, b.EAX)
    b.jz_label("move_range_was_tail")
    b.mov_mem_reg(b.EAX, 0, b.ECX)
    b.jmp_label("move_after_before")
    b.label("move_range_was_tail")
    b.mov_mem_reg(b.EBP, 0x1c, b.ECX)
    b.label("move_after_before")
    b.test_reg_reg(b.ECX, b.ECX)
    b.jz_label("move_range_was_head")
    b.mov_mem_reg(b.ECX, 4, b.EAX)
    b.jmp_label("move_detached")
    b.label("move_range_was_head")
    b.mov_mem_reg(b.EBP, 0x20, b.EAX)
    b.label("move_detached")
    b.mov_reg_mem(b.ESI, b.EDI, 0)
    b.mov_mem_reg(b.EDI, 0, b.EBX)
    b.mov_mem_reg(b.EBX, 4, b.EDI)
    b.mov_mem_reg(b.EDX, 0, b.ESI)
    b.test_reg_reg(b.ESI, b.ESI)
    b.jz_label("move_inserted_at_head")
    b.mov_mem_reg(b.ESI, 4, b.EDX)
    b.ret()
    b.label("move_inserted_at_head")
    b.mov_mem_reg(b.EBP, 0x20, b.EDX)
    b.label("move_done")
    b.ret()

    b.label("debug_dump")
    b.push_reg(b.EBX)
    b.push_reg(b.ESI)
    b.mov_reg_mem(b.ESI, b.EBP, 0x1c)
    b.label("debug_loop")
    b.test_reg_reg(b.ESI, b.ESI)
    b.jz_label("debug_done")
    b.push_reg(b.ESI)
    b.push_imm32(2)
    b.push_reg(b.ESI)
    b.call_abs(0x00416d72)
    b.add_esp(8)
    b.mov_reg_reg(b.EBX, b.EAX)
    b.push_reg(b.EBX)
    b.push_label_abs("s_debug_fmt")
    b.push_abs_mem(0x004f84c0)
    b.call_abs(0x004a2620)
    b.add_esp(12)
    b.pop_reg(b.ESI)
    b.mov_reg_mem(b.ESI, b.ESI, 0)
    b.jmp_label("debug_loop")
    b.label("debug_done")
    b.pop_reg(b.ESI)
    b.pop_reg(b.EBX)
    b.ret()

    # Stack locals in transform:
    # +00 sw17, +04 or17, +08 or4, +0c or6, +10 sw19,
    # +14 asm_first, +18 asm_last.
    b.label("transform")
    b.sub_esp(0x1c)
    if debug_dump:
        b.call_label("debug_dump")
    b.mov_reg_mem(b.ESI, b.EBP, 0x1c)

    def find(label: str, slot: int | None = None) -> None:
        b.mov_reg_label_abs(b.EDI, label)
        b.call_label("find_next")
        b.test_reg_reg(b.EAX, b.EAX)
        b.jz_label("transform_done")
        if slot is not None:
            b.mov_mem_reg(b.ESP, slot, b.EAX)

    find("s_sw16")
    find("s_sw18")
    find("s_or16")
    find("s_or18")
    find("s_sw17", 0x00)
    find("s_or17", 0x04)
    find("s_or4", 0x08)
    find("s_or6", 0x0c)
    find("s_sw19", 0x10)
    find("s_sw31")
    find("s_jal_ctor")
    find("s_or7")
    find("s_asm_start", 0x14)
    find("s_asm_nop")
    find("s_asm_result")
    find("s_asm_nop")
    find("s_asm_end", 0x18)

    b.mov_reg_mem(b.EBX, b.ESP, 0x10)
    b.mov_reg_mem(b.EDX, b.ESP, 0x10)
    b.mov_reg_mem(b.EDI, b.ESP, 0x00)
    b.call_label("move_range_after")
    b.mov_reg_mem(b.EBX, b.ESP, 0x14)
    b.mov_reg_mem(b.EDX, b.ESP, 0x18)
    b.mov_reg_mem(b.EDI, b.ESP, 0x04)
    b.call_label("move_range_after")

    b.label("transform_done")
    b.add_esp(0x1c)
    b.ret()

    b.align(4)
    b.ascii_label("s_sw16", "sw\t$16,20($sp)")
    b.ascii_label("s_sw18", "sw\t$18,28($sp)")
    b.ascii_label("s_or16", "or\t$16,$6,$0")
    b.ascii_label("s_or18", "or\t$18,$4,$0")
    b.ascii_label("s_sw17", "sw\t$17,24($sp)")
    b.ascii_label("s_or17", "or\t$17,$5,$0")
    b.ascii_label("s_or4", "or\t$4,$sp,$0")
    b.ascii_label("s_or6", "or\t$6,$0,1")
    b.ascii_label("s_sw19", "sw\t$19,32($sp)")
    b.ascii_label("s_sw31", "sw\t$31,36($sp)")
    b.ascii_label("s_jal_ctor", "jal\t__0oKcReadBlockctR6FcFileUib")
    b.ascii_label("s_or7", "or\t$7,$0,1")
    b.ascii_label("s_asm_start", "#asm{")
    b.ascii_label("s_asm_nop", "#nop <- $,$")
    b.ascii_label("s_asm_result", "#nop <- $19,$0,$")
    b.ascii_label("s_asm_end", "#}asm")
    b.ascii_label("s_debug_fmt", "#RPHOOK:%s\n")
    return b.finish()


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


def install_read_prologue_transform_hook(exe: Path, debug_dump: bool) -> dict:
    """Install the first narrow final-list Read prologue transform hook."""
    pe = read_pe_info(exe)
    hook_va = 0x00418351
    original = rel32_call(hook_va, 0x004169ca)
    stub_va = next_section_va(pe)
    stub = build_read_prologue_transform_stub(stub_va, debug_dump)
    actual_stub_va = add_section(exe, ".rpread", stub)
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
        rel32_call(hook_va, stub_va),
        "read-prologue-transform-hook",
    )
    return {
        "name": "read-prologue-transform-hook",
        "hook_va": f"0x{hook_va:08x}",
        "stub_va": f"0x{stub_va:08x}",
        "stub_size": len(stub),
        "debug_dump": debug_dump,
        "note": (
            "Runs after BB prologue merge and before assembly emission; "
            "rewrites only the exact cReadBlock Read prologue pattern."
        ),
    }


def prepare_read_prologue(source_dir: Path, output_dir: Path,
                          apply_functional_patch: bool,
                          install_noop_hook: bool,
                          install_transform_hook: bool,
                          debug_transform_strings: bool) -> dict:
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
    if install_transform_hook:
        functional_count += 1
        applied.append(install_read_prologue_transform_hook(
            exe,
            debug_transform_strings,
        ))

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
    if args.stock_copy and args.read_prologue_transform_hook:
        raise SystemExit("--stock-copy and --read-prologue-transform-hook are mutually exclusive")
    if args.debug_read_prologue_strings and not args.read_prologue_transform_hook:
        raise SystemExit("--debug-read-prologue-strings requires --read-prologue-transform-hook")
    manifest = prepare_read_prologue(
        Path(args.source_dir),
        Path(args.output_dir),
        apply_functional_patch=not args.stock_copy,
        install_noop_hook=args.noop_cgemit_hook,
        install_transform_hook=args.read_prologue_transform_hook,
        debug_transform_strings=args.debug_read_prologue_strings,
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
    prep.add_argument(
        "--read-prologue-transform-hook",
        action="store_true",
        help="Install the experimental final-list Read prologue transform hook.",
    )
    prep.add_argument(
        "--debug-read-prologue-strings",
        action="store_true",
        help="With the transform hook, emit comment-only debug strings to kept assembly.",
    )
    prep.set_defaults(func=cmd_prepare_read_prologue)

    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
