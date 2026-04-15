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
    "lv.q C120, 0x20($a0)\n"
    "vdot.t S100, C120, C120\n"
    "vcmp.s ez, S100\n"
    "vrsq.s S101, S100\n"
    "vsqrt.s S102, S100\n"
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W            — prefix: S100 = 1.0
    "vcmovt.s S101, S100, 0\n"
    "vscl.t C120, C120, S101\n"
    "sv.q C120, 0x20($a0)\n"
    // Step 2: Orthogonalize and normalize row1 (y-axis, offset 0x10)
    "lv.q C130, 0x10($a0)\n"
    "vdot.t S100, C120, C130\n"
    "mfv $a1, S100\n"
    "mtc1 $a1, $f12\n"
    "mfc1 $a1, $f12\n"
    "mtv $a1, S100\n"
    "vscl.t C200, C120, S100\n"
    "vsub.t C130, C130, C200\n"
    "vdot.t S100, C130, C130\n"
    "vcmp.s ez, S100\n"
    "vrsq.s S101, S100\n"
    "vsqrt.s S102, S100\n"
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
    "vcmovt.s S101, S100, 0\n"
    "vscl.t C130, C130, S101\n"
    "sv.q C130, 0x10($a0)\n"
    // Step 3: Orthogonalize and normalize row0 (x-axis, offset 0x00)
    "lv.q C200, 0($a0)\n"
    "vdot.t S100, C120, C200\n"
    "mfv $a1, S100\n"
    "mtc1 $a1, $f12\n"
    "mfc1 $a1, $f12\n"
    "mtv $a1, S100\n"
    "vscl.t C120, C120, S100\n"
    "vdot.t S100, C130, C200\n"
    "mfv $a1, S100\n"
    "mtc1 $a1, $f13\n"
    "mfc1 $a1, $f13\n"
    "mtv $a1, S100\n"
    "vscl.t C130, C130, S100\n"
    "vsub.t C120, C200, C120\n"
    "vsub.t C120, C120, C130\n"
    "vdot.t S100, C120, C120\n"
    "vcmp.s ez, S100\n"
    "vrsq.s S101, S100\n"
    "vsqrt.s S102, S100\n"
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
    "vcmovt.s S101, S100, 0\n"
    "vscl.t C120, C120, S101\n"
    "jr $ra\n"
    "sv.q C120, 0($a0)\n"
    ".set reorder\n"
);
