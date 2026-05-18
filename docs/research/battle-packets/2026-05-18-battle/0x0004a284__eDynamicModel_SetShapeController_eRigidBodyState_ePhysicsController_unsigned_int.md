# Battle Packet: `eDynamicModel::SetShapeController(eRigidBodyState *, ePhysicsController *, unsigned int)`

## Target

- Address: `0x0004a284`
- Size: `176` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `SetShapeController`
- Leaf: `True`

## Queue Metadata

- score: `167`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `10`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x00064f68` `untried` 1084B `eBipedController::Activate(eDynamicModel *)`
- `0x000653a4` `untried` 176B `eBipedController::Deactivate(eDynamicModel *)`
- `0x0006560c` `untried` 1172B `eBipedController::ChangeShape(int, const mVec3 &)`
- `0x00076978` `untried` 636B `eRigidBodyController::Activate(eDynamicModel *)`
- `0x00076bf4` `untried` 148B `eRigidBodyController::Deactivate(eDynamicModel *)`
- `0x000a6098` `untried` 3244B `eDynamicModel::Reset(cMemPool *, bool)`

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

- session `af005f70`; src `src/eDynamicModel.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/af005f70/0004a284__agent_self_reported_failure__src_eDynamicModel.cpp`

SetShapeController: 10/176 byte diff (sized correctly). Two structural issues remaining after extensive iteration. (1) Path 2 instruction scheduling: `lw a3, 276(a0)` and `li t0, -7` are SWAPPED (mine: lw,li; expected: li,lw). Tried explicit `int defaultPtr` extraction (which fixed register choice and unblocked the bnezl pattern, dropping diff from 71 to 10), reordering source statements, masked-temp variable, volatile reads — none flipped this specific scheduling decision. (2) FLT_MAX constant encoding: mine emits `lui 0x7f80; addiu -1` for 0x7f7fffff; expected emits `lui 0x7f7f; ori 0xffff`. Tradeoff: using inline asm for lui/ori (matches encoding) prevents the compiler from hoisting lui into the `beqz a0` delay slot (adds nop, costs 8 bytes + cascade); using C constant (0x7f7fffff or composed shift|or) lets compiler fill delay slot but it picks lui+addiu encoding. Tried `0x7f7f0000|0xffff`, `(0x7f7fu<<16)|0xffffu`, two-step assignment, unsigned types — compiler always folds to lui+addiu. KEY POSITIVES achieved: bnezl emission (via `if (controller==0)` form + volatile read in path 2 first lhu), correct lui/ori encoding for fmin (0xff7fffff), interleaved second lui before first sv.q (via splitting mtv/sv.q asm blocks), correct path duplication of lbu/andi (via explicit doVfpu variable computed in both branches). Permuter ran 3312 mutations in 120s with zero improvement — confirms this is a deep compiler decision not reachable via source mutation. The 10-byte diff is ~2 swapped instructions + 1 alternate-encoding instruction; both appear to be compiler-internal heuristics that source can't influence.


## Disassembly

```asm
4a284:	54c0000c 	bnezl	a2,0x4a2b8
4a288:	94a80098 	lhu	t0,152(a1)
4a28c:	94a60098 	lhu	a2,152(a1)
4a290:	2408fff9 	li	t0,-7
4a294:	8c870114 	lw	a3,276(a0)
4a298:	00c83024 	and	a2,a2,t0
4a29c:	a4a60098 	sh	a2,152(a1)
4a2a0:	aca70094 	sw	a3,148(a1)
4a2a4:	94a60098 	lhu	a2,152(a1)
4a2a8:	a4a60098 	sh	a2,152(a1)
4a2ac:	9084008c 	lbu	a0,140(a0)
4a2b0:	10000009 	b	0x4a2d8
4a2b4:	30840002 	andi	a0,a0,0x2
4a2b8:	30e7ffff 	andi	a3,a3,0xffff
4a2bc:	a4a80098 	sh	t0,152(a1)
4a2c0:	94a80098 	lhu	t0,152(a1)
4a2c4:	aca60094 	sw	a2,148(a1)
4a2c8:	01073025 	or	a2,t0,a3
4a2cc:	a4a60098 	sh	a2,152(a1)
4a2d0:	9084008c 	lbu	a0,140(a0)
4a2d4:	30840002 	andi	a0,a0,0x2
4a2d8:	10800014 	beqz	a0,0x4a32c
4a2dc:	3c047f7f 	lui	a0,0x7f7f
4a2e0:	3484ffff 	ori	a0,a0,0xffff
4a2e4:	44846000 	mtc1	a0,$f12
4a2e8:	44046000 	mfc1	a0,$f12
4a2ec:	44066000 	mfc1	a2,$f12
4a2f0:	44076000 	mfc1	a3,$f12
4a2f4:	48e40006 	mtv         $a0, S120
4a2f8:	48e60026 	mtv         $a2, S121
4a2fc:	48e70046 	mtv         $a3, S122
4a300:	3c04ff7f 	lui	a0,0xff7f
4a304:	f8a60070 	sv.q        C120, 0x70($a1)
4a308:	3484ffff 	ori	a0,a0,0xffff
4a30c:	44846800 	mtc1	a0,$f13
4a310:	44046800 	mfc1	a0,$f13
4a314:	44066800 	mfc1	a2,$f13
4a318:	44076800 	mfc1	a3,$f13
4a31c:	48e40006 	mtv         $a0, S120
4a320:	48e60026 	mtv         $a2, S121
4a324:	48e70046 	mtv         $a3, S122
4a328:	f8a60080 	sv.q        C120, 0x80($a1)
4a32c:	03e00008 	jr	ra
4a330:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::SetShapeController(eRigidBodyState *, ePhysicsController *, unsigned int)
// Address: 0x0004a284, Size: 176B
// Obj: eAll_psp.obj

void eDynamicModel__SetShapeController_eRigidBodyStateptr_ePhysicsControllerptr_unsignedint(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    if (arg2 != 0) {
        arg1->unk98 = (u16) arg1->unk98;
        arg1->unk94 = arg2;
        arg1->unk98 = (u16) (arg1->unk98 | (arg3 & 0xFFFF));
    } else {
        arg1->unk98 = (u16) (arg1->unk98 & ~6);
        arg1->unk94 = (s32) arg0->unk114;
        arg1->unk98 = (u16) arg1->unk98;
    }
    if (arg0->unk8C & 2) {
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a2, S121");
        __asm__ volatile("mtv $a3, S122");
        __asm__ volatile("sv.q C120, 0x70($a1)");
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a2, S121");
        __asm__ volatile("mtv $a3, S122");
        __asm__ volatile("sv.q C120, 0x80($a1)");
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
