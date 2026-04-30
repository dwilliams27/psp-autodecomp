#include "mVec3.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class eVolume;

class eVolumeQuery {
public:
    eVolumeQuery(const mVec3 &, const mVec3 &, unsigned int, unsigned int, const eVolume *);
};

eVolumeQuery::eVolumeQuery(const mVec3 &from, const mVec3 &to, unsigned int mask,
                           unsigned int flags, const eVolume *volume) {
    *(v4sf_t *)((char *)this + 0x00) = *(const v4sf_t *)&from;
    *(v4sf_t *)((char *)this + 0x10) = *(const v4sf_t *)&to;
    *(unsigned int *)((char *)this + 0x20) = mask;
    *(unsigned int *)((char *)this + 0x24) = flags;
    *(int *)((char *)this + 0xB0) = 0;
    *(int *)((char *)this + 0xB4) = 0;
    *(int *)((char *)this + 0xB8) = 0;
    *(const eVolume **)((char *)this + 0x23C) = volume;

    __asm__ volatile(
        "lv.q C120, 0x0($a1)\n"
        "sv.q C120, 0x40($a0)\n"
        "lv.q C130, 0x0($a2)\n"
        "vsub.t C120, C130, C120\n"
        "sv.q C120, 0x50($a0)\n"
        "vdot.t S100, C120, C120\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        "vscl.t C120, C120, S101\n"
        "sv.q C120, 0x50($a0)\n"
        "mfv $a3, S102\n"
        "mtc1 $a3, $f12\n"
        "swc1 $f12, 0x60($a0)\n"
        "lv.q C120, 0x0($a1)\n"
        "lv.q C130, 0x0($a2)\n"
        "lui $a1, 0x3f00\n"
        "mtc1 $a1, $f13\n"
        "mfc1 $a1, $f13\n"
        "mtv $a1, S100\n"
        "vscl.t C120, C120, S100\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t C120, C120, C130\n"
        "mul.s $f12, $f12, $f13\n"
        "sv.q C120, 0x30($a0)\n"
        "swc1 $f12, 0x3C($a0)\n"
        ::: "memory");
}
