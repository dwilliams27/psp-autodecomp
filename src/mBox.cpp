#include "mBox.h"
#include "mFrustum.h"

void mBox::Extend(const mBox &other) {
    int cond;

    // Check if other box is valid (min <= max in all 3 components)
    __asm__ volatile(
        "lv.q C120, 0($a1)\n"
        "lv.q C130, 0x10($a1)\n"
        "vcmp.t gt, C120, C130\n"
        "vnop\n"
        "mfvc %0, $131\n"
        : "=r"(cond)
    );

    if ((unsigned char)((cond & 0x20) != 0))
        return;

    // Check if this box is valid
    __asm__ volatile(
        "lv.q C120, 0($a0)\n"
        "lv.q C130, 0x10($a0)\n"
        "vcmp.t gt, C120, C130\n"
        "vnop\n"
        "mfvc %0, $131\n"
        "andi %0, %0, 0x20\n"
        "sltu %0, $zero, %0\n"
        "andi %0, %0, 0xff\n"
        : "=r"(cond)
    );

    // Reload other.min into C120 (needed for both paths)
    SceULong128 oMin = other.min;

    if (cond) {
        // This box is uninitialized — just copy other
        this->min = oMin;
        this->max = other.max;
        return;
    }

    // Both boxes valid — extend with min/max
    __asm__ volatile(
        "lv.q C130, 0($a0)\n"
        "vmin.t C120, C130, C120\n"
        "sv.q C120, 0($a0)\n"
        "lv.q C120, 0x10($a0)\n"
        "lv.q C130, 0x10($a1)\n"
        "vmax.t C120, C120, C130\n"
        "sv.q C120, 0x10($a0)\n"
    );
}

void mFrustum::GetOrtho(float *left, float *right, float *bottom, float *top, float *nearVal, float *farVal) const {
    const mPlane *p = (const mPlane *)this;
    const mPlane *p1 = p + 1;
    const mPlane *p2 = p + 2;
    const mPlane *p3 = p + 3;
    const mPlane *p4 = p + 4;
    const mPlane *p5 = p + 5;
    *left = p->w;
    *right = -p1->w;
    *bottom = p2->w;
    *top = -p3->w;
    *nearVal = -p4->w;
    *farVal = p5->w;
}
