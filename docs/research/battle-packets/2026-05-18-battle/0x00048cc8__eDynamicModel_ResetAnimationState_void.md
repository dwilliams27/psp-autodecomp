# Battle Packet: `eDynamicModel::ResetAnimationState(void)`

## Target

- Address: `0x00048cc8`
- Size: `180` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `ResetAnimationState`
- Leaf: `True`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 17B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `17`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `17`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x000a6098` `untried` 3244B `eDynamicModel::Reset(cMemPool *, bool)`
- `0x00111f08` `untried` 532B `gcEntityController::HandleNextAnimation(cTimeValue)`
- `0x00170f00` `untried` 2204B `gcEntityController::Replicate(gcReplicationVisitor *)`

## Placement

- Canonical source: `src/eDynamicModel.cpp`
- Header(s): `include/eDynamicModel.h`
- Allowed source(s): `src/eDynamicModel.cpp`, `src/eDynamicModel_SetGeomFlagsOnOff.cpp`
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

- session `8ff06f7b`; src `src/eDynamicModel_SetGeomFlagsOnOff.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/8ff06f7b/00048cc8__agent_self_reported_failure__src_eDynamicModel_SetGeomFlagsOnOff.cpp`

category=REG_ALLOC/retryable_nearmiss. Started from the existing split-TU closest attempt. Re-tested the known sched=2 shape, tried explicit register-bound locals for byteOff=t0/len=t1, and tried asm-output zero initialization for byteOff. Register-bound locals were ignored by SNC and left the same 17-byte t0/t1 swap; asm-output initialization worsened scheduling to a 57-byte diff, so it was reverted. Current source is back to the best known 17/180-byte mismatch and preserves the matched SetGeomFlagsOnOff sibling.

Remaining diff is only register naming in the loop body: compiled byteOff/len use t1/t0 while expected uses t0/t1. Differing words are at offsets 0x50, 0x64-0x7c, 0x88, 0x90-0xa0, all matching operations/immediates/control flow except t0/t1 encodings. Ran tools/permuter.py for 326.5s with 8,448 generated candidates and 5,403 compiled; best stayed 17 bytes with zero improvements.

### Note 2

- session `888688e8`; src `src/eDynamicModel.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/888688e8/00048cc8__agent_self_reported_failure__src_eDynamicModel.cpp`

Got to 17/180 byte diff (90.6% match). Function structure, instruction order, and scheduling all align with expected. Remaining diff is purely register naming inside the loop body: our compile uses byteOff=t1 and len=t0, expected has byteOff=t0 and len=t1. The compiler picks t2 for `i` (matching expected), then allocates t1 for byteOff (second declared variable used in loop preamble) and t0 for len (third variable, declared inside loop). Expected allocates them swapped (t0 for byteOff, t1 for len) — this is an SNC register-allocator quirk we couldn't trigger from C source.

Approaches tried: (1) various declaration orders of i/byteOff/len (in/out of loop); (2) for-loop vs while-true; (3) ternary vs if-else for len computation; (4) keeping len at function scope; (5) using raw pointer arithmetic instead of struct accessor; (6) inserting `__asm__ volatile("" ::: "memory")` barrier (this fixed scheduling, putting `lw a1` reload into the b instruction's delay slot, dropping diff from 26 → 17 bytes); (7) `__asm__ volatile("" : "+r"(byteOff))` register pin attempts (made things worse); (8) swapping i++/byteOff += 16 order. Permuter ran 180s with sched=1 (auto-detected from filename) and could only improve from 149 → 148 byte diff under sched=1, suggesting sched=2 is correct (pragma in source).

The remaining 17 bytes are likely 4-5 instructions where t0/t1 register encoding bytes differ (slt, addu, addiu i++, addiu byteOff+=16, beqz, lw, and). All control flow, memory access patterns, and immediate constants match expected. No source-level fix appears to coax SNC into picking byteOff=t0 instead of t1. Possibly needs permuter run with sched=2 enforced, or the original may use a structurally different loop (e.g., recasting i as a pointer derivative).


## Disassembly

```asm
48cc8:	8c850124 	lw	a1,292(a0)
48ccc:	10a00006 	beqz	a1,0x48ce8
48cd0:	34060001 	li	a2,0x1
48cd4:	8ca6fffc 	lw	a2,-4(a1)
48cd8:	3c074000 	lui	a3,0x4000
48cdc:	24e7ffff 	addiu	a3,a3,-1
48ce0:	00c73024 	and	a2,a2,a3
48ce4:	2cc60001 	sltiu	a2,a2,1
48ce8:	14c00022 	bnez	a2,0x48d74
48cec:	00000000 	nop
48cf0:	908600d2 	lbu	a2,210(a0)
48cf4:	44806800 	mtc1	zero,$f13
48cf8:	a080013c 	sb	zero,316(a0)
48cfc:	34c60010 	ori	a2,a2,0x10
48d00:	a08600d2 	sb	a2,210(a0)
48d04:	3c063f80 	lui	a2,0x3f80
48d08:	44866000 	mtc1	a2,$f12
48d0c:	3c0b4000 	lui	t3,0x4000
48d10:	3c068000 	lui	a2,0x8000
48d14:	340a0000 	li	t2,0x0
48d18:	34080000 	li	t0,0x0
48d1c:	256bffff 	addiu	t3,t3,-1
48d20:	24c6ffff 	addiu	a2,a2,-1
48d24:	3c078000 	lui	a3,0x8000
48d28:	10a00003 	beqz	a1,0x48d38
48d2c:	34090000 	li	t1,0x0
48d30:	8ca9fffc 	lw	t1,-4(a1)
48d34:	012b4824 	and	t1,t1,t3
48d38:	0149482a 	slt	t1,t2,t1
48d3c:	1120000d 	beqz	t1,0x48d74
48d40:	00a82821 	addu	a1,a1,t0
48d44:	8ca9000c 	lw	t1,12(a1)
48d48:	aca00000 	sw	zero,0(a1)
48d4c:	e4ad0004 	swc1	$f13,4(a1)
48d50:	01274824 	and	t1,t1,a3
48d54:	e4ac0008 	swc1	$f12,8(a1)
48d58:	aca9000c 	sw	t1,12(a1)
48d5c:	01264824 	and	t1,t1,a2
48d60:	aca9000c 	sw	t1,12(a1)
48d64:	254a0001 	addiu	t2,t2,1
48d68:	25080010 	addiu	t0,t0,16
48d6c:	1000ffee 	b	0x48d28
48d70:	8c850124 	lw	a1,292(a0)
48d74:	03e00008 	jr	ra
48d78:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::ResetAnimationState(void)
// Address: 0x00048cc8, Size: 180B
// Obj: eAll_psp.obj

void eDynamicModel__ResetAnimationState_void(void *arg0) {
    s32 temp_t1;
    s32 var_a2;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    void *temp_a1;
    void *temp_a1_2;

    temp_a1 = arg0->unk124;
    var_a2 = 1;
    if (temp_a1 != NULL) {
        var_a2 = (temp_a1->unk-4 & 0x3FFFFFFF) == 0;
    }
    if (var_a2 == 0) {
        arg0->unk13C = 0;
        arg0->unkD2 = (u8) (arg0->unkD2 | 0x10);
        var_t2 = 0;
        var_t0 = 0;
loop_4:
        var_t1 = 0;
        if (arg0->unk124 != NULL) {
            var_t1 = arg0->unk124->unk-4 & 0x3FFFFFFF;
        }
        temp_a1_2 = arg0->unk124 + var_t0;
        if (var_t2 < var_t1) {
            temp_a1_2->unk0 = 0;
            temp_a1_2->unk4 = 0.0f;
            temp_t1 = temp_a1_2->unkC & 0x80000000;
            temp_a1_2->unk8 = 1.0f;
            temp_a1_2->unkC = temp_t1;
            temp_a1_2->unkC = (s32) (temp_t1 & 0x7FFFFFFF);
            var_t2 += 1;
            var_t0 += 0x10;
            goto loop_4;
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
