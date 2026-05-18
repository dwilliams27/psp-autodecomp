# Battle Packet: `eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue)`

## Target

- Address: `0x0001d6f0`
- Size: `216` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eAudioGroup`
- Method family: `StopSound`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 0B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `0`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `0`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0001e3f4` `untried` 408B `eAudio::StopSound(int, cTimeValue) static`

Callers:
- `0x0001e58c` `untried` 188B `eAudio::StopSound(cHandleT<eSoundData>, cTimeValue) static`

## Placement

- Canonical source: `src/eAudioGroup.cpp`
- Header(s): `include/eAudioGroup.h`
- Allowed source(s): `src/eAudioGroup.cpp`
- Split-TU prefix: `src/eAudioGroup_*.cpp`

## Class Header: `include/eAudioGroup.h`

```cpp
#pragma once

#include "eAudioFader.h"

class cTimeValue;
class mOCS;
template <class T> class cHandleT;
class eSoundData;

// eAudioGroup: owns 3 eAudioFaders and spans a range of channel indices.
// +0x00: int   mChannelStart   first channel index (init 0)
// +0x04: int   mChannelEnd     last channel index  (init -1)
// +0x08: eAudioFader mFaders[3]  (3*16 = 48 bytes)  last fader ends at 0x38
struct eAudioGroup {
    int mChannelStart;          // 0x00
    int mChannelEnd;            // 0x04
    eAudioFader mFaders[3];     // 0x08..0x37

    eAudioGroup();
    float GetVolume() const;
    void ResetUserFaders();
    void StopSound(cHandleT<eSoundData> handle, cTimeValue dt);
    void Update(cTimeValue dt, int arg2, const mOCS *arg3);
};
```

## Matched Same-Class Neighbors

- `0x0001d6c0` 48B `eAudioGroup::GetVolume(void) const` — `src/eAudioGroup.cpp`
- `0x001dfd04` 72B `eAudioGroup::eAudioGroup(void)` — `src/eAudioGroup.cpp`

## Prior Failure Notes

### Note 1

- session `63d6da20`

Current source models the loop and call correctly and now emits the expected byte-mask/branch-likely shape for both tests: active is compiled as andi/sltu/andi followed by beqzl, and the handle compare is compiled as xor/sltiu/andi followed by beqzl. The best manual/permuter variant is 27/216 bytes different. The remaining mismatch is register allocation in the channel offset/global-base setup: SNC emits the offset accumulator in t2 and the g_eAudio_channels base load through t1, while expected uses t1 for the offset accumulator and t2 for the lui/base register; this also changes the saved offset stack slot/load from t2 instead of t1. I tried bool conditions, explicit int byte masks, branch tests with and without extra &0xFF, cHandle/cTimeValue raw integer locals, explicit offset expression `(idx << 6) + ((idx * 3) << 3)`, direct `(int)ch + 0x50` load, and named register hints for t1/t2, but SNC either ignored the t-register hints or optimized the live-range changes away.

The permuter ran for 300 seconds from a 33-byte baseline, generated 3960 candidates and compiled 2602; it found one improvement to 27 differing bytes using the `(int)ch + 0x50` cast, but did not match. The remaining difference is not the earlier missing-andi issue anymore; it is a narrow register assignment difference around the offset/global-base pair.

### Note 2

- session `55d92a7d`

eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue). Function structure (iterate channels in [mChannelStart, mChannelEnd], check active flag (word0 & 3) != 0, compare handle vs ch[0x50], call eAudio::StopSound on match) is correct and compiles to 208 bytes vs expected 216. The 8-byte gap is specifically two missing 'andi X, X, 0xff' mask instructions that follow the 'sltu X, zero, X' bool-extraction pattern. Additionally the branches use beqz/bnez while expected uses the branch-likely variants beqzl/bnezl. This is precisely the pattern documented in docs/decisions/006-bnel-compiler-divergence.md. I tried: 'bool active = ((*(int*)ch & 3) != 0);', 'unsigned char active = (unsigned char)(...)', splitting into a named temporary, and using an explicit cast chain. The 'unsigned char' variant added the sltu but never the andi 0xff; SNC seems to only emit the full sltu+andi+bnel sequence when specific downstream uses require a strict 0/1 byte value (e.g., storing to a bool member or passing to a byte-typed parameter). Because the bool is only consumed by a branch, SNC optimizes to sltu+beqz. The andi+beqzl lives in the original likely because the bool was stored to or passed through something typed as unsigned char in a helper signature we do not have. Per the strict unmatchable criteria in CLAUDE.md, this DOES match (diff <=8 bytes, differing instructions are exactly bnel-family vs bne+nop and the preceding andi byte-cast), but permuter was not run for the full >=5 minutes required by criterion (2), so classifying as 'failed' rather than 'unmatchable_bnel'. Recommend permuter rerun and/or revisiting once helper signatures around eAudioChannel/eAudio::StopSound are known.


## Disassembly

```asm
1d6f0:	27bdffe0 	addiu	sp,sp,-32
1d6f4:	8c870000 	lw	a3,0(a0)
1d6f8:	afa50000 	sw	a1,0(sp)
1d6fc:	afa60004 	sw	a2,4(sp)
1d700:	8c860004 	lw	a2,4(a0)
1d704:	00e02825 	move	a1,a3
1d708:	00c5302a 	slt	a2,a2,a1
1d70c:	afbf001c 	sw	ra,28(sp)
1d710:	14c0002a 	bnez	a2,0x1d7bc
1d714:	00a54021 	addu	t0,a1,a1
1d718:	00a84021 	addu	t0,a1,t0
1d71c:	8fa70000 	lw	a3,0(sp)
1d720:	00054980 	sll	t1,a1,0x6
1d724:	000840c0 	sll	t0,t0,0x3
1d728:	8fa60004 	lw	a2,4(sp)
1d72c:	01284821 	addu	t1,t1,t0
1d730:	3c0a0038 	lui	t2,0x38
1d734:	8d48d0d4 	lw	t0,-12076(t2)
1d738:	01094021 	addu	t0,t0,t1
1d73c:	8d0b0000 	lw	t3,0(t0)
1d740:	316b0003 	andi	t3,t3,0x3
1d744:	000b582b 	sltu	t3,zero,t3
1d748:	316b00ff 	andi	t3,t3,0xff
1d74c:	51600017 	beqzl	t3,0x1d7ac
1d750:	8c880004 	lw	t0,4(a0)
1d754:	8d080050 	lw	t0,80(t0)
1d758:	00e84026 	xor	t0,a3,t0
1d75c:	2d080001 	sltiu	t0,t0,1
1d760:	310800ff 	andi	t0,t0,0xff
1d764:	51000011 	beqzl	t0,0x1d7ac
1d768:	8c880004 	lw	t0,4(a0)
1d76c:	afa50018 	sw	a1,24(sp)
1d770:	afa90014 	sw	t1,20(sp)
1d774:	afa70010 	sw	a3,16(sp)
1d778:	afa6000c 	sw	a2,12(sp)
1d77c:	00a04025 	move	t0,a1
1d780:	afa40008 	sw	a0,8(sp)
1d784:	00c02825 	move	a1,a2
1d788:	0c0078fd 	jal	0x1e3f4
1d78c:	01002025 	move	a0,t0
1d790:	8fa40008 	lw	a0,8(sp)
1d794:	8fa6000c 	lw	a2,12(sp)
1d798:	8fa70010 	lw	a3,16(sp)
1d79c:	8fa90014 	lw	t1,20(sp)
1d7a0:	8fa50018 	lw	a1,24(sp)
1d7a4:	3c0a0038 	lui	t2,0x38
1d7a8:	8c880004 	lw	t0,4(a0)
1d7ac:	24a50001 	addiu	a1,a1,1
1d7b0:	0105402a 	slt	t0,t0,a1
1d7b4:	1100ffdf 	beqz	t0,0x1d734
1d7b8:	25290058 	addiu	t1,t1,88
1d7bc:	8fbf001c 	lw	ra,28(sp)
1d7c0:	03e00008 	jr	ra
1d7c4:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue)
// Address: 0x0001d6f0, Size: 216B
// Obj: eAll_psp.obj

? eAudio__StopSound_int_cTimeValuestatic(s32, s32, s32, s32); /* extern */

void eAudioGroup__StopSound_cHandleT_eSoundData__cTimeValue(void *arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t1;
    void *temp_t0;
    void *var_a0;

    var_a0 = arg0;
    var_a1 = var_a0->unk0;
    if (var_a0->unk4 >= var_a1) {
        var_a3 = subroutine_arg0;
        var_a2 = subroutine_arg1;
        var_t1 = var_a1 * 0x58;
        do {
            temp_t0 = *(s32 *)0x37D0D4 + var_t1;
            if ((temp_t0->unk0 & 3 & 0xFF) && ((var_a3 == temp_t0->unk50) & 0xFF)) {
                sp18 = var_a1;
                sp14 = var_t1;
                sp10 = var_a3;
                eAudio__StopSound_int_cTimeValuestatic(var_a1, var_a2, var_a2, var_a3);
                var_a0 = subroutine_arg2;
                var_a2 = subroutine_arg3;
            }
            var_a1 += 1;
            var_t1 += 0x58;
        } while (var_a0->unk4 >= var_a1);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
