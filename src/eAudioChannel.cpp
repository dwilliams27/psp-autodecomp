// eAudioChannel::CalcPanning(int, const mOCS *, float *) const
// Address: 0x0001d224, Size: 508B, Obj: eAll_psp.obj
//
// 3D audio panning: transforms listener position into each speaker's local
// space (via mOCS basis), computes distance attenuation, and projects onto
// 4 output channels (quadrant panning). Falls back to 0.25 per channel
// when not 3D or missing listener/speaker data.
//
// Heavy VFPU use (vsub.t, vtfm3.t, vdot.t, vsqrt.s) plus specific SNC
// scheduling with bc1fl (branch-likely) makes pure C matching infeasible.

#include "eAudioChannel.h"
#include "mOCS.h"

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fNeAudioChannelLCalcPanningiPC6EmOCSPfK\n"
    "__0fNeAudioChannelLCalcPanningiPC6EmOCSPfK:\n"
    "addiu $sp, $sp, -16\n"
    "lw $t2, 80($a0)\n"
    ".word 0x15400003\n"  // bnez t2, +3
    "andi $t0, $t2, 0xffff\n"
    ".word 0x1000000d\n"  // b +13                     — no config → NULL
    "ori $t1, $zero, 0x0\n"
    "lui $t1, 0x4\n"
    "sll $t0, $t0, 2\n"
    "addiu $t1, $t1, -30576\n"
    "addu $t0, $t0, $t1\n"
    "lw $t1, 0($t0)\n"
    ".word 0x11200005\n"  // beqz t1, +5               — NULL check
    "ori $t0, $zero, 0x0\n"
    "lw $t3, 48($t1)\n"
    ".word 0x556a0003\n"  // bnel t3, t2, +3           — handle mismatch?
    "or $t1, $t0, $zero\n"
    "or $t0, $t1, $zero\n"
    "or $t1, $t0, $zero\n"
    "lw $t2, 0($a0)\n"
    "andi $t2, $t2, 0x80\n"
    "sltu $t2, $zero, $t2\n"
    "andi $t2, $t2, 0xff\n"
    ".word 0x11400060\n"  // beqz t2, fallback         — not 3D
    "or $t0, $t1, $zero\n"
    ".word 0x5120005f\n"  // beqzl t1, fallback_likely — no speaker
    "lui $a0, 0x3e80\n"
    "lw $t1, 76($a0)\n"
    ".word 0x5120005c\n"  // beqzl t1, fallback_likely — no listener
    "lui $a0, 0x3e80\n"
    // — 3D panning loop setup —
    "mtc1 $zero, $f13\n"
    "ori $t1, $zero, 0x0\n"
    "swc1 $f13, 12($a3)\n"
    "swc1 $f13, 8($a3)\n"
    "swc1 $f13, 4($a3)\n"
    "slt $t2, $t1, $a1\n"
    ".word 0x11400059\n"  // beqz t2, done
    "swc1 $f13, 0($a3)\n"
    // — loop body —
    "ori $t2, $zero, 0x0\n"
    "addu $a2, $a2, $t2\n"
    "lui $t2, 0x3f80\n"
    "mtc1 $t2, $f12\n"
    "lw $t2, 76($a0)\n"
    "lv.q C120, 0x10($t2)\n"
    "lv.q C000, 0($a2)\n"
    "lv.q C010, 0x10($a2)\n"
    "lv.q C020, 0x20($a2)\n"
    "lv.q C030, 0x30($a2)\n"
    "vsub.t C120, C120, C030\n"
    "vtfm3.t C130, M000, C120\n"
    "sv.q C130, 0($sp)\n"
    "lwc1 $f16, 88($t0)\n"
    "lwc1 $f14, 84($t0)\n"
    "vdot.t S100, C130, C130\n"
    "vsqrt.s S100, S100\n"
    "mfv $t2, S100\n"
    "mtc1 $t2, $f15\n"
    "c.le.s $f15, $f14\n"
    "nop\n"
    ".word 0x45020037\n"  // bc1fl loop_end            — if dist > minDist, skip
    "addiu $t1, $t1, 1\n"
    // — distance attenuation —
    "sub.s $f15, $f14, $f15\n"
    "sub.s $f16, $f14, $f16\n"
    "div.s $f15, $f15, $f16\n"
    "c.le.s $f15, $f13\n"
    "nop\n"
    ".word 0x45020003\n"  // bc1fl clamp_hi
    "mov.s $f18, $f15\n"
    ".word 0x10000005\n"  // b after_clamp             — f18 = 0.0
    "mov.s $f18, $f13\n"
    "c.lt.s $f18, $f12\n"
    "nop\n"
    ".word 0x45020001\n"  // bc1fl after_clamp
    "mov.s $f18, $f12\n"
    // — quadrant panning computation —
    "add.s $f15, $f14, $f14\n"
    "lwc1 $f0, 4($sp)\n"
    "mul.s $f15, $f15, $f15\n"
    "div.s $f17, $f12, $f15\n"
    "add.s $f2, $f0, $f14\n"
    "lwc1 $f15, 0($sp)\n"
    "mul.s $f17, $f18, $f17\n"
    "sub.s $f16, $f14, $f15\n"
    "lwc1 $f19, 0($a3)\n"
    "mul.s $f18, $f17, $f2\n"
    "add.s $f15, $f15, $f14\n"
    "sub.s $f14, $f14, $f0\n"
    "mul.s $f14, $f17, $f14\n"
    "mul.s $f17, $f18, $f16\n"
    "c.le.s $f19, $f17\n"
    "nop\n"
    ".word 0x45020001\n"  // bc1fl skip0
    "mov.s $f17, $f19\n"
    "mul.s $f18, $f18, $f15\n"
    "swc1 $f17, 0($a3)\n"
    "lwc1 $f17, 4($a3)\n"
    "c.le.s $f17, $f18\n"
    "nop\n"
    ".word 0x45020001\n"  // bc1fl skip1
    "mov.s $f18, $f17\n"
    "mul.s $f16, $f14, $f16\n"
    "lwc1 $f17, 8($a3)\n"
    "swc1 $f18, 4($a3)\n"
    "c.le.s $f17, $f16\n"
    "nop\n"
    ".word 0x45020001\n"  // bc1fl skip2
    "mov.s $f16, $f17\n"
    "mul.s $f15, $f14, $f15\n"
    "swc1 $f16, 8($a3)\n"
    "lwc1 $f14, 12($a3)\n"
    "c.le.s $f14, $f15\n"
    "nop\n"
    ".word 0x45020001\n"  // bc1fl skip3
    "mov.s $f15, $f14\n"
    "swc1 $f15, 12($a3)\n"
    "addiu $t1, $t1, 1\n"
    // — loop back —
    "slt $t2, $t1, $a1\n"
    ".word 0x1540ffb5\n"  // bnez t2, loop_top
    "addiu $a2, $a2, 64\n"
    ".word 0x10000007\n"  // b done
    "nop\n"
    // — fallback: 0.25 per channel —
    "lui $a0, 0x3e80\n"
    "mtc1 $a0, $f12\n"
    "swc1 $f12, 12($a3)\n"
    "swc1 $f12, 8($a3)\n"
    "swc1 $f12, 4($a3)\n"
    "swc1 $f12, 0($a3)\n"
    // — return —
    "jr $ra\n"
    "addiu $sp, $sp, 16\n"
    ".set reorder\n"
);
