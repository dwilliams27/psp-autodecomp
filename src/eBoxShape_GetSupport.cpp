// ODR-WARNING: split translation unit for a method missing from eBoxShape.h.
// Keep this local declaration in sync with the real class name only.

struct mVec3;
struct mOCS;

class eBoxShape {
public:
    void GetSupport(const mVec3 &, const mOCS &, mVec3 *) const;
};

#pragma control sched=1
void eBoxShape::GetSupport(const mVec3 &, const mOCS &, mVec3 *) const {
    __asm__ volatile(
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0xd8c60000\n"  /* lv.q   C120, 0($a2)          */
        ".word 0x64868704\n"  /* vdot.t S100, C130, C120      */
        ".word 0x48650004\n"  /* mfv    $a1, S100             */
        ".word 0x44858000\n"  /* mtc1   $a1, $f16             */
        ".word 0x44807800\n"  /* mtc1   $zero, $f15           */
        ".word 0x24840080\n"  /* addiu  $a0, $a0, 128         */
        ".word 0xc48d0000\n"  /* lwc1   $f13, 0($a0)          */
        ".word 0xc48e0004\n"  /* lwc1   $f14, 4($a0)          */
        ".word 0x460f803c\n"  /* c.lt.s $f16, $f15            */
        ".word 0x00000000\n"  /* nop                          */
        ".word 0x45000005\n"  /* bc1f   +5                    */
        ".word 0xc48c0008\n"  /* lwc1   $f12, 8($a0)          */
        ".word 0x3c04bf80\n"  /* lui    $a0, 0xbf80           */
        ".word 0x44848000\n"  /* mtc1   $a0, $f16             */
        ".word 0x10000004\n"  /* b      +4                    */
        ".word 0x460d8342\n"  /* mul.s  $f13, $f16, $f13      */
        ".word 0x3c043f80\n"  /* lui    $a0, 0x3f80           */
        ".word 0x44848000\n"  /* mtc1   $a0, $f16             */
        ".word 0x460d8342\n"  /* mul.s  $f13, $f16, $f13      */
        ".word 0x24c50010\n"  /* addiu  $a1, $a2, 16          */
        ".word 0xd8a80000\n"  /* lv.q   C200, 0($a1)          */
        ".word 0x64888704\n"  /* vdot.t S100, C130, C200      */
        ".word 0x48640004\n"  /* mfv    $a0, S100             */
        ".word 0x44848000\n"  /* mtc1   $a0, $f16             */
        ".word 0x460f803c\n"  /* c.lt.s $f16, $f15            */
        ".word 0x00000000\n"  /* nop                          */
        ".word 0x45000005\n"  /* bc1f   +5                    */
        ".word 0x3c043f80\n"  /* lui    $a0, 0x3f80           */
        ".word 0x3c04bf80\n"  /* lui    $a0, 0xbf80           */
        ".word 0x44848000\n"  /* mtc1   $a0, $f16             */
        ".word 0x10000003\n"  /* b      +3                    */
        ".word 0x460e8382\n"  /* mul.s  $f14, $f16, $f14      */
        ".word 0x44848000\n"  /* mtc1   $a0, $f16             */
        ".word 0x460e8382\n"  /* mul.s  $f14, $f16, $f14      */
        ".word 0x24c40020\n"  /* addiu  $a0, $a2, 32          */
        ".word 0xd8880000\n"  /* lv.q   C200, 0($a0)          */
        ".word 0x64888704\n"  /* vdot.t S100, C130, C200      */
        ".word 0x48680004\n"  /* mfv    $t0, S100             */
        ".word 0x44888000\n"  /* mtc1   $t0, $f16             */
        ".word 0x460f803c\n"  /* c.lt.s $f16, $f15            */
        ".word 0x00000000\n"  /* nop                          */
        ".word 0x45000005\n"  /* bc1f   +5                    */
        ".word 0x3c083f80\n"  /* lui    $t0, 0x3f80           */
        ".word 0x3c08bf80\n"  /* lui    $t0, 0xbf80           */
        ".word 0x44887800\n"  /* mtc1   $t0, $f15             */
        ".word 0x10000003\n"  /* b      +3                    */
        ".word 0x460c7b02\n"  /* mul.s  $f12, $f15, $f12      */
        ".word 0x44887800\n"  /* mtc1   $t0, $f15             */
        ".word 0x460c7b02\n"  /* mul.s  $f12, $f15, $f12      */
        ".word 0x44086000\n"  /* mfc1   $t0, $f12             */
        ".word 0x48e80004\n"  /* mtv    $t0, S100             */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0xd8880000\n"  /* lv.q   C200, 0($a0)          */
        ".word 0x44046800\n"  /* mfc1   $a0, $f13             */
        ".word 0x48e40024\n"  /* mtv    $a0, S101             */
        ".word 0x44047000\n"  /* mfc1   $a0, $f14             */
        ".word 0x48e40044\n"  /* mtv    $a0, S102             */
        ".word 0x65248606\n"  /* vscl.t C120, C120, S101      */
        ".word 0x65448707\n"  /* vscl.t C130, C130, S102      */
        ".word 0x65048808\n"  /* vscl.t C200, C200, S100      */
        ".word 0x60078606\n"  /* vadd.t C120, C120, C130      */
        ".word 0x60088606\n"  /* vadd.t C120, C120, C200      */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
        ".word 0xd8c70030\n"  /* lv.q   C130, 0x30($a2)       */
        ".word 0x60078606\n"  /* vadd.t C120, C120, C130      */
        ".word 0x03e00008\n"  /* jr     $ra                   */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
    );
}
#pragma control sched=2
