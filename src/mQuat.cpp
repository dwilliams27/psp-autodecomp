#include "mQuat.h"
#include "mBasis.h"
#include "mVec3.h"

extern "C" float acosf(float);
extern "C" float sinf(float);
extern "C" float sqrtf(float);

class cWriteBlock;

class cGUID {
public:
    int a;
    int b;
    cGUID();
    void Write(cWriteBlock &) const;
};

class cWriteBlock {
public:
    void Write(const cGUID &);
};

void mQuat::Mult(const mQuat &a, const mQuat &b) {
    mQuat result;
    result.x = b.w * a.x + b.x * a.w + b.y * a.z - b.z * a.y;
    result.y = b.w * a.y - b.x * a.z + b.y * a.w + b.z * a.x;
    result.z = b.w * a.z + b.x * a.y - b.y * a.x + b.z * a.w;
    result.w = b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z;
    *this = result;
}

void mQuat::GetHPR(float *h, float *p, float *r) const {
    mBasis basis;
    basis.SetQuat(*this);
    basis.GetHPR(h, p, r);
}

void mQuat::GetHPR(mVec3 *v) const {
    GetHPR((float *)v, (float *)v + 1, (float *)v + 2);
}

void mQuat::GetAxisAngle(mVec3 *axis, float *angle) const {
    float sinsq = 1.0f - w * w;
    if (sinsq < 0.00001f) {
        float fzero = 0.0f;
        float fone = 1.0f;
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(fzero));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(fzero));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(fone));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(axis)
        );
        *angle = 0.0f;
    } else {
        float inv = 1.0f / sqrtf(sinsq);
        float tmp[4];
        float tx = x;
        float ty = y;
        tmp[0] = tx;
        float tz = z;
        tmp[1] = ty;
        tmp[2] = tz;
        __asm__ volatile(
            "lv.q C120, 0(%0)\n"
            :: "r"(tmp)
        );
        int ii;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ii) : "f"(inv));
        __asm__ volatile(
            "mtv %0, S100\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(ii), "r"(axis)
        );
        float cw = w;
        float cval;
        if (cw <= -1.0f) {
            cval = -1.0f;
        } else if (cw < 1.0f) {
            cval = cw;
        } else {
            cval = 1.0f;
        }
        *angle = acosf(cval) * 57.29578f * -2.0f;
    }
}

void mQuat::Slerp(const mQuat &q0, const mQuat &q1, float t) {
    SceULong128 q1_stack;
    int flag;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vcmp.q eq, C120, C130\n"
        "vnop\n"
        "mfvc %0, $131\n"
        : "=r"(flag)
        : "r"(&q0), "r"(&q1)
    );
    float tt = t;
    int eq = ((flag & 0x20) != 0) & 0xFF;
    if (eq) {
        __asm__ volatile("sv.q C120, 0(%0)\n" :: "r"(this) : "memory");
        return;
    }

    int dot_int;
    __asm__ volatile(
        "vdot.q S100, C120, C130\n"
        "mfv %0, S100\n"
        : "=r"(dot_int)
    );
    float dot;
    __asm__ volatile("mtc1 %1, %0" : "=f"(dot) : "r"(dot_int));

    float one = 1.0f;
    float omt = one - tt;
    float diff;
    if (dot < 0.0f) {
        dot = -dot;
        __asm__ volatile("vneg.q C130, C130");
        diff = one - dot;
    } else {
        diff = one - dot;
    }
    __asm__ volatile("sv.q C130, 0(%0)\n" :: "r"(&q1_stack) : "memory");

    if (diff <= 0.0001f) {
        int a0, a1;
        __asm__ volatile("mfc1 %0, %1" : "=r"(a0) : "f"(omt));
        __asm__ volatile("mfc1 %0, %1" : "=r"(a1) : "f"(tt));
        __asm__ volatile("mtv %0, S100\n" :: "r"(a0));
        __asm__ volatile("mtv %0, S101\n" :: "r"(a1));
    } else {
        float cd;
        if (dot <= -1.0f) {
            cd = -1.0f;
        } else if (dot < one) {
            cd = dot;
        } else {
            cd = one;
        }
        float theta = acosf(cd);
        float invsin = one / sinf(theta);
        float c0 = sinf(omt * theta) * invsin;
        float c1 = sinf(theta * tt) * invsin;
        int ic0, ic1;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic0) : "f"(c0));
        __asm__ volatile("lv.q C120, 0(%0)\n" :: "r"(&q0));
        __asm__ volatile("mtv %0, S100\n" :: "r"(ic0));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic1) : "f"(c1));
        __asm__ volatile("mtv %0, S101\n" :: "r"(ic1));
    }
    __asm__ volatile(
        "lv.q C130, 0(%0)\n"
        "vscl.q C120, C120, S100\n"
        "vscl.q C130, C130, S101\n"
        "vadd.q C120, C120, C130\n"
        "sv.q C120, 0(%1)\n"
        :: "r"(&q1_stack), "r"(this) : "memory"
    );
}

void cGUID::Write(cWriteBlock &wb) const {
    wb.Write(*this);
}
