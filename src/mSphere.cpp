#include "mSphere.h"
#include "mCollideHit.h"
#include "nwConnection.h"
#include "gcGameCore.h"

int mSphere::CalcEmbeddedContact(const mSphere &other, mCollideHit *hit) const {
    __asm__ volatile(
        ".word 0xd8860000\n"  /* lv.q   C120, 0($a0)          */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0x60878606\n"  /* vsub.t C120, C120, C130       */
        ".word 0x64868604\n"  /* vdot.t S100, C120, C120       */
        ".word 0xc48c000c\n"  /* lwc1   $f12, 12($a0)         */
        ".word 0xc4ae000c\n"  /* lwc1   $f14, 12($a1)         */
        ".word 0x48670004\n"  /* mfv    $a3, S100              */
        ".word 0x460e6300\n"  /* add.s  $f12, $f12, $f14       */
        ".word 0x44876800\n"  /* mtc1   $a3, $f13              */
        ".word 0x460c6302\n"  /* mul.s  $f12, $f12, $f12       */
        ".word 0x460c683c\n"  /* c.lt.s $f13, $f12             */
        ".word 0x00000000\n"  /* nop                           */
        ".word 0x4500001b\n"  /* bc1f   +27                    */
        ".word 0x34020001\n"  /* li     $v0, 1                 */
        ".word 0xa0c20034\n"  /* sb     $v0, 52($a2)           */
        ".word 0xd8860000\n"  /* lv.q   C120, 0($a0)          */
        ".word 0xf8c60000\n"  /* sv.q   C120, 0($a2)          */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0x60878606\n"  /* vsub.t C120, C120, C130       */
        ".word 0xf8c60020\n"  /* sv.q   C120, 0x20($a2)       */
        ".word 0x64868604\n"  /* vdot.t S100, C120, C120       */
        ".word 0x6c000408\n"  /* vcmp.s ez, S100               */
        ".word 0xd0110424\n"  /* vrsq.s S101, S100             */
        ".word 0xd0160444\n"  /* vsqrt.s S102, S100            */
        ".word 0xdc0010e5\n"  /* vpfxs  1, Y, Z, W            */
        ".word 0xd2a00424\n"  /* vcmovt.s S101, S100, 0       */
        ".word 0x65248606\n"  /* vscl.t C120, C120, S101       */
        ".word 0xf8c60020\n"  /* sv.q   C120, 0x20($a2)       */
        ".word 0xc4ae000c\n"  /* lwc1   $f14, 12($a1)         */
        ".word 0x44047000\n"  /* mfc1   $a0, $f14              */
        ".word 0x48e40004\n"  /* mtv    $a0, S100              */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0x65048606\n"  /* vscl.t C120, C120, S100       */
        ".word 0x60068706\n"  /* vadd.t C120, C130, C120       */
        ".word 0xf8c60010\n"  /* sv.q   C120, 0x10($a2)       */
        ".word 0x46006304\n"  /* sqrt.s $f12, $f12             */
        ".word 0x46006b44\n"  /* sqrt.s $f13, $f13             */
        ".word 0x460d6301\n"  /* sub.s  $f12, $f12, $f13       */
        ".word 0x03e00008\n"  /* jr     $ra                    */
        ".word 0xe4cc0030\n"  /* swc1   $f12, 48($a2)         */
    );
    return 0;
}

void nwConnection::UpdatePing(cTimeValue tv) {
    int adjusted;
    int delta;
    int pingValue;
    int maxPingVal;

    int seq = mSeqNumber;
    int ack = mLastAckTick;
    int offset = *(int *)0x0009F988;
    delta = (seq - ack) - offset;

    int *pVal = &pingValue;
    int *pMax = &maxPingVal;
    (void)&delta;

    int *pSrc;
    if (delta > 0) {
        adjusted = mPingSamples + delta;
        pSrc = &adjusted;
    } else {
        pSrc = &mPingSamples;
    }

    pingValue = *pSrc;
    maxPingVal = *(int *)0x0036C7FC;

    if (!(pingValue < maxPingVal)) {
        pVal = pMax;
    } else {
        pMax = pVal;
    }
    pingValue = *pMax;

    float alpha = 0.99f;
    if (delta > 0) {
        alpha = 0.97f;
    }

    mSmoothedPing = mSmoothedPing * alpha + *(float *)0x0036C800 * (float)pingValue * (1.0f - alpha);
}
