# Battle Packet: `eDynamicModel::GetSurface(int) const`

## Target

- Address: `0x0004a070`
- Size: `224` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `GetSurface`
- Leaf: `True`

## Queue Metadata

- score: `181`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x001f84a0` `untried` 1640B `eCollisionIterator<eRayCollisionIterator, 1>::TraverseDynamicModel(const eGeom *, unsigned int, mVolume::mContainment)`
- `0x001fae64` `untried` 1644B `eCollisionIterator<eSphereCollisionIterator, 1>::TraverseDynamicModel(const eGeom *, unsigned int, mVolume::mContainment)`
- `0x001fd7c8` `untried` 1340B `eCollisionIterator<eSphereCullCollisionIterator, 0>::TraverseDynamicModel(const eGeom *, unsigned int, mVolume::mContainment)`
- `0x0020014c` `untried` 1332B `eCollisionIterator<eBoxCullCollisionIterator, 0>::TraverseDynamicModel(const eGeom *, unsigned int, mVolume::mContainment)`

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

## Matched Method Exemplars

### Exemplar 1: `eHeightmap::GetSurface(int) const`

- Address: `0x00053148`
- Size: `36` bytes
- Source: `src/eHeightmap.cpp`

```cpp
int eHeightmap::GetSurface(int idx) const {
    char *arr = *(char * volatile *)((char *)*(void **)((char *)this + 0x60) + 0x7C);
    char *entry = arr + idx * 20 + 12;
    return *(int *)entry;
}
```

### Exemplar 2: `eShape::GetSurface(void) const`

- Address: `0x001e0fb0`
- Size: `8` bytes
- Source: `src/eShape.cpp`

```cpp
void *eShape::GetSurface(void) const {
    return *(void **)((const char *)this + 0x6C);
}
```

### Exemplar 3: `eGeom::GetSurface(int) const`

- Address: `0x001e1e5c`
- Size: `8` bytes
- Source: `src/eGeom.cpp`

```cpp
eSurface *eGeom::GetSurface(int) const {
    return 0;
}
```


## Prior Failure Notes

### Note 1

- session `4852e10b`; src `src/eDynamicModel.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/4852e10b/0004a070__agent_self_reported_failure__src_eDynamicModel.cpp`

Implemented eDynamicModel::GetSurface(int) const in C++ using the duplicated D_00038890 handle resolution shown in the disassembly, volatile stack-zero returns, and a memory barrier before loading the surface array to prevent SNC from forming bnel with the array load in the delay slot. Tried the direct if(handle != 0) form, the matched sibling if(handle == 0)/else form from GetSubObjectIndex, void* vs char* entry types, explicit register-variable hints, local sched=1, and source ordering of len/arr. The best source currently emits the correct size and is an 8/224 byte mismatch after relocation masking.

The remaining GetSurface diff is not the documented bnel divergence. The 300s permuter run improved the function from 19 bytes different to 8 bytes different and saved the best source; a required 600s run from that 8-byte baseline generated 12048 candidates/6607 compiled with zero further improvement. Current mismatch is a narrow register-allocation issue in the first handle lookup: SNC keeps the first resolved pointer in a3 where the original uses a2, producing swapped a2/a3 encodings in the null check, field load, move, and subsequent bnez. All later array length/bounds/return code matches structurally.


## Disassembly

```asm
4a070:	27bdfff0 	addiu	sp,sp,-16
4a074:	8c84010c 	lw	a0,268(a0)
4a078:	14800003 	bnez	a0,0x4a088
4a07c:	3086ffff 	andi	a2,a0,0xffff
4a080:	1000000c 	b	0x4a0b4
4a084:	34060000 	li	a2,0x0
4a088:	3c070004 	lui	a3,0x4
4a08c:	00063080 	sll	a2,a2,0x2
4a090:	24e78890 	addiu	a3,a3,-30576
4a094:	00c73021 	addu	a2,a2,a3
4a098:	8cc70000 	lw	a3,0(a2)
4a09c:	10e00005 	beqz	a3,0x4a0b4
4a0a0:	34060000 	li	a2,0x0
4a0a4:	8ce80030 	lw	t0,48(a3)
4a0a8:	15040002 	bne	t0,a0,0x4a0b4
4a0ac:	00000000 	nop
4a0b0:	00e03025 	move	a2,a3
4a0b4:	14c00005 	bnez	a2,0x4a0cc
4a0b8:	00000000 	nop
4a0bc:	afa00000 	sw	zero,0(sp)
4a0c0:	8fa20000 	lw	v0,0(sp)
4a0c4:	03e00008 	jr	ra
4a0c8:	27bd0010 	addiu	sp,sp,16
4a0cc:	14800003 	bnez	a0,0x4a0dc
4a0d0:	3086ffff 	andi	a2,a0,0xffff
4a0d4:	1000000c 	b	0x4a108
4a0d8:	34060000 	li	a2,0x0
4a0dc:	3c070004 	lui	a3,0x4
4a0e0:	00063080 	sll	a2,a2,0x2
4a0e4:	24e78890 	addiu	a3,a3,-30576
4a0e8:	00c73021 	addu	a2,a2,a3
4a0ec:	8cc70000 	lw	a3,0(a2)
4a0f0:	10e00005 	beqz	a3,0x4a108
4a0f4:	34060000 	li	a2,0x0
4a0f8:	8ce80030 	lw	t0,48(a3)
4a0fc:	15040002 	bne	t0,a0,0x4a108
4a100:	00000000 	nop
4a104:	00e03025 	move	a2,a3
4a108:	8cc60044 	lw	a2,68(a2)
4a10c:	10c00005 	beqz	a2,0x4a124
4a110:	34040000 	li	a0,0x0
4a114:	8cc4fffc 	lw	a0,-4(a2)
4a118:	3c074000 	lui	a3,0x4000
4a11c:	24e7ffff 	addiu	a3,a3,-1
4a120:	00872024 	and	a0,a0,a3
4a124:	00a4202a 	slt	a0,a1,a0
4a128:	14800005 	bnez	a0,0x4a140
4a12c:	00052080 	sll	a0,a1,0x2
4a130:	afa00004 	sw	zero,4(sp)
4a134:	8fa20004 	lw	v0,4(sp)
4a138:	03e00008 	jr	ra
4a13c:	27bd0010 	addiu	sp,sp,16
4a140:	00c42021 	addu	a0,a2,a0
4a144:	8c820000 	lw	v0,0(a0)
4a148:	03e00008 	jr	ra
4a14c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::GetSurface(int) const
// Address: 0x0004a070, Size: 224B
// Obj: eAll_psp.obj

s32 eDynamicModel__GetSurface_intconst(void *arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;
    s32 temp_a0;
    s32 var_a0;
    void *temp_a2;
    void *temp_a3;
    void *temp_a3_2;
    void *var_a2;
    void *var_a2_2;

    temp_a0 = arg0->unk10C;
    if (temp_a0 == 0) {
        var_a2 = NULL;
    } else {
        temp_a3 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a2 = NULL;
        if ((temp_a3 != NULL) && (temp_a3->unk30 == temp_a0)) {
            var_a2 = temp_a3;
        }
    }
    if (var_a2 == NULL) {
        sp0 = 0;
        return sp0;
    }
    if (temp_a0 == 0) {
        var_a2_2 = NULL;
    } else {
        temp_a3_2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a2_2 = NULL;
        if ((temp_a3_2 != NULL) && (temp_a3_2->unk30 == temp_a0)) {
            var_a2_2 = temp_a3_2;
        }
    }
    temp_a2 = var_a2_2->unk44;
    var_a0 = 0;
    if (temp_a2 != NULL) {
        var_a0 = temp_a2->unk-4 & 0x3FFFFFFF;
    }
    if (arg1 >= var_a0) {
        sp4 = 0;
        return sp4;
    }
    return *(temp_a2 + (arg1 * 4));
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
