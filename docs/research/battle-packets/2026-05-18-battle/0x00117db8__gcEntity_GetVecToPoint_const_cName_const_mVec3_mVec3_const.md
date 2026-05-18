# Battle Packet: `gcEntity::GetVecToPoint(const cName &, const mVec3 &, mVec3 *) const`

## Target

- Address: `0x00117db8`
- Size: `140` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntity`
- Method family: `GetVecToPoint`
- Leaf: `True`

## Queue Metadata

- score: `129`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 36B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `36`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `36`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x00117e44` `untried` 160B `gcEntity::GetVecToEntityHelper(const cName &, const gcEntity *, const cName &, mVec3 *) const`
- `0x00328ee0` `untried` 4180B `gcValEntityDirection::Evaluate(void) const`
- `0x0032c730` `untried` 2708B `gcValEntityDistance::Evaluate(void) const`

## Placement

- Canonical source: `src/gcEntity.cpp`
- Header(s): `include/gcEntity.h`
- Allowed source(s): `src/gcEntity.cpp`, `src/gcEntity_GetVecToPoint.cpp`
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

- session `ab3d346d`; src `src/gcEntity_GetVecToPoint.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/ab3d346d/00117db8__agent_self_reported_failure__src_gcEntity_GetVecToPoint.cpp`

Implemented the real split-TU C++ body with local gcEntity redeclaration, dispatch entries at dynamic-model classdesc offsets 0xE0/0xE8, a 64-byte scratch area passed as sp, and the VFPU subtract/store sequence loading point from the incoming mVec3 and scratch+0x30. Tried canonical 3-arg function pointer casts, varargs dispatch entries that preserve the function pointer in a3, volatile dispatch load, declaration reordering, sched=1, and hard register hints; sched=1 did not help and register hints were ignored by SNC. The current closest source is the permuter-saved best.

Current best is correct size (140B) but 36/140 bytes differ. The second dynamic call and VFPU tail are structurally right; the remaining diff is concentrated in prologue/call-setup scheduling and register allocation. Target delays some s-register saves and uses s0=point, s1=entry+0xE8, s2=out, with dispatch initially in t0; current best saves all callee registers up front, uses s0=out and s2=point, and keeps some offset loads in a2 rather than t0. A 300s permuter run generated 10,536 candidates / 4,160 compiled and improved 45 -> 36 bytes, then stalled.


## Disassembly

```asm
117db8:	27bdffa0 	addiu	sp,sp,-96
117dbc:	8c880084 	lw	t0,132(a0)
117dc0:	afb00040 	sw	s0,64(sp)
117dc4:	afb10044 	sw	s1,68(sp)
117dc8:	afb20048 	sw	s2,72(sp)
117dcc:	00c08025 	move	s0,a2
117dd0:	251100e8 	addiu	s1,t0,232
117dd4:	00e09025 	move	s2,a3
117dd8:	86260000 	lh	a2,0(s1)
117ddc:	250700e0 	addiu	a3,t0,224
117de0:	24840080 	addiu	a0,a0,128
117de4:	84e80000 	lh	t0,0(a3)
117de8:	afb3004c 	sw	s3,76(sp)
117dec:	00869821 	addu	s3,a0,a2
117df0:	8ce70004 	lw	a3,4(a3)
117df4:	00882021 	addu	a0,a0,t0
117df8:	afbf0050 	sw	ra,80(sp)
117dfc:	00e0f809 	jalr	a3
117e00:	00003025 	move	a2,zero
117e04:	8e270004 	lw	a3,4(s1)
117e08:	02602025 	move	a0,s3
117e0c:	00402825 	move	a1,v0
117e10:	00e0f809 	jalr	a3
117e14:	03a03025 	move	a2,sp
117e18:	da060000 	lv.q        C120, 0x0($s0)
117e1c:	dba70030 	lv.q        C130, 0x30($sp)
117e20:	60878606 	vsub.t      C120, C120, C130
117e24:	fa460000 	sv.q        C120, 0x0($s2)
117e28:	8fb00040 	lw	s0,64(sp)
117e2c:	8fb10044 	lw	s1,68(sp)
117e30:	8fb20048 	lw	s2,72(sp)
117e34:	8fb3004c 	lw	s3,76(sp)
117e38:	8fbf0050 	lw	ra,80(sp)
117e3c:	03e00008 	jr	ra
117e40:	27bd0060 	addiu	sp,sp,96
```

## m2c Starting Point

```c
// Decompiled: gcEntity::GetVecToPoint(const cName &, const mVec3 &, mVec3 *) const
// Address: 0x00117db8, Size: 140B
// Obj: gcAll_psp.obj

void gcEntity__GetVecToPoint_constcNameref_constmVec3ref_mVec3ptrconst(void *arg0, ? arg2, ? arg3) {
    ? (*temp_a3_2)(void *, s32, void *, ?);
    s32 (*temp_a3)(void *, ?, ?);
    void *temp_a0;
    void *temp_s3;
    void *temp_t0;

    temp_t0 = arg0->unk84;
    temp_a0 = arg0 + 0x80;
    temp_s3 = temp_a0 + temp_t0->unkE8;
    temp_a3 = (temp_t0 + 0xE0)->unk4;
    temp_a3_2 = (temp_t0 + 0xE8)->unk4;
    temp_a3_2(temp_s3, temp_a3(temp_a0 + temp_t0->unkE0, 0, temp_a3), sp, temp_a3_2);
    __asm__ volatile("lv.q C120, ($s0)");
    __asm__ volatile("lv.q C130, 0x30($sp)");
    __asm__ volatile("vsub.t C120, C120, C130");
    __asm__ volatile("sv.q C120, ($s2)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
