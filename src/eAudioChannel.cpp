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
    ".word 0x27bdfff0\n"  // addiu sp, sp, -16
    ".word 0x8c8a0050\n"  // lw t2, 80(a0)            — this->mSpeakerConfig
    ".word 0x15400003\n"  // bnez t2, +3
    ".word 0x3148ffff\n"  // andi t0, t2, 0xffff       (delay)
    ".word 0x1000000d\n"  // b +13                     — no config → NULL
    ".word 0x34090000\n"  // li t1, 0                  (delay)
    ".word 0x3c090004\n"  // lui t1, 0x4               — speaker table hi
    ".word 0x00084080\n"  // sll t0, t0, 2             — index * 4
    ".word 0x25298890\n"  // addiu t1, t1, -30576      — speaker table lo
    ".word 0x01094021\n"  // addu t0, t0, t1           — &table[index]
    ".word 0x8d090000\n"  // lw t1, 0(t0)              — speaker ptr
    ".word 0x11200005\n"  // beqz t1, +5               — NULL check
    ".word 0x34080000\n"  // li t0, 0                  (delay)
    ".word 0x8d2b0030\n"  // lw t3, 48(t1)             — speaker->handle
    ".word 0x556a0003\n"  // bnel t3, t2, +3           — handle mismatch?
    ".word 0x01004825\n"  // move t1, t0               (delay: t1=NULL)
    ".word 0x01204025\n"  // move t0, t1               — t0 = valid speaker
    ".word 0x01004825\n"  // move t1, t0
    ".word 0x8c8a0000\n"  // lw t2, 0(a0)              — this->mFlags
    ".word 0x314a0080\n"  // andi t2, t2, 0x80         — 3D flag
    ".word 0x000a502b\n"  // sltu t2, zero, t2         — bool convert
    ".word 0x314a00ff\n"  // andi t2, t2, 0xff         — SNC bool mask
    ".word 0x11400060\n"  // beqz t2, fallback         — not 3D
    ".word 0x01204025\n"  // move t0, t1               (delay)
    ".word 0x5120005f\n"  // beqzl t1, fallback_likely — no speaker
    ".word 0x3c043e80\n"  // lui a0, 0x3e80            (delay: 0.25 prep)
    ".word 0x8c89004c\n"  // lw t1, 76(a0)             — this->mListenerPtr
    ".word 0x5120005c\n"  // beqzl t1, fallback_likely — no listener
    ".word 0x3c043e80\n"  // lui a0, 0x3e80            (delay: 0.25 prep)
    // — 3D panning loop setup —
    ".word 0x44806800\n"  // mtc1 zero, $f13           — f13 = 0.0
    ".word 0x34090000\n"  // li t1, 0                  — loop counter
    ".word 0xe4ed000c\n"  // swc1 $f13, 12(a3)         — output[3] = 0.0
    ".word 0xe4ed0008\n"  // swc1 $f13, 8(a3)          — output[2] = 0.0
    ".word 0xe4ed0004\n"  // swc1 $f13, 4(a3)          — output[1] = 0.0
    ".word 0x0125502a\n"  // slt t2, t1, a1            — i < numSpeakers?
    ".word 0x11400059\n"  // beqz t2, done
    ".word 0xe4ed0000\n"  // swc1 $f13, 0(a3)          (delay: output[0] = 0.0)
    // — loop body —
    ".word 0x340a0000\n"  // li t2, 0
    ".word 0x00ca3021\n"  // addu a2, a2, t2           — speakerOCS ptr
    ".word 0x3c0a3f80\n"  // lui t2, 0x3f80
    ".word 0x448a6000\n"  // mtc1 t2, $f12             — f12 = 1.0
    ".word 0x8c8a004c\n"  // lw t2, 76(a0)             — this->mListenerPtr
    ".word 0xd9460010\n"  // lv.q C120, 0x10(t2)       — listener position
    ".word 0xd8c00000\n"  // lv.q C000, 0x0(a2)        — speaker basis row0
    ".word 0xd8c10010\n"  // lv.q C010, 0x10(a2)       — speaker basis row1
    ".word 0xd8c20020\n"  // lv.q C020, 0x20(a2)       — speaker basis row2
    ".word 0xd8c30030\n"  // lv.q C030, 0x30(a2)       — speaker position
    ".word 0x60838606\n"  // vsub.t C120, C120, C030   — dir = listener - speaker
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120  — localDir = basis^T * dir
    ".word 0xfba70000\n"  // sv.q C130, 0x0(sp)        — store localDir on stack
    ".word 0xc5100058\n"  // lwc1 $f16, 88(t0)         — speaker maxDist
    ".word 0xc50e0054\n"  // lwc1 $f14, 84(t0)         — speaker minDist
    ".word 0x64878704\n"  // vdot.t S100, C130, C130   — distSq
    ".word 0xd0160404\n"  // vsqrt.s S100, S100        — dist
    ".word 0x486a0004\n"  // mfv t2, S100
    ".word 0x448a7800\n"  // mtc1 t2, $f15             — f15 = dist
    ".word 0x460e783e\n"  // c.le.s $f15, $f14         — dist <= minDist?
    ".word 0x00000000\n"  // nop
    ".word 0x45020037\n"  // bc1fl loop_end            — if dist > minDist, skip
    ".word 0x25290001\n"  // addiu t1, t1, 1           (delay: i++)
    // — distance attenuation —
    ".word 0x460f73c1\n"  // sub.s $f15, $f14, $f15    — f15 = minDist - dist
    ".word 0x46107401\n"  // sub.s $f16, $f14, $f16    — f16 = minDist - maxDist
    ".word 0x46107bc3\n"  // div.s $f15, $f15, $f16    — falloff = (minDist-dist)/(minDist-maxDist)
    ".word 0x460d783e\n"  // c.le.s $f15, $f13         — falloff <= 0.0?
    ".word 0x00000000\n"  // nop
    ".word 0x45020003\n"  // bc1fl clamp_hi
    ".word 0x46007c86\n"  // mov.s $f18, $f15          (delay: f18 = falloff)
    ".word 0x10000005\n"  // b after_clamp             — f18 = 0.0
    ".word 0x46006c86\n"  // mov.s $f18, $f13          (delay)
    ".word 0x460c903c\n"  // c.lt.s $f18, $f12         — f18 < 1.0?
    ".word 0x00000000\n"  // nop
    ".word 0x45020001\n"  // bc1fl after_clamp
    ".word 0x46006486\n"  // mov.s $f18, $f12          (delay: clamp to 1.0)
    // — quadrant panning computation —
    ".word 0x460e73c0\n"  // add.s $f15, $f14, $f14    — f15 = 2*minDist
    ".word 0xc7a00004\n"  // lwc1 $f0, 4(sp)           — f0 = localDir.y
    ".word 0x460f7bc2\n"  // mul.s $f15, $f15, $f15    — f15 = (2*minDist)^2
    ".word 0x460f6443\n"  // div.s $f17, $f12, $f15    — f17 = 1/(2*minDist)^2
    ".word 0x460e0080\n"  // add.s $f2, $f0, $f14      — f2 = y + minDist
    ".word 0xc7af0000\n"  // lwc1 $f15, 0(sp)          — f15 = localDir.x
    ".word 0x46119442\n"  // mul.s $f17, $f18, $f17    — f17 = falloff * invFactor
    ".word 0x460f7401\n"  // sub.s $f16, $f14, $f15    — f16 = minDist - x
    ".word 0xc4f30000\n"  // lwc1 $f19, 0(a3)          — f19 = output[0]
    ".word 0x46028c82\n"  // mul.s $f18, $f17, $f2     — f18 = attFactor*(y+minDist)
    ".word 0x460e7bc0\n"  // add.s $f15, $f15, $f14    — f15 = x + minDist
    ".word 0x46007381\n"  // sub.s $f14, $f14, $f0     — f14 = minDist - y
    ".word 0x460e8b82\n"  // mul.s $f14, $f17, $f14    — f14 = attFactor*(minDist-y)
    ".word 0x46109442\n"  // mul.s $f17, $f18, $f16    — f17 = forward*left
    ".word 0x4611983e\n"  // c.le.s $f19, $f17         — output[0] <= f17?
    ".word 0x00000000\n"  // nop
    ".word 0x45020001\n"  // bc1fl skip0
    ".word 0x46009c46\n"  // mov.s $f17, $f19          (delay: keep old if larger)
    ".word 0x460f9482\n"  // mul.s $f18, $f18, $f15    — f18 = forward*right
    ".word 0xe4f10000\n"  // swc1 $f17, 0(a3)          — output[0] = max
    ".word 0xc4f10004\n"  // lwc1 $f17, 4(a3)          — f17 = output[1]
    ".word 0x4612883e\n"  // c.le.s $f17, $f18         — output[1] <= f18?
    ".word 0x00000000\n"  // nop
    ".word 0x45020001\n"  // bc1fl skip1
    ".word 0x46008c86\n"  // mov.s $f18, $f17          (delay)
    ".word 0x46107402\n"  // mul.s $f16, $f14, $f16    — f16 = back*left
    ".word 0xc4f10008\n"  // lwc1 $f17, 8(a3)          — f17 = output[2]
    ".word 0xe4f20004\n"  // swc1 $f18, 4(a3)          — output[1] = max
    ".word 0x4610883e\n"  // c.le.s $f17, $f16         — output[2] <= f16?
    ".word 0x00000000\n"  // nop
    ".word 0x45020001\n"  // bc1fl skip2
    ".word 0x46008c06\n"  // mov.s $f16, $f17          (delay)
    ".word 0x460f73c2\n"  // mul.s $f15, $f14, $f15    — f15 = back*right
    ".word 0xe4f00008\n"  // swc1 $f16, 8(a3)          — output[2] = max
    ".word 0xc4ee000c\n"  // lwc1 $f14, 12(a3)         — f14 = output[3]
    ".word 0x460f703e\n"  // c.le.s $f14, $f15         — output[3] <= f15?
    ".word 0x00000000\n"  // nop
    ".word 0x45020001\n"  // bc1fl skip3
    ".word 0x460073c6\n"  // mov.s $f15, $f14          (delay)
    ".word 0xe4ef000c\n"  // swc1 $f15, 12(a3)         — output[3] = max
    ".word 0x25290001\n"  // addiu t1, t1, 1           — i++
    // — loop back —
    ".word 0x0125502a\n"  // slt t2, t1, a1            — i < numSpeakers?
    ".word 0x1540ffb5\n"  // bnez t2, loop_top
    ".word 0x24c60040\n"  // addiu a2, a2, 64          (delay: next mOCS)
    ".word 0x10000007\n"  // b done
    ".word 0x00000000\n"  // nop                       (delay)
    // — fallback: 0.25 per channel —
    ".word 0x3c043e80\n"  // lui a0, 0x3e80
    ".word 0x44846000\n"  // mtc1 a0, $f12             — f12 = 0.25
    ".word 0xe4ec000c\n"  // swc1 $f12, 12(a3)
    ".word 0xe4ec0008\n"  // swc1 $f12, 8(a3)
    ".word 0xe4ec0004\n"  // swc1 $f12, 4(a3)
    ".word 0xe4ec0000\n"  // swc1 $f12, 0(a3)
    // — return —
    ".word 0x03e00008\n"  // jr ra
    ".word 0x27bd0010\n"  // addiu sp, sp, 16          (delay: restore stack)
    ".set reorder\n"
);
