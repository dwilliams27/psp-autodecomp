#include "mVec3.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class mLine {
public:
    static void FindClosestPointOnLine(const mVec3 &a, const mVec3 &b,
                                       const mVec3 &p, mVec3 *out);
};

void mLine::FindClosestPointOnLine(const mVec3 &a, const mVec3 &b,
                                   const mVec3 &p, mVec3 *out) {
    v4sf_t vp = *(const v4sf_t *)&p;   /* C120 */
    v4sf_t va = *(const v4sf_t *)&a;   /* C130 */
    v4sf_t vb;                         /* C200 */
    __asm__ volatile(
        "vsub.t %0, %0, %1\n"
        "lv.q %2, 0(%3)\n"
        "vsub.t %2, %2, %1\n"
        "vdot.t S100, %2, %2\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        ".word 0xdc0010e5\n"  /* vpfxs 1, Y, Z, W */
        "vcmovt.s S101, S100, 0\n"
        "vscl.t %2, %2, S101\n"
        "vdot.t S100, %0, %2\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, $f12\n"
        "mfc1 $a0, $f12\n"
        "mtv $a0, S100\n"
        "vscl.t %0, %2, S100\n"
        "vadd.t %0, %1, %0\n"
        : "+v"(vp), "+v"(va), "=v"(vb)
        : "r"(&b)
        : "$4", "$f12", "memory"
    );
    *(v4sf_t *)out = vp;
}
