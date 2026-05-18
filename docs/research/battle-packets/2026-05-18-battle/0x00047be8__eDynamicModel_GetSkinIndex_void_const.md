# Battle Packet: `eDynamicModel::GetSkinIndex(void) const`

## Target

- Address: `0x00047be8`
- Size: `124` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicModel`
- Method family: `GetSkinIndex`
- Leaf: `True`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 11B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `11`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`, `needs_layout`
- Near miss: `11`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
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

- session `aa56511d`

Started from the prior best 11/124-byte mismatch. I verified sched=2 is still the best local pragma; changing the function to sched=1 worsened the preheader to 15 bytes. Manual variants included fixed-register locals for mask/byte offset/array base, tying those locals to empty asm operands, changing declaration order for mask/i/arr/byteOff, splitting the 0x3fffffff mask into 0x40000000 followed by decrement, and moving the byte offset before the array load. None produced the target register coloring. The stable best remains the known shape where arr is in a3 but mask and byte offset are swapped (mask in a1, byte offset in a2) compared with the target (mask in a2, byte offset in a1).

Ran tools/permuter.py for 300.7s with --save-best from the 11-byte baseline: 6720 candidates generated, 4002 compiled, zero improvements. This is not a bnel divergence; the remaining bytes are register-field differences and one preheader scheduling difference. I restored src/eDynamicModel.cpp to the clean pre-existing best state after the experiments.

### Note 2

- session `768a91f8`

eDynamicModel::GetSkinIndex(void) const. Started at 102/124 byte diff against EBOOT, reduced to 11/124 (~91% byte-identical) but could not close. Initial large diff was the missing SNC bool-cast pattern: target wraps the equality test in (unsigned char)(...==...) which forces the 4-instruction xor/sltiu/andi/bnez sequence instead of a direct bne. Adding that cast cut the diff to 12 bytes. The remaining 11 bytes are a register-allocation divergence: target keeps the cArrayBase length-mask constant (0x3FFFFFFF) hoisted into $a2 and uses $a1 for the running byte offset; our SNC swaps these (mask in $a1, byte offset in $a2). This swap shifts the lui/addiu mask materialisation and several subsequent register fields by one bit position in the encoding, producing 11 differing bytes spread across the loop body.

Approaches attempted: (1) declared mask before/after byteOff in source; (2) declared arr/byteOff/i in every permutation; (3) restructured the loop as while-true / for / do-while; (4) added #pragma control sched=2 directly above the function (this file is in the sched=2 zone but tested anyway); (5) inserted __asm__ volatile memory barriers between locals to discourage reordering; (6) ran tools/permuter.py twice (~120s + ~90s, ~3500 candidates total) — best result was the 11-byte version with an asm-barrier permutation, and the permuter saved that as src/eDynamicModel.cpp. None of these source-level mutations changed which physical register SNC picks for the mask vs the byte offset; the choice appears deterministic from the allocator's perspective for this loop shape. Likely fixable only by either a different loop topology that changes live ranges enough to flip the colouring, or by hand-rolled inline asm — neither of which produces real C/C++ training value. Marking failed for retry rather than unmatchable since the 11-byte diff exceeds the strict bnel-divergence criterion (≤8 bytes, instructions specifically bnel/beql vs bne/beq + nop).


## Disassembly

```asm
47be8:	8c870060 	lw	a3,96(a0)
47bec:	10e00006 	beqz	a3,0x47c08
47bf0:	3c064000 	lui	a2,0x4000
47bf4:	34020000 	li	v0,0x0
47bf8:	8ce7004c 	lw	a3,76(a3)
47bfc:	34050000 	li	a1,0x0
47c00:	10000003 	b	0x47c10
47c04:	24c6ffff 	addiu	a2,a2,-1
47c08:	03e00008 	jr	ra
47c0c:	2402ffff 	li	v0,-1
47c10:	10e00003 	beqz	a3,0x47c20
47c14:	34080000 	li	t0,0x0
47c18:	8ce8fffc 	lw	t0,-4(a3)
47c1c:	01064024 	and	t0,t0,a2
47c20:	0048402a 	slt	t0,v0,t0
47c24:	1100000d 	beqz	t0,0x47c5c
47c28:	00e54021 	addu	t0,a3,a1
47c2c:	8d080000 	lw	t0,0(t0)
47c30:	8c8900fc 	lw	t1,252(a0)
47c34:	01094026 	xor	t0,t0,t1
47c38:	2d080001 	sltiu	t0,t0,1
47c3c:	310800ff 	andi	t0,t0,0xff
47c40:	15000004 	bnez	t0,0x47c54
47c44:	00000000 	nop
47c48:	24420001 	addiu	v0,v0,1
47c4c:	1000fff0 	b	0x47c10
47c50:	24a50004 	addiu	a1,a1,4
47c54:	03e00008 	jr	ra
47c58:	00000000 	nop
47c5c:	03e00008 	jr	ra
47c60:	2402ffff 	li	v0,-1
```

## m2c Starting Point

```c
// Decompiled: eDynamicModel::GetSkinIndex(void) const
// Address: 0x00047be8, Size: 124B
// Obj: eAll_psp.obj

s32 eDynamicModel__GetSkinIndex_voidconst(void *arg0) {
    s32 var_a1;
    s32 var_t0;
    s32 var_v0;
    void *temp_a3;
    void *temp_a3_2;

    temp_a3 = arg0->unk60;
    if (temp_a3 != NULL) {
        var_v0 = 0;
        temp_a3_2 = temp_a3->unk4C;
        var_a1 = 0;
loop_3:
        var_t0 = 0;
        if (temp_a3_2 != NULL) {
            var_t0 = temp_a3_2->unk-4 & 0x3FFFFFFF;
        }
        if (var_v0 < var_t0) {
            if (!((*(temp_a3_2 + var_a1) == arg0->unkFC) & 0xFF)) {
                var_v0 += 1;
                var_a1 += 4;
                goto loop_3;
            }
            return var_v0;
        }
        return -1;
    }
    return -1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
