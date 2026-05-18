# Battle Packet: `eDynamicModel::GetPartialAnimationState(int)`

## Target

- Address: `0x00049624`
- Size: `144` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `GetPartialAnimationState`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 20B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `20`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `20`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x0014074c` `untried` 148B `gcPartialBodyController::Update(cTimeValue)`
- `0x001407e0` `untried` 160B `gcPartialBodyController::PostUpdateFinal(void)`
- `0x00140a3c` `untried` 400B `gcPartialBodyController::HandleNextAnimation(cTimeValue)`
- `0x0017d534` `untried` 1756B `gcPartialBodyController::Replicate(gcReplicationVisitor *)`

## Placement

- Canonical source: `src/eDynamicModel.cpp`
- Header(s): `include/eDynamicModel.h`
- Allowed source(s): `src/eDynamicModel.cpp`
- Split-TU prefix: `src/eDynamicModel_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eDynamicModel.h`

```cpp
#ifndef EDYNAMICMODEL_H
#define EDYNAMICMODEL_H

#include "eDynamicGeom.h"

class cBase;
class cMemPool;
class cFile;
class cTimeValue;
class cType;
class eDynamicMesh;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mSphere;
class mRay;
class mCollideHit;
class eSkin;
template <class T> class cHandleT;
struct mOCS;

class eDynamicModel : public eDynamicGeom {
public:
    eDynamicModel(cBase *);
    eDynamicModel &operator=(const eDynamicModel &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void PlatformRead(cFile &, cMemPool *);
    void PlatformFree(void);
    void AddToBroadphase(void);
    void ResetPartialAnimationController(int);
    void OnRemovedFromWorld(void);
    bool NeedsSkinning(const eDynamicMesh *, int, int *) const;
    void *GetCurrentPhysicsController(void) const;
    void *GetCurrentPhysicsController(void);
    void GetColliderToWorld(int, mOCS *) const;
    int GetSubObjectIndex(const cName &, int) const;
    void GetSubObjectToWorld(int, mOCS *) const;
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
    void Write(cFile &) const;
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void __attribute__((naked)) CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    void *GetAnimationState(void) const;
    void *GetPartialAnimationState(int);
    void ClearPartialAnimationController(int);
    int GetSkinIndex(void) const;
    void SetSkin(cHandleT<eSkin>, int, int, cTimeValue);
    void SetMaterialSet(int, cTimeValue);
    void SetSurfaceSet(int);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00047070` 88B `eDynamicModel::Write(cFile &) const` — `src/eDynamicModel.cpp`
- `0x0004737c` 132B `eDynamicModel::PlatformFree(void)` — `src/eDynamicModel.cpp`
- `0x00047400` 144B `eDynamicModel::eDynamicModel(cBase *)` — `src/eDynamicModel.cpp`
- `0x00047a00` 8B `eDynamicModel::PlatformReset(cMemPool *, bool)` — `src/eDynamicModel_PlatformReset.cpp`
- `0x00047a08` 156B `eDynamicModel::NeedsSkinning(const eDynamicMesh *, int, int *) const` — `src/eDynamicModel.cpp`

## Prior Failure Notes

### Note 1

- session `4583e88c`; src `src/eDynamicModel.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/4583e88c/00049624__agent_self_reported_failure__src_eDynamicModel.cpp`

Closest C++ attempt is a normal cArray count check plus handle lookup through D_00038890. It compiles to the correct size and control flow, including the bnezl return pattern, but 20/144 bytes differ: SNC allocates the state pointer in a1 and the handle in a0, while the original keeps the state pointer in a0 and loads the handle into a1; the lookup result/resolved registers are similarly swapped (ours uses a2/a3 opposite the original). Tried char* arithmetic, in-place pointer updates, reusing idx as the handle, explicit register variables, a 0x44-byte PartialAnimationState struct, volatile state pointer, sched=1, and sched=2. The targeted permuter ran for 300s with --sched 2, compiled 3,418 candidates, and found no improvement from the 20-byte baseline. This is not a bnel-only case because the diff is 20 bytes and primarily register allocation/source-shape related.


## Disassembly

```asm
49624:	8c840128 	lw	a0,296(a0)
49628:	10800005 	beqz	a0,0x49640
4962c:	34060000 	li	a2,0x0
49630:	8c86fffc 	lw	a2,-4(a0)
49634:	3c074000 	lui	a3,0x4000
49638:	24e7ffff 	addiu	a3,a3,-1
4963c:	00c73024 	and	a2,a2,a3
49640:	00a6302a 	slt	a2,a1,a2
49644:	10c00019 	beqz	a2,0x496ac
49648:	00053180 	sll	a2,a1,0x6
4964c:	00052880 	sll	a1,a1,0x2
49650:	00c52821 	addu	a1,a2,a1
49654:	00852021 	addu	a0,a0,a1
49658:	8c850000 	lw	a1,0(a0)
4965c:	14a00003 	bnez	a1,0x4966c
49660:	34020000 	li	v0,0x0
49664:	1000000d 	b	0x4969c
49668:	34060000 	li	a2,0x0
4966c:	30a6ffff 	andi	a2,a1,0xffff
49670:	3c070004 	lui	a3,0x4
49674:	00063080 	sll	a2,a2,0x2
49678:	24e78890 	addiu	a3,a3,-30576
4967c:	00c73021 	addu	a2,a2,a3
49680:	8cc70000 	lw	a3,0(a2)
49684:	10e00005 	beqz	a3,0x4969c
49688:	34060000 	li	a2,0x0
4968c:	8ce80030 	lw	t0,48(a3)
49690:	15050002 	bne	t0,a1,0x4969c
49694:	00000000 	nop
49698:	00e03025 	move	a2,a3
4969c:	54c00001 	bnezl	a2,0x496a4
496a0:	00801025 	move	v0,a0
496a4:	03e00008 	jr	ra
496a8:	00000000 	nop
496ac:	03e00008 	jr	ra
496b0:	00001025 	move	v0,zero
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::GetPartialAnimationState(int)
// Address: 0x00049624, Size: 144B
// Obj: eAll_psp.obj

s32 *eDynamicModel__GetPartialAnimationState_int(void *arg0, s32 arg1) {
    s32 *temp_a0_2;
    s32 *var_v0;
    s32 temp_a1;
    s32 var_a2;
    void *temp_a0;
    void *temp_a3;
    void *var_a2_2;

    temp_a0 = arg0->unk128;
    var_a2 = 0;
    if (temp_a0 != NULL) {
        var_a2 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    if (arg1 < var_a2) {
        temp_a0_2 = temp_a0 + (arg1 * 0x44);
        temp_a1 = *temp_a0_2;
        var_v0 = NULL;
        if (temp_a1 == 0) {
            var_a2_2 = NULL;
        } else {
            temp_a3 = ((temp_a1 & 0xFFFF) * 4)->unk38890;
            var_a2_2 = NULL;
            if ((temp_a3 != NULL) && (temp_a3->unk30 == temp_a1)) {
                var_a2_2 = temp_a3;
            }
        }
        if (var_a2_2 != NULL) {
            var_v0 = temp_a0_2;
        }
        return var_v0;
    }
    return NULL;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
