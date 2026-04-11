#include "eRayCast.h"

// eRayCast constructor: initializes a ray cast query from origin to end.
// Sets collision mask, default hit values, copies origin, computes and
// normalizes direction (end - origin), stores ray length.
// Heavy VFPU usage for vector init, subtraction, normalization.
eRayCast::eRayCast(const mVec3 &origin, const mVec3 &end, unsigned int mask, bool flag) {
    __asm__ volatile(
        ".set noreorder\n"
        // mask bool to byte (5th arg in t0 per EABI)
        "andi $t0, $t0, 0xff\n"
        // store collision mask
        "sw $a3, 0($a0)\n"
        // zero field04
        "sw $zero, 4($a0)\n"
        // mEnabled = 1
        "ori $a3, $zero, 1\n"
        "sb $a3, 24($a0)\n"
        // mField19 = 0
        "sb $zero, 25($a0)\n"
        // mMaxDist = FLT_MAX (0x7f7fffff)
        "lui $a3, 0x7f7f\n"
        "ori $a3, $a3, 0xffff\n"
        "mtc1 $a3, $f12\n"
        "swc1 $f12, 32($a0)\n"
        // Zero vector {0,0,0} at offset 0x30
        "mtc1 $zero, $f12\n"
        "mfc1 $a3, $f12\n"
        "mfc1 $t1, $f12\n"
        "mfc1 $t2, $f12\n"
        "mtv $a3, S120\n"
        "mtv $t1, S121\n"
        "mtv $t2, S122\n"
        "sv.q C120, 0x30($a0)\n"
        // {0,0,1.0f} vector at offset 0x40
        "mfc1 $a3, $f12\n"
        "mfc1 $t1, $f12\n"
        "lui $t2, 0x3f80\n"
        "mtc1 $t2, $f13\n"
        "mfc1 $t2, $f13\n"
        "mtv $a3, S120\n"
        "mtv $t1, S121\n"
        "mtv $t2, S122\n"
        "sv.q C120, 0x40($a0)\n"
        // Hit indices = -1, fields = 0
        "li $a3, -1\n"
        "sw $a3, 80($a0)\n"
        "sw $a3, 84($a0)\n"
        "sw $zero, 88($a0)\n"
        "sw $zero, 92($a0)\n"
        // Copy origin, compute direction = end - origin
        "lv.q C120, 0x0($a1)\n"
        "sv.q C120, 0x60($a0)\n"
        "lv.q C130, 0x0($a2)\n"
        "vsub.t C120, C130, C120\n"
        "sv.q C120, 0x70($a0)\n"
        // Normalize direction: lengthSq = dot(dir,dir)
        "vdot.t S100, C120, C120\n"
        "vcmp.s EZ, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        // If lengthSq == 0, set invLen = 1.0 to avoid NaN
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        // dir *= invLen
        "vscl.t C120, C120, S101\n"
        "sv.q C120, 0x70($a0)\n"
        // Extract length = sqrt(lengthSq) and store
        "mfv $a1, S102\n"
        "mtc1 $a1, $f12\n"
        "swc1 $f12, 128($a0)\n"
        // Store bool flag
        "sb $t0, 144($a0)\n"
        ".set reorder\n"
    );
}
