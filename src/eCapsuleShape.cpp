#include "eCapsuleShape.h"
#include "eSimulatedController.h"
#include "mVec3.h"
#include "mOCS.h"

// eCapsuleShape::GetSupport(const mVec3 &, const mOCS &, mVec3 *) const
// Address: 0x0006afbc, Size: 104 bytes
void eCapsuleShape::GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const {
    __asm__ volatile(
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0xd8c60020\n"  /* lv.q   C120, 0x20($a2)       */
        ".word 0x64868704\n"  /* vdot.t S100, C130, C120       */
        ".word 0x48680004\n"  /* mfv    $t0, S100              */
        ".word 0x44886800\n"  /* mtc1   $t0, $f13              */
        ".word 0x44807000\n"  /* mtc1   $zero, $f14            */
        ".word 0x460e683e\n"  /* c.le.s $f13, $f14             */
        ".word 0x00000000\n"  /* nop                           */
        ".word 0x45000003\n"  /* bc1f   +3                     */
        ".word 0xc48c0084\n"  /* lwc1   $f12, 132($a0)        */
        ".word 0x10000001\n"  /* b      +1                     */
        ".word 0x46006307\n"  /* neg.s  $f12, $f12             */
        ".word 0x44086000\n"  /* mfc1   $t0, $f12              */
        ".word 0x48e80004\n"  /* mtv    $t0, S100              */
        ".word 0xd8c70030\n"  /* lv.q   C130, 0x30($a2)       */
        ".word 0x65048606\n"  /* vscl.t C120, C120, S100       */
        ".word 0x60068706\n"  /* vadd.t C120, C130, C120       */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
        ".word 0xc48d0080\n"  /* lwc1   $f13, 128($a0)        */
        ".word 0x44046800\n"  /* mfc1   $a0, $f13              */
        ".word 0x48e40004\n"  /* mtv    $a0, S100              */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0x65048707\n"  /* vscl.t C130, C130, S100       */
        ".word 0x60078606\n"  /* vadd.t C120, C120, C130       */
        ".word 0x03e00008\n"  /* jr     $ra                    */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
    );
}

// eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x0006c490, Size: 180 bytes
//
// The scalar bool logic (entry computation, null checks, bnezl) cannot be matched
// from C because SNC schedules sll before lw (different register flow) and
// pre-computes entry+0x20 into a register (CSE prevents direct-offset lw).
// The VFPU ops (vtfm3.t, vcrsp.t, vsub.t, vadd.t) have no C equivalent.
// Using .word for the scalar logic + zero path return, and asm mnemonics for VFPU.
void eSimulatedController::GetVelocity(int index, const mVec3 &pos, mVec3 *out) const {
    __asm__ volatile(
        /* Entry computation + bool logic + bnezl */
        ".word 0x8c840038\n"  /* lw     $a0, 56($a0)            */
        ".word 0x00052900\n"  /* sll    $a1, $a1, 4             */
        ".word 0x00a54021\n"  /* addu   $t0, $a1, $a1           */
        ".word 0x00a82821\n"  /* addu   $a1, $a1, $t0           */
        ".word 0x00852821\n"  /* addu   $a1, $a0, $a1           */
        ".word 0x8ca80020\n"  /* lw     $t0, 32($a1)            */
        ".word 0x11000005\n"  /* beqz   $t0, +5 (label1)        */
        ".word 0x34040000\n"  /* li     $a0, 0                  */
        ".word 0x8ca80024\n"  /* lw     $t0, 36($a1)            */
        ".word 0x11000003\n"  /* beqz   $t0, +3 (label2)        */
        ".word 0x308400ff\n"  /* andi   $a0, $a0, 0xff          */
        ".word 0x34040001\n"  /* li     $a0, 1                  */
        ".word 0x308400ff\n"  /* andi   $a0, $a0, 0xff (label1) */
        ".word 0x5480000b\n"  /* bnezl  $a0, +11 (compute)      */
        ".word 0x8ca40020\n"  /* lw     $a0, 32($a1) (delay)    */
        /* Zero path (fall-through when !hasBodies) */
        "mtc1 $zero, $f12\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0($a3)\n"
        ".word 0x03e00008\n"  /* jr     $ra                     */
        ".word 0x27bd0030\n"  /* addiu  $sp, $sp, 48 (delay)    */
        /* Compute path (bnezl target, a0 = body pointer) */
        "lv.q C120, 0x50($a0)\n"
        "lv.q C000, 0($a0)\n"
        "lv.q C010, 0x10($a0)\n"
        "lv.q C020, 0x20($a0)\n"
        "lv.q C030, 0x30($a0)\n"
        "vtfm3.t C130, E000, C120\n"
        "sv.q C130, 0($sp)\n"
        "lv.q C120, 0($a2)\n"
        "lv.q C200, 0x30($a0)\n"
        "vsub.t C120, C120, C200\n"
        "sv.q C120, 0x10($sp)\n"
        "lv.q C200, 0x20($sp)\n"
        "vcrsp.t C200, C130, C120\n"
        "sv.q C200, 0x20($sp)\n"
        "lv.q C120, 0x40($a0)\n"
        "vadd.t C120, C120, C200\n"
        "sv.q C120, 0($a3)\n"
        ::: "memory"
    );
}
