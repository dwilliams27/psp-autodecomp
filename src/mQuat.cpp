#include "mQuat.h"

// mQuat::Mult(const mQuat &, const mQuat &)
// Address: 0x001a2f38, Size: 180B, Obj: mAll_psp.obj
//
// Quaternion multiplication: this = b * a (Hamilton product)
//   result.x = bw*ax + bx*aw + by*az - bz*ay
//   result.y = bw*ay - bx*az + by*aw + bz*ax
//   result.z = bw*az + bx*ay - by*ax + bz*aw
//   result.w = bw*aw - bx*ax - by*ay - bz*az
//
// Computes into a stack-local mQuat, then copies to this via VFPU quadword.
// The scalar FPU computation matches C++ exactly (verified with qw=r.qw,
// 178/180 bytes identical). File-scope asm needed only because SNC's VFPU
// register allocator picks C000 in isolation but the original binary uses
// C120 (compilation-unit state from mAll_psp.obj).
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fFmQuatEMultRC6FmQuatTB\n"
    "__0fFmQuatEMultRC6FmQuatTB:\n"
    "addiu $sp, $sp, -16\n"
    "lwc1 $f12, 12($a2)\n"
    "lwc1 $f13, 0($a1)\n"
    "lwc1 $f14, 0($a2)\n"
    "lwc1 $f15, 12($a1)\n"
    "mul.s $f16, $f12, $f13\n"
    "lwc1 $f17, 8($a1)\n"
    "mul.s $f18, $f14, $f15\n"
    "lwc1 $f19, 4($a1)\n"
    "lwc1 $f0, 4($a2)\n"
    "mul.s $f2, $f14, $f17\n"
    "mul.s $f1, $f12, $f19\n"
    "lwc1 $f3, 8($a2)\n"
    "mul.s $f4, $f0, $f17\n"
    "add.s $f16, $f16, $f18\n"
    "mul.s $f6, $f3, $f19\n"
    "sub.s $f2, $f1, $f2\n"
    "mul.s $f7, $f0, $f15\n"
    "add.s $f16, $f16, $f4\n"
    "mul.s $f5, $f12, $f17\n"
    "mul.s $f9, $f0, $f13\n"
    "mul.s $f8, $f14, $f19\n"
    "mul.s $f4, $f3, $f13\n"
    "sub.s $f16, $f16, $f6\n"
    "add.s $f18, $f2, $f7\n"
    "mul.s $f12, $f12, $f15\n"
    "mul.s $f13, $f14, $f13\n"
    "swc1 $f16, 0($sp)\n"
    "add.s $f1, $f5, $f8\n"
    "add.s $f16, $f18, $f4\n"
    "mul.s $f15, $f3, $f15\n"
    "sub.s $f12, $f12, $f13\n"
    "mul.s $f14, $f0, $f19\n"
    "sub.s $f2, $f1, $f9\n"
    "swc1 $f16, 4($sp)\n"
    "mul.s $f16, $f3, $f17\n"
    "sub.s $f12, $f12, $f14\n"
    "add.s $f15, $f2, $f15\n"
    "sub.s $f12, $f12, $f16\n"
    "swc1 $f15, 8($sp)\n"
    "swc1 $f12, 12($sp)\n"
    "lv.q C120, 0($sp)\n"
    "sv.q C120, 0($a0)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 16\n"
    ".set reorder\n"
);

// mBasis::Orthonormalize(void)
// Address: 0x001a020c, Size: 196B, Obj: mAll_psp.obj
//
// Gram-Schmidt orthonormalization of a 3x3 basis matrix (3 quadword rows).
// Entirely VFPU — normalize row2, orthogonalize+normalize row1, then row0.
// The mfv/mtc1/mfc1/mtv round-trips are a compiler artifact that forces
// the VFPU dot product result through the FPU pipeline (possibly for
// IEEE single-precision rounding).
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fGmBasisOOrthonormalizev\n"
    "__0fGmBasisOOrthonormalizev:\n"
    // Step 1: Normalize row2
    "lv.q C120, 0x20($a0)\n"
    "vdot.t S100, C120, C120\n"
    "vcmp.s ez, S100\n"
    "vrsq.s S101, S100\n"
    "vsqrt.s S102, S100\n"
    ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
    "vcmovt.s S101, S100, 0\n"
    "vscl.t C120, C120, S101\n"
    "sv.q C120, 0x20($a0)\n"
    // Step 2: Orthogonalize and normalize row1
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
    // Step 3: Orthogonalize and normalize row0
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
