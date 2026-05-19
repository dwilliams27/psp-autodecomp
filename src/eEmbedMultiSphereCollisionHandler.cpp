// ODR-WARNING: split translation unit local redeclarations for targeted methods only.

#include "mOCS.h"

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mCapsule {
    SceULong128 start;
    SceULong128 end;
    float radius;
};

class eEmbedVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
};

class eEmbedMultiSphereCollisionHandler : public eEmbedVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
    void Set(const mCapsule &capsule);
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

void eEmbedMultiSphereCollisionHandler::Set(const mCapsule &capsule) {
    volatile SceULong128 direction;
    volatile SceULong128 pad;
    __asm__ volatile("" : : "m"(pad));
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "lv.q C120, 0x0($a1)\n"
        "sv.q C120, 0x48d0($a0)\n"
        "lv.q C120, 0x10($a1)\n"
        "sv.q C120, 0x48e0($a0)\n"
        "lwc1 $f13, 0x20($a1)\n"
        "swc1 $f13, 0x48f0($a0)\n"
        "lv.q C120, 0x48e0($a0)\n"
        "lv.q C130, 0x48d0($a0)\n"
        "vsub.t C120, C120, C130\n"
        "vdot.t S100, C120, C120\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        "vscl.t C120, C120, S101\n"
        "sv.q C120, %0\n"
        "mfv $a1, S102\n"
        "mtc1 $a1, $f12\n"
        "lui $a1, 0x4000\n"
        "mtc1 $a1, $f14\n"
        "mul.s $f13, $f13, $f14\n"
        "add.s $f15, $f12, $f13\n"
        "lui $a1, 0x3f19\n"
        "ori $a1, $a1, 0x999a\n"
        "mtc1 $a1, $f16\n"
        "mul.s $f13, $f13, $f16\n"
        "div.s $f13, $f15, $f13\n"
        "trunc.w.s $f13, $f13\n"
        "mfc1 $a2, $f13\n"
        "slti $a3, $a2, 3\n"
        "bnez $a3, 1f\n"
        "ori $a1, $zero, 2\n"
        "move $a1, $a2\n"
        "slti $a2, $a1, 4\n"
        "beqzl $a2, 1f\n"
        "ori $a1, $zero, 4\n"
        "1:\n"
        "sw $a1, 0x48c0($a0)\n"
        "move $a2, $a1\n"
        "addiu $a1, $a2, -1\n"
        "mtc1 $a1, $f13\n"
        "cvt.s.w $f13, $f13\n"
        "div.s $f12, $f12, $f13\n"
        "ori $a1, $zero, 0\n"
        "slt $a2, $a1, $a2\n"
        "beqz $a2, 3f\n"
        "nop\n"
        "lv.q C120, %0\n"
        "addiu $a2, $a0, 0x4900\n"
        "ori $a3, $zero, 0\n"
        "addu $a2, $a2, $a3\n"
        "2:\n"
        "mtc1 $a1, $f13\n"
        "cvt.s.w $f13, $f13\n"
        "mul.s $f13, $f12, $f13\n"
        "mfc1 $a3, $f13\n"
        "mtv $a3, S100\n"
        "lv.q C130, 0x48d0($a0)\n"
        "vscl.t C200, C120, S100\n"
        "vadd.t C130, C130, C200\n"
        "lwc1 $f14, 0x48f0($a0)\n"
        "sv.q C130, 0x0($a2)\n"
        "swc1 $f14, 0xc($a2)\n"
        "addiu $a1, $a1, 1\n"
        "lw $a3, 0x48c0($a0)\n"
        "slt $a3, $a1, $a3\n"
        "bnez $a3, 2b\n"
        "addiu $a2, $a2, 0x10\n"
        "3:\n"
        ".set pop\n"
        : "+m"(direction)
        :
        : "memory", "$a1", "$a2", "$a3", "$f12", "$f13", "$f14", "$f15",
          "$f16");
}
