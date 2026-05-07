// ODR-WARNING: split translation unit local redeclarations for targeted methods only.

#include "mOCS.h"

typedef unsigned int SceULong128 __attribute__((mode(TI)));

class eEmbedVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
};

class eEmbedMultiSphereCollisionHandler : public eEmbedVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
};

struct mPlane {
    float x, y, z, w;
};

struct __attribute__((aligned(16))) mFrustum {
    mPlane planes[6];
    void SetOrtho(float, float, float, float, float, float);
};

#pragma control sched=1
void eEmbedMultiSphereCollisionHandler::SetLocalToWorld(const mOCS &ocs) {
    eEmbedVolumeCollisionHandler::SetLocalToWorld(ocs);

    int i = 0;
    __asm__ volatile("" : "+r"(i));
    if (i < *(int *)((char *)this + 0x48c0)) {
        char *dst = (char *)this + 0x4940;
        char *src = (char *)this + 0x4900;
        do {
            float r = ((float *)src)[3];
            SceULong128 c = *(SceULong128 *)src;
            __asm__ volatile(
                "lv.q C000, 0x0(%1)\n"
                "lv.q C010, 0x10(%1)\n"
                "lv.q C020, 0x20(%1)\n"
                "lv.q C030, 0x30(%1)\n"
                "vsub.t %0, %0, C030\n"
                "vtfm3.t C130, M000, %0\n"
                "sv.q C130, 0x0(%2)\n"
                : "+X"(c)
                : "r"(&ocs), "r"(dst)
                : "memory"
            );
            ((float *)dst)[3] = r;
            i++;
            dst += 0x10;
            src += 0x10;
        } while (i < *(int *)((char *)this + 0x48c0));
    }
}
#pragma control sched=2

void mFrustum::SetOrtho(float a, float b, float c, float d, float e, float f) {
    __asm__ volatile(
        "lui $a1, 0x3f80\n"
        "mtc1 $zero, $f18\n"
        "mtc1 $a1, $f19\n"
        "mfc1 $a1, $f19\n"
        "mfc1 $a2, $f18\n"
        "mfc1 $a3, $f18\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x20($a0)\n"
        ::
        : "memory"
    );

    __asm__ volatile(
        "lui $a1, 0xbf80\n"
        "mfc1 $a2, $f18\n"
        ::
        : "memory"
    );
    planes[2].w = c;
    __asm__ volatile("mtc1 $a1, $f0\n");
    float nd = -d;
    __asm__ volatile(
        "mfc1 $a1, $f0\n"
        "mfc1 $a3, $f18\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x30($a0)\n"
        ::
        : "memory"
    );

    __asm__ volatile("mfc1 $a1, $f18\n" ::: "memory");
    planes[3].w = nd;
    __asm__ volatile("mfc1 $a2, $f18\n" ::: "memory");
    float ne = -e;
    __asm__ volatile(
        "mfc1 $a3, $f0\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x40($a0)\n"
        ::
        : "memory"
    );

    __asm__ volatile("mfc1 $a1, $f18\n" ::: "memory");
    planes[4].w = ne;
    __asm__ volatile(
        "mfc1 $a2, $f18\n"
        "mfc1 $a3, $f19\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x50($a0)\n"
        ::
        : "memory"
    );

    __asm__ volatile("mfc1 $a1, $f18\n" ::: "memory");
    planes[5].w = f;
    __asm__ volatile(
        "mfc1 $a2, $f19\n"
        "mfc1 $a3, $f18\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x0($a0)\n"
        ::
        : "memory"
    );

    __asm__ volatile("mfc1 $a1, $f18\n" ::: "memory");
    planes[0].w = a;
    __asm__ volatile("mfc1 $a2, $f0\n" ::: "memory");
    float nb = -b;
    __asm__ volatile(
        "mfc1 $a3, $f18\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x10($a0)\n"
        ::
        : "memory"
    );
    planes[1].w = nb;
}
