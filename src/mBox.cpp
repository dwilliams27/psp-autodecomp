#include "mBox.h"
#include "mFrustum.h"
#include "mVec3.h"
#include "mOCS.h"

// mBox::XformInv(const mBox &, const mOCS &) @ 0x001a0af8
// this = inverse-OCS transform of `other`. If other is an invalid (empty) box,
// this becomes invalid too. Otherwise, corners of other are transformed by
// OCS^-1 and this is set to their AABB.
void mBox::XformInv(const mBox &other, const mOCS &ocs) {
    // Load other's min/max into VFPU regs (compiler-schedulable) and
    // compare componentwise — any min > max means the box is invalid.
    typedef int v4sf_t __attribute__((mode(V4SF)));
    v4sf_t vomin = *(const v4sf_t *)&other.min;
    v4sf_t vomax = *(const v4sf_t *)&other.max;

    int flags;
    __asm__ volatile(
        "vcmp.t gt, %1, %2\n"
        "vnop\n"
        "mfvc %0, $131\n"
        : "=r"(flags) : "x"(vomin), "x"(vomax)
    );

    // Interleave fmax/fmin construction with the invalid-flag andi chain —
    // matches SNC's observed instruction scheduling for this prologue.
    float fmax, fmin;
    int t;
    int invalid;
    __asm__ volatile("lui %0, 0x7f7f" : "=r"(t));
    __asm__ volatile("ori %0, %0, 0xffff" : "+r"(t));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fmax) : "r"(t));
    __asm__ volatile("andi %0, %1, 0x20" : "=r"(invalid) : "r"(flags));
    __asm__ volatile("lui %0, 0xff7f" : "=r"(t));
    __asm__ volatile("sltu %0, $zero, %0" : "+r"(invalid));
    __asm__ volatile("ori %0, %0, 0xffff" : "+r"(t));
    __asm__ volatile("andi %0, %0, 0xff" : "+r"(invalid));
    __asm__ volatile("mtc1 %1, %0" : "=f"(fmin) : "r"(t));

    if (invalid) {
        // Set this to invalid (empty) box
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(fmax));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(fmax));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(fmax));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(this)
        );
        int ja, jb, jc;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ja) : "f"(fmin));
        __asm__ volatile("mfc1 %0, %1" : "=r"(jb) : "f"(fmin));
        __asm__ volatile("mfc1 %0, %1" : "=r"(jc) : "f"(fmin));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x10(%3)\n"
            :: "r"(ja), "r"(jb), "r"(jc), "r"(this)
        );
        return;
    }

    mVec3 corners[8];
    other.GetCorners(corners);

    // Init this to invalid
    int ia, ib, ic;
    __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(fmax));
    __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(fmax));
    __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(fmax));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(ia), "r"(ib), "r"(ic), "r"(this)
    );
    int ja, jb, jc;
    __asm__ volatile("mfc1 %0, %1" : "=r"(ja) : "f"(fmin));
    __asm__ volatile("mfc1 %0, %1" : "=r"(jb) : "f"(fmin));
    __asm__ volatile("mfc1 %0, %1" : "=r"(jc) : "f"(fmin));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x10(%3)\n"
        :: "r"(ja), "r"(jb), "r"(jc), "r"(this)
    );

    // Transform each corner by OCS inverse and extend this
    int i = 0;
    do {
        mVec3 *c = &corners[i];
        __asm__ volatile(
            "lv.q C120, 0(%0)\n"
            "lv.q C000, 0(%1)\n"
            "lv.q C010, 0x10(%1)\n"
            "lv.q C020, 0x20(%1)\n"
            "lv.q C030, 0x30(%1)\n"
            "vsub.t C120, C120, C030\n"
            "vtfm3.t C130, M000, C120\n"
            "sv.q C130, 0(%0)\n"
            "lv.q C120, 0(%0)\n"
            "lv.q C130, 0(%2)\n"
            "vmin.t C120, C130, C120\n"
            "sv.q C120, 0(%2)\n"
            "lv.q C120, 0(%0)\n"
            "lv.q C130, 0x10(%2)\n"
            "vmax.t C120, C130, C120\n"
            "sv.q C120, 0x10(%2)\n"
            :: "r"(c), "r"(&ocs), "r"(this) : "memory"
        );
        i++;
    } while (i < 8);
}

// mBox::GetCorners(mVec3 *) const @ 0x001a0928
// Produces the 8 corners of the AABB as mVec3s. Uses the SNC
// mtc1/mfc1/mtv/sv.q pattern from the inlined mVec3(float,float,float) ctor.
void mBox::GetCorners(mVec3 *out) const {
    // Corner 0: (mx, my, mz) — whole min quadword copy
    __asm__ volatile(
        "lv.q C120, 0(%0)\n"
        "sv.q C120, 0(%1)\n"
        :: "r"(this), "r"(out) : "memory"
    );

    const float *b = (const float *)this;

    // Corner 1: (mx, my, Mz)
    {
        float x = b[0], y = b[1], z = b[6];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x10(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 2: (mx, My, mz)
    {
        float x = b[0], y = b[5], z = b[2];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x20(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 3: (mx, My, Mz)
    {
        float x = b[0], y = b[5], z = b[6];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x30(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 4: (Mx, my, mz)
    {
        float x = b[4], y = b[1], z = b[2];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x40(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 5: (Mx, my, Mz)
    {
        float x = b[4], y = b[1], z = b[6];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x50(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 6: (Mx, My, mz)
    {
        float x = b[4], y = b[5], z = b[2];
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x60(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(out)
        );
    }

    // Corner 7: (Mx, My, Mz) — compiler-generated quad copy so sv.q lands in jr delay slot
    out[7].qw = this->max;
}

// mBox::Extend(int, const mVec3 *, int) @ 0x001a0a2c
// Expand this box to contain `count` points starting at `pts` with byte stride.
void mBox::Extend(int count, const mVec3 *pts, int stride) {
    int i = 0;
    if (i < count) {
        do {
            __asm__ volatile(
                "lv.q C120, 0(%0)\n"
                "lv.q C130, 0(%1)\n"
                "vmin.t C120, C120, C130\n"
                "sv.q C120, 0(%0)\n"
                "lv.q C120, 0x10(%0)\n"
                "lv.q C130, 0(%1)\n"
                "vmax.t C120, C120, C130\n"
                :: "r"(this), "r"(pts) : "memory"
            );
            i++;
            __asm__ volatile(
                "sv.q C120, 0x10(%0)\n"
                :: "r"(this) : "memory"
            );
            pts = (const mVec3 *)((const char *)pts + stride);
        } while (i < count);
    }
}

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
