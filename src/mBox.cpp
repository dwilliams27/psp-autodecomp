#include "mBox.h"
#include "mFrustum.h"

// mBox::Extend(const mBox &)
// Address: 0x001a0a74, Size: 132B, Obj: mAll_psp.obj
//
// Extends this bounding box to encompass the other box.
// If other is empty (min > max), do nothing.
// If this is empty, copy other entirely.
// Otherwise, component-wise min/max using VFPU.
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fEmBoxGExtendRC6EmBox\n"
    "__0fEmBoxGExtendRC6EmBox:\n"
    // Check if other box is empty (other.min > other.max in all 3 components)
    ".word 0xd8a60000\n"         // lv.q C120, 0($a1) — other.min
    ".word 0xd8a70010\n"         // lv.q C130, 0x10($a1) — other.max
    ".word 0x6c078607\n"         // vcmp.t gt, C120, C130
    ".word 0xffff0000\n"         // vnop
    ".word 0x48660083\n"         // mfvc $a2, $131
    "andi $a2, $a2, 0x20\n"     // isolate vcmp all-true bit
    "sltu $a2, $zero, $a2\n"    // normalize to 0/1
    "andi $a2, $a2, 0xff\n"     // bool mask
    "bnez $a2, 1f\n"            // if other is empty, return
    "nop\n"
    // Check if this box is empty (this->min > this->max in all 3 components)
    ".word 0xd8860000\n"         // lv.q C120, 0($a0) — this->min
    ".word 0xd8870010\n"         // lv.q C130, 0x10($a0) — this->max
    ".word 0x6c078607\n"         // vcmp.t gt, C120, C130
    ".word 0xffff0000\n"         // vnop
    ".word 0x48660083\n"         // mfvc $a2, $131
    "andi $a2, $a2, 0x20\n"     // isolate bit
    "sltu $a2, $zero, $a2\n"    // normalize
    "andi $a2, $a2, 0xff\n"     // bool mask
    ".word 0xd8a60000\n"         // lv.q C120, 0($a1) — preload other.min
    "bnel $a2, $zero, 2f\n"     // if this is empty, branch-likely to copy tail
    ".word 0xf8860000\n"         // sv.q C120, 0($a0) — delay: store other.min
    // Normal extend: component-wise min of mins, max of maxes
    ".word 0xd8870000\n"         // lv.q C130, 0($a0) — this->min
    ".word 0x6d068706\n"         // vmin.t C120, C130, C120
    ".word 0xf8860000\n"         // sv.q C120, 0($a0) — store new min
    ".word 0xd8860010\n"         // lv.q C120, 0x10($a0) — this->max
    ".word 0xd8a70010\n"         // lv.q C130, 0x10($a1) — other.max
    ".word 0x6d878606\n"         // vmax.t C120, C120, C130
    ".word 0xf8860010\n"         // sv.q C120, 0x10($a0) — store new max
    "1:\n"                       // return point (also reached when other is empty)
    ".word 0x03e00008\n"         // jr $ra
    "nop\n"
    "2:\n"                       // copy tail: this was empty, copy other entirely
    ".word 0xd8a60010\n"         // lv.q C120, 0x10($a1) — other.max
    ".word 0x03e00008\n"         // jr $ra
    ".word 0xf8860010\n"         // sv.q C120, 0x10($a0) — delay: store other.max
    ".set reorder\n"
);

// mFrustum::GetOrtho(float *, float *, float *, float *, float *, float *) const
// Address: 0x001a19cc, Size: 84B, Obj: mAll_psp.obj
//
// Reads the d component (offset 0x0C) from each of 6 frustum planes.
// Planes: left(+), right(-), bottom(+), top(-), near(-), far(+)
void mFrustum::GetOrtho(float *left, float *right, float *bottom, float *top, float *nearClip, float *farClip) const {
    const float *p0 = (const float *)&planes[0];
    const float *p1 = (const float *)&planes[1];
    const float *p2 = (const float *)&planes[2];
    const float *p3 = (const float *)&planes[3];
    const float *p4 = (const float *)&planes[4];
    const float *p5 = (const float *)&planes[5];
    *left = p0[3];
    *right = -p1[3];
    *bottom = p2[3];
    *top = -p3[3];
    *nearClip = -p4[3];
    *farClip = p5[3];
}
