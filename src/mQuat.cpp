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
    ".word 0x27bdfff0\n"  // addiu sp, sp, -16
    ".word 0xc4cc000c\n"  // lwc1 $f12, 12(a2)      — bw = b.w
    ".word 0xc4ad0000\n"  // lwc1 $f13, 0(a1)       — ax = a.x
    ".word 0xc4ce0000\n"  // lwc1 $f14, 0(a2)       — bx = b.x
    ".word 0xc4af000c\n"  // lwc1 $f15, 12(a1)      — aw = a.w
    ".word 0x460d6402\n"  // mul.s $f16, $f12, $f13  — bw*ax
    ".word 0xc4b10008\n"  // lwc1 $f17, 8(a1)       — az = a.z
    ".word 0x460f7482\n"  // mul.s $f18, $f14, $f15  — bx*aw
    ".word 0xc4b30004\n"  // lwc1 $f19, 4(a1)       — ay = a.y
    ".word 0xc4c00004\n"  // lwc1 $f0, 4(a2)        — by = b.y
    ".word 0x46117082\n"  // mul.s $f2, $f14, $f17   — bx*az
    ".word 0x46136042\n"  // mul.s $f1, $f12, $f19   — bw*ay
    ".word 0xc4c30008\n"  // lwc1 $f3, 8(a2)        — bz = b.z
    ".word 0x46110102\n"  // mul.s $f4, $f0, $f17    — by*az
    ".word 0x46128400\n"  // add.s $f16, $f16, $f18  — bw*ax + bx*aw
    ".word 0x46131982\n"  // mul.s $f6, $f3, $f19    — bz*ay
    ".word 0x46020881\n"  // sub.s $f2, $f1, $f2     — bw*ay - bx*az
    ".word 0x460f01c2\n"  // mul.s $f7, $f0, $f15    — by*aw
    ".word 0x46048400\n"  // add.s $f16, $f16, $f4   — bw*ax+bx*aw+by*az
    ".word 0x46116142\n"  // mul.s $f5, $f12, $f17   — bw*az
    ".word 0x460d0242\n"  // mul.s $f9, $f0, $f13    — by*ax
    ".word 0x46137202\n"  // mul.s $f8, $f14, $f19   — bx*ay
    ".word 0x460d1902\n"  // mul.s $f4, $f3, $f13    — bz*ax
    ".word 0x46068401\n"  // sub.s $f16, $f16, $f6   — x = bw*ax+bx*aw+by*az-bz*ay
    ".word 0x46071480\n"  // add.s $f18, $f2, $f7    — bw*ay-bx*az+by*aw
    ".word 0x460f6302\n"  // mul.s $f12, $f12, $f15  — bw*aw
    ".word 0x460d7342\n"  // mul.s $f13, $f14, $f13  — bx*ax
    ".word 0xe7b00000\n"  // swc1 $f16, 0(sp)        — store result.x
    ".word 0x46082840\n"  // add.s $f1, $f5, $f8     — bw*az+bx*ay
    ".word 0x46049400\n"  // add.s $f16, $f18, $f4   — y = bw*ay-bx*az+by*aw+bz*ax
    ".word 0x460f1bc2\n"  // mul.s $f15, $f3, $f15   — bz*aw
    ".word 0x460d6301\n"  // sub.s $f12, $f12, $f13  — bw*aw-bx*ax
    ".word 0x46130382\n"  // mul.s $f14, $f0, $f19   — by*ay
    ".word 0x46090881\n"  // sub.s $f2, $f1, $f9     — bw*az+bx*ay-by*ax
    ".word 0xe7b00004\n"  // swc1 $f16, 4(sp)        — store result.y
    ".word 0x46111c02\n"  // mul.s $f16, $f3, $f17   — bz*az
    ".word 0x460e6301\n"  // sub.s $f12, $f12, $f14  — bw*aw-bx*ax-by*ay
    ".word 0x460f13c0\n"  // add.s $f15, $f2, $f15   — z = bw*az+bx*ay-by*ax+bz*aw
    ".word 0x46106301\n"  // sub.s $f12, $f12, $f16  — w = bw*aw-bx*ax-by*ay-bz*az
    ".word 0xe7af0008\n"  // swc1 $f15, 8(sp)        — store result.z
    ".word 0xe7ac000c\n"  // swc1 $f12, 12(sp)       — store result.w
    ".word 0xdba60000\n"  // lv.q C120, 0(sp)        — load result quadword
    ".word 0xf8860000\n"  // sv.q C120, 0(a0)        — store to this
    ".word 0x03e00008\n"  // jr ra
    ".word 0x27bd0010\n"  // addiu sp, sp, 16 (delay slot)
    ".set reorder\n"
);
