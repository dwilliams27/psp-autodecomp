# Battle Packet: `gcEntity::GetPlayer(void) const`

## Target

- Address: `0x00117440`
- Size: `88` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntity`
- Method family: `GetPlayer`
- Leaf: `True`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 15B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `15`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `15`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x0011ecc0` `matched` 68B `gcValControllerState::GetEntityControllerId(gcEntity *) static`
- `0x0012194c` `untried` 324B `gcDesiredCamera::GetFromEntity(gcEntity *, bool) static`

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

Kept the best existing C form for gcEntity::GetPlayer: it matches the original through the player-table calculation and return path, including the initial beqz, lb, bltzl with the next-entity load in the delay slot, and the redundant second bltz before bounds checking. I tried a canonical while(entity) loop, a do/while assignment condition, a while-with-goto body, a fallthrough-to-final-return variant, and a comma-expression short-circuit variant aimed at making the tail null-check become the original bnezl with lb in the delay slot.

The while(entity) form can produce the desired bnezl tail, but it changes the front half to a bgez-based layout and becomes a 65-byte mismatch. All forms that preserve the correct front half still emit beqz a1; nop; b check_index with the lb in the unconditional branch delay slot, leaving the same 15/88 byte mismatch. I also tried rerunning tools/permuter.py for this target, but it still reported an incorrect 74-byte baseline for the function despite compare_func.py showing the focused 15-byte diff, so I stopped it rather than optimizing against the wrong comparison.

### Note 2

- session `c3a8767f`

Implemented the controller-index walk in C and tried multiple source shapes: straight while loop, do/while after a negative index, short-circuit next-pointer assignment, and a goto-shaped tail layout. The best manual form matches the front half exactly, including the initial bltzl with the 0x218 next-pointer load in the delay slot and the redundant second bltz before the player-table lookup. The remaining mismatch is the tail null check after loading the next entity: original has bnezl a1 back to the index check with lb 0x67(a1) in the delay slot, then falls directly to return zero; current output emits beqz a1 to return, nop, then an unconditional branch back with the lb in its delay slot, producing a 15/88 byte mismatch. Ran tools/permuter.py for 300 seconds on 0x00117440; it used a worse 74-byte baseline for this file and saved no focused-compare improvement.


## Disassembly

```asm
117440:	00802825 	move	a1,a0
117444:	10a00012 	beqz	a1,0x117490
117448:	00000000 	nop
11744c:	80a40067 	lb	a0,103(a1)
117450:	0482000d 	bltzl	a0,0x117488
117454:	8ca50218 	lw	a1,536(a1)
117458:	04800009 	bltz	a0,0x117480
11745c:	34020000 	li	v0,0x0
117460:	28850008 	slti	a1,a0,8
117464:	10a00006 	beqz	a1,0x117480
117468:	00042980 	sll	a1,a0,0x6
11746c:	00042080 	sll	a0,a0,0x2
117470:	00a41021 	addu	v0,a1,a0
117474:	3c040038 	lui	a0,0x38
117478:	8c84d87c 	lw	a0,-10116(a0)
11747c:	00821021 	addu	v0,a0,v0
117480:	03e00008 	jr	ra
117484:	00000000 	nop
117488:	54a0fff1 	bnezl	a1,0x117450
11748c:	80a40067 	lb	a0,103(a1)
117490:	03e00008 	jr	ra
117494:	00001025 	move	v0,zero
```

## m2c Starting Point

```c
// Decompiled: gcEntity::GetPlayer(void) const
// Address: 0x00117440, Size: 88B
// Obj: gcAll_psp.obj

s32 gcEntity__GetPlayer_voidconst(void *arg0) {
    s32 var_v0;
    s8 temp_a0;
    void *var_a1;

    var_a1 = arg0;
    if (var_a1 != NULL) {
loop_1:
        temp_a0 = var_a1->unk67;
        if (temp_a0 < 0) {
            var_a1 = var_a1->unk218;
            if (var_a1 == NULL) {
                /* Duplicate return node #9. Try simplifying control flow for better match */
                return 0;
            }
            goto loop_1;
        }
        var_v0 = 0;
        if ((temp_a0 >= 0) && (temp_a0 < 8)) {
            var_v0 = *(s32 *)0x37D87C + (temp_a0 * 0x44);
        }
        return var_v0;
    }
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
