# Battle Packet: `eDynamicModel::SetSkin(cHandleT<eSkin>, int, cHandleT<eSkin>, cTimeValue)`

## Target

- Address: `0x00047f80`
- Size: `80` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `SetSkin`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; has failed src_file; near miss 8B; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00047fd0` `untried` 1304B `eDynamicModel::SetMaterialSet(int, cTimeValue)`
- `0x000484e8` `untried` 656B `eDynamicModel::SetSurfaceSet(int)`

Callers:
- `0x00171e80` `untried` 4952B `gcEntity::Reset(cMemPool *, bool)`
- `0x001746f8` `untried` 11232B `gcEntity::Replicate(gcReplicationVisitor *)`
- `0x002ccf40` `untried` 1128B `gcDoEntitySetSkin::Evaluate(void) const`

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

- session `manual_2026-05-06`; src `src/eDynamicModel.cpp`

PREEXISTING DRIFT — matched-status was a lie. Investigation:
(1) Commit 55c91d6 (May 4) changed signature `cHandleT<eSkin> skin2` to `int skin2` and dropped `.mHandle` access while claiming a match. The orchestrator's verify accepted it but the symbol mangled differently; bytes match enough that a naive symbol-by-symbol diff passed.
(2) Real source signature per .map debug symbols: (cHandleT<eSkin>, int, cHandleT<eSkin>, cTimeValue). Restoring this gives `iTB...` mangling; the EBOOT symbol is `iTC...` because the original SNC counts `this` for the backref position (B=this, C=arg1) while our SNC doesn't (B=arg1). Same compiler version, different mangling rule. We can either match-the-bytes with `int skin2` (mangles `iTC`, type-incorrect) or match-the-truth with `cHandleT<eSkin> skin2` (mangles `iTB`, doesn't find the DB symbol). Currently using `int skin2`.
(3) With `int skin2` the codegen is byte-mismatched ~8-30 bytes (depending on counting method) due to the s0↔s1 register-allocation flip — the same SNC instability that affects cFactory::MarkForClean, cFactory::CleanGroups, gcViewport::UpdateAll, gcViewport::UpdateUI. Function semantics are correct; SNC's allocator picks `time→s0/this→s1` while the original picks `time→s1/this→s0`. Permuter exhausts source-level perturbations with zero improvement.
(4) Header changes elsewhere in the TU perturb the diff (post-run with 4 added decls in eDynamicModel.h: 8-byte diff; clean header: ~30 bytes). Suggests this is on the boundary of matchable but the allocator is locked in by some TU-wide property we don't control.

### Note 2

- session `fix_plan_phase3_2026-04-24`

[fix_plan Phase 3, typedef-int-collapse audit] Reconstruction declares one of ['6KcTimeValue'] as `typedef int X;` somewhere in its include/src chain, collapsing the original class type (mangled='__0fNeDynamicModelHSetSkin6IcHandleT76FeSkin_iTC6KcTimeValue') to a primitive (.o-mangled='__0fNeDynamicModelHSetSkin6IcHandleT76FeSkin_iTB6KcTimeValue'). Bytes match because both pass through MIPS registers identically, but training-data fidelity is lost: model would learn the parameter as int. Reset to untried; reattempt should declare the type as a struct/class with the appropriate single int member so SNC mangling produces 6OX / 6SX.


## Disassembly

```asm
47f80:	27bdfff0 	addiu	sp,sp,-16
47f84:	afb00000 	sw	s0,0(sp)
47f88:	00808025 	move	s0,a0
47f8c:	260400fc 	addiu	a0,s0,252
47f90:	ac850000 	sw	a1,0(a0)
47f94:	afb10004 	sw	s1,4(sp)
47f98:	00c02825 	move	a1,a2
47f9c:	00e08825 	move	s1,a3
47fa0:	02002025 	move	a0,s0
47fa4:	afbf0008 	sw	ra,8(sp)
47fa8:	0c011ff4 	jal	0x47fd0
47fac:	01003025 	move	a2,t0
47fb0:	02002025 	move	a0,s0
47fb4:	0c01213a 	jal	0x484e8
47fb8:	02202825 	move	a1,s1
47fbc:	8fb00000 	lw	s0,0(sp)
47fc0:	8fb10004 	lw	s1,4(sp)
47fc4:	8fbf0008 	lw	ra,8(sp)
47fc8:	03e00008 	jr	ra
47fcc:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::SetSkin(cHandleT<eSkin>, int, cHandleT<eSkin>, cTimeValue)
// Address: 0x00047f80, Size: 80B
// Obj: eAll_psp.obj

? eDynamicModel__SetMaterialSet_int_cTimeValue(void *, ?, ?); /* extern */
? eDynamicModel__SetSurfaceSet_int(void *, ?);      /* extern */

void eDynamicModel__SetSkin_cHandleT_eSkin__int_cHandleT_eSkin__cTimeValue(void *arg0, s32 arg1, ? arg2, ? arg3) {
    arg0->unkFC = arg1;
    eDynamicModel__SetMaterialSet_int_cTimeValue(arg0, arg2, M2C_ERROR(/* Read from unset register $t0 */));
    eDynamicModel__SetSurfaceSet_int(arg0, arg3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
