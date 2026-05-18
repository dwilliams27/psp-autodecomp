# Battle Packet: `gcEntity::OnFullscreenCinematicStarting(void) static`

## Target

- Address: `0x00119a24`
- Size: `88` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntity`
- Method family: `OnFullscreenCinematicStarting`
- Leaf: `True`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 12B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x00123e20` `untried` 460B `gcMsgPlayCinematic::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)`

## Placement

- Canonical source: `src/gcEntity.cpp`
- Header(s): `include/gcEntity.h`
- Allowed source(s): `src/gcEntity.cpp`
- Split-TU prefix: `src/gcEntity_*.cpp`

## Class Header: `include/gcEntity.h`

```cpp
#ifndef GCENTITY_H
#define GCENTITY_H

#include "cObject.h"
#include "mVec3.h"

class cBase;
class cMemPool;
class cType;
class gcPlayer;

class gcEntity : public cObject {
public:
    gcEntity(cBase *);
    gcEntity &operator=(const gcEntity &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    static void OnFullscreenCinematicStarting(void);
    gcPlayer *GetPlayer(void) const;
    void SetInitialControllerIndex(int);
    void SetVelocity(const mVec3 &);
    void *GetSubGeomController(int);
    void GetVelocity(mVec3 *) const;
    void SetPreviousPrimaryController(bool);
    void SetPrimaryController(int, bool);
    float GetSoundFrequencyOffset(int) const;
    float GetSoundVolumeOffset(int) const;
};

#endif
```

## Matched Same-Class Neighbors

- `0x00114300` 316B `gcEntity::gcEntity(cBase *)` — `src/gcEntity.cpp`
- `0x00117498` 28B `gcEntity::SetVelocity(const mVec3 &)` — `src/gcEntity.cpp`
- `0x0011887c` 96B `gcEntity::GetSoundFrequencyOffset(int) const` — `src/gcEntity.cpp`
- `0x00118964` 96B `gcEntity::GetSoundVolumeOffset(int) const` — `src/gcEntity.cpp`
- `0x0011b238` 12B `gcEntity::SetInitialControllerIndex(int)` — `src/gcEntity_SetInitialControllerIndex.cpp`

## Prior Failure Notes

### Note 1

- session `c9d36816`

The clean C implementation of gcEntity::OnFullscreenCinematicStarting still has the correct algorithm and control flow: test global 0x37D858 bit 1, read the circular list head from 0x37D86C, clear 0x20000 when node flags at +0x5C have bit 0x80, advance by +0x228, and loop until the head is reached. The remaining diff is register allocation only: emitted code keeps the global base in a3, cursor in a2, and flags/head reload in a1; the original uses a2 for the global base, a1 for the cursor, and a3 for flags/head reload.

I tried fixed register variables for globals/entity/flags using both aN and $N names, empty asm constraints to force those locals live, moving flags outside the loop, typed node/head structs, direct 0x37D86C loads, 0x380000 negative-index typed globals, declaration-order changes, and a sched=1 compile. Fixed register hints were ignored for this leaf allocation, typed direct-head forms introduced an extra addiu and worsened the diff to 53 bytes, and sched=1 produced the same register assignment. The best form remains the original 12/88 byte register-allocation mismatch.

### Note 2

- session `c3a8767f`

Implemented the fullscreen cinematic circular-list walk in C: check global flag 0x37D858 bit 1, start from the list head at 0x37D86C, clear bit 0x20000 when entity flags at +0x5C have bit 0x80, then advance by +0x228 until the head is reached again. Tried direct 0x37D86C loads, an extern linker symbol, a 0x380000 negative-index global table form, typed and void cursors, and widened flag lifetimes. Best output has identical control flow and instruction selection but differs by register allocation only: current uses a3 for the global base, a2 for the entity cursor, and a1 for flags/head reload, while the original uses a2/a1/a3 respectively. Ran tools/permuter.py for 300 seconds on 0x00119a24 with a correct 12-byte baseline; it generated 10488 candidates, compiled 5974, and found no improvement.


## Disassembly

```asm
119a24:	3c040038 	lui	a0,0x38
119a28:	8c84d858 	lw	a0,-10152(a0)
119a2c:	30840002 	andi	a0,a0,0x2
119a30:	0004202b 	sltu	a0,zero,a0
119a34:	308400ff 	andi	a0,a0,0xff
119a38:	1080000e 	beqz	a0,0x119a74
119a3c:	3c060038 	lui	a2,0x38
119a40:	8cc5d86c 	lw	a1,-10132(a2)
119a44:	10a0000b 	beqz	a1,0x119a74
119a48:	3c04fffe 	lui	a0,0xfffe
119a4c:	2484ffff 	addiu	a0,a0,-1
119a50:	8ca7005c 	lw	a3,92(a1)
119a54:	30e80080 	andi	t0,a3,0x80
119a58:	11000002 	beqz	t0,0x119a64
119a5c:	00e43824 	and	a3,a3,a0
119a60:	aca7005c 	sw	a3,92(a1)
119a64:	8ca50228 	lw	a1,552(a1)
119a68:	8cc7d86c 	lw	a3,-10132(a2)
119a6c:	54a7fff9 	bnel	a1,a3,0x119a54
119a70:	8ca7005c 	lw	a3,92(a1)
119a74:	03e00008 	jr	ra
119a78:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcEntity::OnFullscreenCinematicStarting(void) static
// Address: 0x00119a24, Size: 88B
// Obj: gcAll_psp.obj

void gcEntity__OnFullscreenCinematicStarting_voidstatic(void) {
    s32 temp_a3;
    void *var_a1;

    if (*(s32 *)0x37D858 & 2 & 0xFF) {
        var_a1 = *(void **)0x37D86C;
        if (var_a1 != NULL) {
            do {
                temp_a3 = var_a1->unk5C;
                if (temp_a3 & 0x80) {
                    var_a1->unk5C = (s32) (temp_a3 & 0xFFFDFFFF);
                }
                var_a1 = var_a1->unk228;
            } while (var_a1 != *(void **)0x37D86C);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
