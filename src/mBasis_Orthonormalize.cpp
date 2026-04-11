#include "mBasis.h"

// mBasis::Orthonormalize(void)
// Address: 0x001a020c, Size: 196B, Obj: mAll_psp.obj
//
// Gram-Schmidt orthonormalization of a 3x3 rotation matrix (3 quadword rows).
// Algorithm:
//   1. Normalize row2 (z-axis)
//   2. row1 -= dot(row2, row1) * row2, then normalize row1 (y-axis)
//   3. row0 -= dot(row2, row0) * row2 - dot(row1, row0) * row1, then normalize (x-axis)
//
// Each normalization uses: vdot -> vrsq -> vcmp(ez) -> vcmovt(safe_rsq) -> vscl
// The mfv/mtc1/mfc1/mtv round-trips force dot products through the FPU to
// normalize float NaN/denorm handling between VFPU and FPU register files.
//
// Entirely VFPU — no scalar equivalent exists for this instruction set.
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fGmBasisOOrthonormalizev\n"
    "__0fGmBasisOOrthonormalizev:\n"
    // Step 1: Normalize row2 (z-axis, offset 0x20)
    ".word 0xd8860020\n"  // lv.q C120, 0x20($a0)       — load row2
    ".word 0x64868604\n"  // vdot.t S100, C120, C120     — dot(row2, row2)
    ".word 0x6c000408\n"  // vcmp.s ez, S100             — check if zero
    ".word 0xd0110424\n"  // vrsq.s S101, S100           — 1/sqrt(dot)
    ".word 0xd0160444\n"  // vsqrt.s S102, S100          — sqrt(dot) (unused, SNC artifact)
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W            — prefix: S100 = 1.0
    ".word 0xd2a00424\n"  // vcmovt.s S101, S100, 0      — if zero, rsq = 1.0 (safe divide)
    ".word 0x65248606\n"  // vscl.t C120, C120, S101     — row2 *= rsq
    ".word 0xf8860020\n"  // sv.q C120, 0x20($a0)        — store normalized row2
    // Step 2: Orthogonalize and normalize row1 (y-axis, offset 0x10)
    ".word 0xd8870010\n"  // lv.q C130, 0x10($a0)        — load row1
    ".word 0x64878604\n"  // vdot.t S100, C120, C130     — dot(row2, row1)
    ".word 0x48650004\n"  // mfv $a1, S100               — VFPU -> CPU
    ".word 0x44856000\n"  // mtc1 $a1, $f12              — CPU -> FPU
    ".word 0x44056000\n"  // mfc1 $a1, $f12              — FPU -> CPU
    ".word 0x48e50004\n"  // mtv $a1, S100               — CPU -> VFPU
    ".word 0x65048608\n"  // vscl.t C200, C120, S100     — C200 = row2 * dot
    ".word 0x60888707\n"  // vsub.t C130, C130, C200     — row1 -= row2 * dot(row2,row1)
    ".word 0x64878704\n"  // vdot.t S100, C130, C130     — dot(row1, row1)
    ".word 0x6c000408\n"  // vcmp.s ez, S100
    ".word 0xd0110424\n"  // vrsq.s S101, S100
    ".word 0xd0160444\n"  // vsqrt.s S102, S100
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
    ".word 0xd2a00424\n"  // vcmovt.s S101, S100, 0
    ".word 0x65248707\n"  // vscl.t C130, C130, S101     — normalize row1
    ".word 0xf8870010\n"  // sv.q C130, 0x10($a0)        — store normalized row1
    // Step 3: Orthogonalize and normalize row0 (x-axis, offset 0x00)
    ".word 0xd8880000\n"  // lv.q C200, 0x0($a0)         — load row0
    ".word 0x64888604\n"  // vdot.t S100, C120, C200     — dot(row2, row0)
    ".word 0x48650004\n"  // mfv $a1, S100
    ".word 0x44856000\n"  // mtc1 $a1, $f12
    ".word 0x44056000\n"  // mfc1 $a1, $f12
    ".word 0x48e50004\n"  // mtv $a1, S100
    ".word 0x65048606\n"  // vscl.t C120, C120, S100     — C120 = row2 * dot(row2,row0)
    ".word 0x64888704\n"  // vdot.t S100, C130, C200     — dot(row1, row0)
    ".word 0x48650004\n"  // mfv $a1, S100
    ".word 0x44856800\n"  // mtc1 $a1, $f13              — note: $f13, not $f12
    ".word 0x44056800\n"  // mfc1 $a1, $f13
    ".word 0x48e50004\n"  // mtv $a1, S100
    ".word 0x65048707\n"  // vscl.t C130, C130, S100     — C130 = row1 * dot(row1,row0)
    ".word 0x60868806\n"  // vsub.t C120, C200, C120     — row0 - row2*dot(row2,row0)
    ".word 0x60878606\n"  // vsub.t C120, C120, C130     — row0 -= row1*dot(row1,row0)
    ".word 0x64868604\n"  // vdot.t S100, C120, C120     — dot(row0, row0)
    ".word 0x6c000408\n"  // vcmp.s ez, S100
    ".word 0xd0110424\n"  // vrsq.s S101, S100
    ".word 0xd0160444\n"  // vsqrt.s S102, S100
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
    ".word 0xd2a00424\n"  // vcmovt.s S101, S100, 0
    ".word 0x65248606\n"  // vscl.t C120, C120, S101     — normalize row0
    ".word 0x03e00008\n"  // jr ra
    ".word 0xf8860000\n"  // sv.q C120, 0x0($a0)         — store row0 (delay slot)
    ".set reorder\n"
);
