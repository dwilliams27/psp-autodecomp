# Battle Packet: `eCompoundShape::Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

## Target

- Address: `0x00073a40`
- Size: `136` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eCompoundShape`
- Method family: `Collide`
- Leaf: `False`

## Queue Metadata

- score: `131`
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

Callees:
- `0x00028bec` `untried` 592B `eCollision::ConvexHullCompound(const eConvexHullShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *) static`

## Placement

- Canonical source: `src/eCompoundShape.cpp`
- Header(s): `include/eCompoundShape.h`
- Allowed source(s): `src/eCompoundShape.cpp`
- Split-TU prefix: `src/eCompoundShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eCompoundShape.h`

```cpp
#pragma once

#include "eShape.h"

class cBase;
class cFile;
class cMemPool;
class cType;
class eBoxShape;
class eCapsuleShape;
class eCollisionContactInfo;
class eCompoundShape;
class eConvexHullShape;
class eCylinderShape;
class eHeightmapShape;
class eMeshShape;
class eMultiSphereShape;
class eShape;
class eSphereShape;
class mBox;
class mCollideInfo;
class mOCS;
class mRay;
class mSphere;
class mVec3;
class eContactCollector;
class eCollisionInfo;
class eDragAreaUtil;
class mPlane;

// eCompoundShape inherits eShape.
// eShape ctor stores cBase* at +0x00, vtable at +0x04, fields up to ~0x78.
// eCompoundShape ctor (0x72dd4) calls eShape::eShape, then:
//   sets vtable at +0x04
//   stores 0 at +0x80 (sub-shapes array pointer)
//   stores this at +0x84 (self-pointer / sentinel)
// GetAABB iterates sub-shapes via array at +0x80.
// GetNumSubShapes reads array count from *(array_ptr - 4).
// GetSubShape indexes into array at +0x80.
// Struct size: at least 0x88 bytes.
class eCompoundShape : public eShape {
public:
    eCompoundShape(cBase *);
    ~eCompoundShape();

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    static eCompoundShape *New(cMemPool *, cBase *);

    // virtual overrides from eShape
    void GetAABB(mBox *, const mOCS &) const;
    int CastRay(const mRay &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    void GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const;
    void GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const;

    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    void GetInertialTensor(float, mVec3 *) const;
    float GetVolume() const;
    void GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *, mVec3 *) const;
    void GetBoxShape(const mOCS &, eBoxShape *, mOCS *) const;
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    int GetNumSubShapes() const;
    eShape *GetSubShape(int) const;

    // eShape base _pad00 covers offsets 0x00..0x7F
    // +0x80: sub-shapes array (pointer to cArray of eShape*)
    // +0x84: self-pointer (set to this in ctor)
    eShape **mSubShapes;        // 0x80 (inherited _pad00 is 0x80 bytes from eShape)
    eCompoundShape *mSelf;      // 0x84
};
```

## Matched Same-Class Neighbors

- `0x00072ae0` 88B `eCompoundShape::Write(cFile &) const` — `src/eCompoundShape.cpp`
- `0x00072dd4` 60B `eCompoundShape::eCompoundShape(cBase *)` — `src/eCompoundShape.cpp`
- `0x00073ac8` 28B `eCompoundShape::Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eCompoundShape.cpp`
- `0x00073ae4` 28B `eCompoundShape::Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eCompoundShape.cpp`
- `0x00073b00` 28B `eCompoundShape::Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eCompoundShape.cpp`

## Matched Method Exemplars

### Exemplar 1: `eSphereShape::Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

- Address: `0x000683c0`
- Size: `140` bytes
- Source: `src/eSphereShape.cpp`

```cpp
int eSphereShape::Collide(const eMultiSphereShape *ms, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMultiSphere(*this, *ms, ocs1, ocs2, info);
}
```

### Exemplar 2: `eSphereShape::Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

- Address: `0x0006844c`
- Size: `140` bytes
- Source: `src/eSphereShape.cpp`

```cpp
int eSphereShape::Collide(const eMultiSphereShape *ms, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMultiSphere(*this, *ms, ocs1, ocs2, info);
}
```

### Exemplar 3: `eSphereShape::Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

- Address: `0x00068314`
- Size: `172` bytes
- Source: `src/eSphereShape.cpp`

```cpp
int eSphereShape::Collide(const eMultiSphereShape *ms, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMultiSphere(*this, *ms, ocs1, ocs2, info);
}
```

### Exemplar 4: `eCylinderShape::Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

- Address: `0x0007470c`
- Size: `172` bytes
- Source: `src/eCylinderShape.cpp`

```cpp
// eCylinderShape::Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x0007470c
// Generic dispatcher: virtual-calls shape's Collide(const eCylinderShape*, ...) at vtable
// offset 0xF8, passing args swapped (a/b swapped, ocs1/ocs2 swapped). Then negates each
// contact normal (first quad of each 0x40-stride contact starting at info+0x20).
#pragma control sched=1
int eCylinderShape::Collide(const eShape *shape, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    char *vtable = *(char **)((char *)shape + 4);
    CollideVtableEntry *entry = (CollideVtableEntry *)(vtable + 0xF8);
    void *adjThis = (char *)shape + entry->thisOffset;
    int hit = entry->fn(adjThis, this, b, a, &ocs2, &ocs1, info);
    if (hit != 0) {
        int i = 0;
        int count = *(int *)((char *)info + 0x14);
        if (i < count) {
            char *p = (char *)info + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
                count = *(int *)((char *)info + 0x14);
            } while (i < count);
        }
        return 1;
    }
    return 0;
}
```


## Prior Failure Notes

### Note 1

- session `2a5f6232`; src `src/eCompoundShape.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/2a5f6232/00073a40__agent_self_reported_failure__src_eCompoundShape.cpp`

Implemented the canonical negate-normal delegate in src/eCompoundShape.cpp: call the collision helper with swapped OCS arguments, then walk eCollisionContactInfo at info+0x20 with 0x40 stride and vneg.t each contact normal. Added local eCylinderShape/eConvexHullShape definitions inheriting eShape so ShapeCompound can accept the cylinder as an eShape, and a local mangled extern for eCollision::ConvexHullCompound because include/eCollision.h lacks that prototype. The file compiles with -Xsched=1 and the helper call/loop semantics are correct.

Closest output is 17/136 bytes off for both overloads. The stable nearest form (`int hit = call; int i = 0; if (hit != 0) ...`) fills the post-call beqz delay slot, but SNC assigns the loop counter/pointer as a0/a1 instead of expected a1/a0 and emits `move a3,a0` before `move s0,t2`; expected saves info first with `move s0,t2`. The direct `if (call != 0) { int i = 0; ... }` form gets the loop registers right but leaves a nop in the beqz delay slot and has a 79-byte diff. Tried declaration-order changes, early-return structure, inside-branch i initialization, fixed register hints, and empty-asm barriers; all either kept the 17-byte allocation drift or regressed to 79/100-byte diffs. The permuter CLI was attempted but is ambiguous for this file because three 136-byte symbols exist and it picked the sphere overload; a direct symbol-specific permuter invocation failed before candidate evaluation due Python multiprocessing from stdin on macOS. Root cause appears to be the known eAll_psp Collide family TU-context/register-allocation sensitivity rather than missing C semantics.


## Disassembly

```asm
73a40:	27bdfff0 	addiu	sp,sp,-16
73a44:	afb00000 	sw	s0,0(sp)
73a48:	afbf0004 	sw	ra,4(sp)
73a4c:	01408025 	move	s0,t2
73a50:	00803825 	move	a3,a0
73a54:	00a02025 	move	a0,a1
73a58:	00e02825 	move	a1,a3
73a5c:	01203825 	move	a3,t1
73a60:	0c00a2fb 	jal	0x28bec
73a64:	02004825 	move	t1,s0
73a68:	10400012 	beqz	v0,0x73ab4
73a6c:	34050000 	li	a1,0x0
73a70:	8e040014 	lw	a0,20(s0)
73a74:	00a4202a 	slt	a0,a1,a0
73a78:	10800009 	beqz	a0,0x73aa0
73a7c:	26040020 	addiu	a0,s0,32
73a80:	d8860000 	lv.q        C120, 0x0($a0)
73a84:	d0028606 	vneg.t      C120, C120
73a88:	f8860000 	sv.q        C120, 0x0($a0)
73a8c:	24a50001 	addiu	a1,a1,1
73a90:	8e060014 	lw	a2,20(s0)
73a94:	00a6302a 	slt	a2,a1,a2
73a98:	14c0fff9 	bnez	a2,0x73a80
73a9c:	24840040 	addiu	a0,a0,64
73aa0:	34020001 	li	v0,0x1
73aa4:	8fb00000 	lw	s0,0(sp)
73aa8:	8fbf0004 	lw	ra,4(sp)
73aac:	03e00008 	jr	ra
73ab0:	27bd0010 	addiu	sp,sp,16
73ab4:	00001025 	move	v0,zero
73ab8:	8fb00000 	lw	s0,0(sp)
73abc:	8fbf0004 	lw	ra,4(sp)
73ac0:	03e00008 	jr	ra
73ac4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eCompoundShape::Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x00073a40, Size: 136B
// Obj: eAll_psp.obj

s32 eCollision__ConvexHullCompound_consteConvexHullShaperef_consteCompoundShaperef_int_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(?, s32, ?); /* extern */

s32 eCompoundShape__Collide_consteConvexHullShapeptr_int_int_constmOCSref_constmOCSref_eCollisionContactInfoptrconst(s32 arg0, ? arg1) {
    s32 var_a1;
    void *temp_s0;

    temp_s0 = M2C_ERROR(/* Read from unset register $t2 */);
    var_a1 = 0;
    if (eCollision__ConvexHullCompound_consteConvexHullShaperef_consteCompoundShaperef_int_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(arg1, arg0, M2C_ERROR(/* Read from unset register $t1 */)) != 0) {
        if (temp_s0->unk14 > 0) {
            do {
                __asm__ volatile("lv.q C120, ($a0)");
                __asm__ volatile("vneg.t C120, C120");
                __asm__ volatile("sv.q C120, ($a0)");
                var_a1 += 1;
            } while (var_a1 < temp_s0->unk14);
        }
        return 1;
    }
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
