# Battle Packet: `eAudioGroup::Update(cTimeValue, int, const mOCS *)`

## Target

- Address: `0x0001d4a8`
- Size: `248` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eAudioGroup`
- Method family: `Update`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0001ce74` `matched` 196B `eAudioFader::Update(cTimeValue)`
- `0x0001d6c0` `matched` 48B `eAudioGroup::GetVolume(void) const`
- `0x00091674` `untried` 1740B `eAudioChannel::Update(cTimeValue, float, int, const mOCS *)`

Callers:
- `0x0001d818` `untried` 480B `eAudio::Update(cTimeValue, int, const mOCS *, eWorld *) static`

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

## Matched Method Exemplars

### Exemplar 1: `eAudioFader::Update(cTimeValue)`

- Address: `0x0001ce74`
- Size: `196` bytes
- Source: `src/eAudioFader.cpp`

```cpp
int eAudioFader::Update(cTimeValue dt) {
    if (!mActive) return 0;

    int duration = mDuration;
    float target = mTarget;

    if (duration == 0) {
        mCurrent = target;
        mActive = 0;
        return 1;
    }

    float ratio = (float)dt.mTime / (float)duration;
    float current = mCurrent;

    if (duration < 0) {
        float result = current + (1.0f - target) * ratio;
        mCurrent = result;
        if (result <= target) {
            mCurrent = target;
            mActive = 0;
            return 1;
        }
    } else {
        float delta = target;
        float result = current + (delta * ratio);
        mCurrent = result;
        if (!(result < target)) {
            mCurrent = target;
            mActive = 0;
            return 1;
        }
    }
    return 0;
}
```

### Exemplar 2: `eDynamicFluid::Update(cTimeValue)`

- Address: `0x0005de70`
- Size: `60` bytes
- Source: `src/eDynamicFluid.cpp`

```cpp
// eDynamicFluid::Update(cTimeValue) — 0x0005DE70
void eDynamicFluid::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}
```

### Exemplar 3: `eProjector::Update(cTimeValue)`

- Address: `0x0007d4f4`
- Size: `60` bytes
- Source: `src/eProjector.cpp`

```cpp
void eProjector::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}
```

### Exemplar 4: `eShadow::Update(cTimeValue)`

- Address: `0x0007ed24`
- Size: `60` bytes
- Source: `src/eShadow.cpp`

```cpp
// eShadow::Update(cTimeValue)
void eShadow::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}
```


## Prior Failure Notes

### Note 1

- session `63d6da20`

The function body is structurally correct: three fader Update calls, volume = g_eAudio_masterVolume * GetVolume(), then the channel Update loop. The current compiled output remains 13/248 bytes different. The visible mismatch is the same allocator/scheduling area noted by the previous session: expected loads `s3` from 0(sp) before setting up the fader pointer and, in the channel loop, wants the offset accumulator/base registers in the opposite assignment depending on source shape. I tried pointer-increment and char-pointer fader loop forms, reusing the pointer as an offset carrier, a named `$s4` register local for the channel offset, explicit dt locals/raw integer ABI calls, and source order changes around idx/offs increments. Some variants swapped the second-loop registers but introduced other scheduling/stack differences; none reached a match.

The permuter ran for 300 seconds from the 13-byte baseline, generated 8832 candidates and compiled 5766 with zero improvements. This appears to be a last-mile SNC scheduling/register-allocation heuristic issue rather than missing control flow.

### Note 2

- session `55d92a7d`

eAudioGroup::Update(cTimeValue, int, const mOCS*). Function body is essentially correct (first loop invokes mFaders[i].Update(dt) 3x, then computes volume = s_fMasterVolume * GetVolume(), then iterates channels from mChannelStart..mChannelEnd calling eAudioChannel::Update). First loop matches; call to GetVolume and mul.s against s_fMasterVolume matches. Remaining 13-byte diff is entirely in the second channel loop: the expected code uses s4 for the byte-offset accumulator and s5 for the base-address lui of g_eAudio_channels, while SNC produces the opposite assignment (s5 for offset, s4 for base). This is a pure register-allocation heuristic difference. Tried: (1) hoisting 'int offs = idx * 0x58' declaration outside the if block; (2) computing 'char *ch = g_eAudio_channels + offs' inside the loop; (3) using a pointer-increment pattern (ch += 0x58) instead of offs += 0x58; (4) re-ordering idx++ and offs++ statements. Permuter (tools/permuter.py) ran for 180s, compiling 4662 candidates at 25.8 cand/sec, zero improvements from 13 bytes. No source-level restructuring reaches the expected register assignment. Suspect SNC's live-range splitting picks the register based on which variable is live-across the call to eAudioChannel::Update in the original (possibly due to a slightly different declaration order in the original source we cannot recover). Recommend revisiting with updated permuter heuristics or when neighboring callers in eAll_psp.obj are matched and reveal the original source layout.


## Disassembly

```asm
1d4a8:	27bdffd0 	addiu	sp,sp,-48
1d4ac:	afa50000 	sw	a1,0(sp)
1d4b0:	afb00008 	sw	s0,8(sp)
1d4b4:	afb1000c 	sw	s1,12(sp)
1d4b8:	afb20010 	sw	s2,16(sp)
1d4bc:	afb30014 	sw	s3,20(sp)
1d4c0:	afb40018 	sw	s4,24(sp)
1d4c4:	afb5001c 	sw	s5,28(sp)
1d4c8:	34150000 	li	s5,0x0
1d4cc:	8fb30000 	lw	s3,0(sp)
1d4d0:	24940008 	addiu	s4,a0,8
1d4d4:	00809025 	move	s2,a0
1d4d8:	00c08825 	move	s1,a2
1d4dc:	00e08025 	move	s0,a3
1d4e0:	e7b40004 	swc1	$f20,4(sp)
1d4e4:	afb60020 	sw	s6,32(sp)
1d4e8:	afbf0024 	sw	ra,36(sp)
1d4ec:	02802025 	move	a0,s4
1d4f0:	0c00739d 	jal	0x1ce74
1d4f4:	02602825 	move	a1,s3
1d4f8:	26b50001 	addiu	s5,s5,1
1d4fc:	2aa40003 	slti	a0,s5,3
1d500:	1480fffa 	bnez	a0,0x1d4ec
1d504:	26940010 	addiu	s4,s4,16
1d508:	0c0075b0 	jal	0x1d6c0
1d50c:	02402025 	move	a0,s2
1d510:	3c040038 	lui	a0,0x38
1d514:	c494d0cc 	lwc1	$f20,-12084(a0)
1d518:	8e560000 	lw	s6,0(s2)
1d51c:	8e440004 	lw	a0,4(s2)
1d520:	0096202a 	slt	a0,a0,s6
1d524:	14800013 	bnez	a0,0x1d574
1d528:	4600a502 	mul.s	$f20,$f20,$f0
1d52c:	02d62021 	addu	a0,s6,s6
1d530:	02c42021 	addu	a0,s6,a0
1d534:	0016a180 	sll	s4,s6,0x6
1d538:	000420c0 	sll	a0,a0,0x3
1d53c:	0284a021 	addu	s4,s4,a0
1d540:	3c150038 	lui	s5,0x38
1d544:	8ea4d0d4 	lw	a0,-12076(s5)
1d548:	4600a306 	mov.s	$f12,$f20
1d54c:	02602825 	move	a1,s3
1d550:	00942021 	addu	a0,a0,s4
1d554:	02203025 	move	a2,s1
1d558:	0c02459d 	jal	0x91674
1d55c:	02003825 	move	a3,s0
1d560:	8e440004 	lw	a0,4(s2)
1d564:	26d60001 	addiu	s6,s6,1
1d568:	0096202a 	slt	a0,a0,s6
1d56c:	1080fff5 	beqz	a0,0x1d544
1d570:	26940058 	addiu	s4,s4,88
1d574:	c7b40004 	lwc1	$f20,4(sp)
1d578:	8fb00008 	lw	s0,8(sp)
1d57c:	8fb1000c 	lw	s1,12(sp)
1d580:	8fb20010 	lw	s2,16(sp)
1d584:	8fb30014 	lw	s3,20(sp)
1d588:	8fb40018 	lw	s4,24(sp)
1d58c:	8fb5001c 	lw	s5,28(sp)
1d590:	8fb60020 	lw	s6,32(sp)
1d594:	8fbf0024 	lw	ra,36(sp)
1d598:	03e00008 	jr	ra
1d59c:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: eAudioGroup::Update(cTimeValue, int, const mOCS *)
// Address: 0x0001d4a8, Size: 248B
// Obj: eAll_psp.obj

? eAudioChannel__Update_cTimeValue_float_int_constmOCSptr(f32, s32, s32, ?, ?); /* extern */
? eAudioFader__Update_cTimeValue(s32, s32);         /* extern */
f32 eAudioGroup__GetVolume_voidconst(void *);       /* extern */

void eAudioGroup__Update_cTimeValue_int_constmOCSptr(void *arg0, s32 arg1, ? arg2, ? arg3) {
    f32 temp_f20;
    s32 var_s4;
    s32 var_s4_2;
    s32 var_s5;
    s32 var_s6;

    var_s5 = 0;
    var_s4 = arg0 + 8;
    do {
        eAudioFader__Update_cTimeValue(var_s4, subroutine_arg0);
        var_s5 += 1;
        var_s4 += 0x10;
    } while (var_s5 < 3);
    var_s6 = arg0->unk0;
    temp_f20 = *(f32 *)0x37D0CC * eAudioGroup__GetVolume_voidconst(arg0);
    if (arg0->unk4 >= var_s6) {
        var_s4_2 = var_s6 * 0x58;
        do {
            eAudioChannel__Update_cTimeValue_float_int_constmOCSptr(temp_f20, *(s32 *)0x37D0D4 + var_s4_2, subroutine_arg0, arg2, arg3);
            var_s6 += 1;
            var_s4_2 += 0x58;
        } while (arg0->unk4 >= var_s6);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
