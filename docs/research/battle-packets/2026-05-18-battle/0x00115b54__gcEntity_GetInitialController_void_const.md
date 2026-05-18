# Battle Packet: `gcEntity::GetInitialController(void) const`

## Target

- Address: `0x00115b54`
- Size: `184` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntity`
- Method family: `GetInitialController`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 27B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `27`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `27`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x00110a30` `untried` 552B `gcEntityController::Reset(cMemPool *, bool)`

## Placement

- Canonical source: `src/gcEntity.cpp`
- Header(s): `include/gcEntity.h`
- Allowed source(s): `src/gcEntity.cpp`, `src/gcEntity_GetInitialController.cpp`
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

- session `ab3d346d`; src `src/gcEntity_GetInitialController.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/ab3d346d/00115b54__agent_self_reported_failure__src_gcEntity_GetInitialController.cpp`

Implemented the real split-TU C++ body: cArray pointer at this+0x1E0 with arr[-1] count, initial signed controller byte at this+0x60, direct indexed return when valid, fallback handle at this+0x44 resolved through global table 0x38890 and validated against candidate+0x30, then fallback signed byte at candidate+0x74. Manual variants tried included signed-char vs int index, explicit if/else sign-extension forms, precomputed nextIndex forms, branch-local fallback assignment, handle-table address-lowering variants, sched=1, fixed-register hints, and a pointer-temp index load.

Current best is correct size (184B) but 27/184 bytes differ. The handle-table path and bottom control-flow shape now match closely, including the duplicated shift/sign-extension block, but SNC allocates the index/count registers opposite the target: current uses a3 for index and a1 for count, while target uses a1 for index and a3 for count. That swap cascades through the initial bounds checks and final fallback bounds checks. The permuter could not run on earlier 176B/180B variants because it refuses size mismatches; once the source reached 184B the remaining diff looked like register-allocation/source-shape drift rather than an obvious branch-likely issue.


## Disassembly

```asm
115b54:	8c8601e0 	lw	a2,480(a0)
115b58:	34070000 	li	a3,0x0
115b5c:	80850060 	lb	a1,96(a0)
115b60:	54c00001 	bnezl	a2,0x115b68
115b64:	8cc7fffc 	lw	a3,-4(a2)
115b68:	04a00003 	bltz	a1,0x115b78
115b6c:	00a7402a 	slt	t0,a1,a3
115b70:	55000006 	bnezl	t0,0x115b8c
115b74:	00052080 	sll	a0,a1,0x2
115b78:	8c880044 	lw	t0,68(a0)
115b7c:	15000006 	bnez	t0,0x115b98
115b80:	3104ffff 	andi	a0,t0,0xffff
115b84:	1000000f 	b	0x115bc4
115b88:	34040000 	li	a0,0x0
115b8c:	00c42021 	addu	a0,a2,a0
115b90:	03e00008 	jr	ra
115b94:	8c820000 	lw	v0,0(a0)
115b98:	3c090004 	lui	t1,0x4
115b9c:	00042080 	sll	a0,a0,0x2
115ba0:	25298890 	addiu	t1,t1,-30576
115ba4:	00892021 	addu	a0,a0,t1
115ba8:	8c890000 	lw	t1,0(a0)
115bac:	11200005 	beqz	t1,0x115bc4
115bb0:	34040000 	li	a0,0x0
115bb4:	8d2a0030 	lw	t2,48(t1)
115bb8:	15480002 	bne	t2,t0,0x115bc4
115bbc:	00000000 	nop
115bc0:	01202025 	move	a0,t1
115bc4:	1080000f 	beqz	a0,0x115c04
115bc8:	00000000 	nop
115bcc:	04a00003 	bltz	a1,0x115bdc
115bd0:	00052e00 	sll	a1,a1,0x18
115bd4:	10000004 	b	0x115be8
115bd8:	00052e03 	sra	a1,a1,0x18
115bdc:	80840074 	lb	a0,116(a0)
115be0:	00042e00 	sll	a1,a0,0x18
115be4:	00052e03 	sra	a1,a1,0x18
115be8:	04a00006 	bltz	a1,0x115c04
115bec:	00a7202a 	slt	a0,a1,a3
115bf0:	10800004 	beqz	a0,0x115c04
115bf4:	00052080 	sll	a0,a1,0x2
115bf8:	00c42021 	addu	a0,a2,a0
115bfc:	03e00008 	jr	ra
115c00:	8c820000 	lw	v0,0(a0)
115c04:	03e00008 	jr	ra
115c08:	00001025 	move	v0,zero
```

## m2c Starting Point

```c
// Decompiled: gcEntity::GetInitialController(void) const
// Address: 0x00115b54, Size: 184B
// Obj: gcAll_psp.obj

s32 gcEntity__GetInitialController_voidconst(void *arg0) {
    s32 temp_a1;
    s32 temp_t0;
    s32 var_a1;
    s32 var_a3;
    s8 temp_a1_2;
    void *temp_a2;
    void *temp_t1;
    void *var_a0;

    temp_a2 = arg0->unk1E0;
    var_a3 = 0;
    temp_a1_2 = arg0->unk60;
    if (temp_a2 != NULL) {
        var_a3 = temp_a2->unk-4;
    }
    if (temp_a1_2 >= 0) {
        if (temp_a1_2 < var_a3) {
            return *(temp_a2 + (temp_a1_2 * 4));
        }
        goto block_6;
    }
block_6:
    temp_t0 = arg0->unk44;
    if (temp_t0 == 0) {
        var_a0 = NULL;
    } else {
        temp_t1 = ((temp_t0 & 0xFFFF) * 4)->unk38890;
        var_a0 = NULL;
        if ((temp_t1 != NULL) && (temp_t1->unk30 == temp_t0)) {
            var_a0 = temp_t1;
        }
    }
    if (var_a0 != NULL) {
        var_a1 = temp_a1_2 << 0x18;
        if (temp_a1_2 >= 0) {

        } else {
            var_a1 = var_a0->unk74 << 0x18;
        }
        temp_a1 = var_a1 >> 0x18;
        if ((temp_a1 >= 0) && (temp_a1 < var_a3)) {
            return *(temp_a2 + (temp_a1 * 4));
        }
        /* Duplicate return node #19. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
