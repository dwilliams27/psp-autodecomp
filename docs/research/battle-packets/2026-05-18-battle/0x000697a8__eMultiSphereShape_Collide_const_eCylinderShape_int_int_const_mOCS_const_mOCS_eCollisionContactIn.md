# Battle Packet: `eMultiSphereShape::Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

## Target

- Address: `0x000697a8`
- Size: `140` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMultiSphereShape`
- Method family: `Collide`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 21B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `21`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `21`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00023c20` `untried` 552B `eCollision::ShapeMultiSphere(const eShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *) static`

## Placement

- Canonical source: `src/eMultiSphereShape.cpp`
- Header(s): `include/eMultiSphereShape.h`
- Allowed source(s): `src/eMultiSphereShape.cpp`, `src/eMultiSphereShape_Cylinder.cpp`
- Split-TU prefix: `src/eMultiSphereShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eMultiSphereShape.h`

```cpp
#ifndef EMULTISPHERESHAPE_H
#define EMULTISPHERESHAPE_H

struct mVec3;
struct mOCS;
class eShape;
class eBoxShape;
class eCapsuleShape;
class eCompoundShape;
class eConvexHullShape;
class eHeightmapShape;
class eMeshShape;
class eCollisionContactInfo;
class cBase;
class cFile;
class cMemPool;
class cType;

class eMultiSphereShape {
public:
    char _pad[0x80];
    float mRadius;       // 0x80
    float mHalfLength;   // 0x84

    eMultiSphereShape(cBase *);
    int CanSweep(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eMultiSphereShape *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x000688d0` 100B `eMultiSphereShape::Write(cFile &) const` — `src/eMultiSphereShape.cpp`
- `0x00068b74` 68B `eMultiSphereShape::eMultiSphereShape(cBase *)` — `src/eSweptSphereCollisionHandler.cpp`
- `0x00069784` 36B `eMultiSphereShape::Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eMultiSphereShape.cpp`
- `0x00069834` 36B `eMultiSphereShape::Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eMultiSphereShape.cpp`
- `0x00069858` 40B `eMultiSphereShape::Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eMultiSphereShape.cpp`

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

- session `fbd23a23`; src `src/eMultiSphereShape_Cylinder.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/fbd23a23/000697a8__agent_self_reported_failure__src_eMultiSphereShape_Cylinder.cpp`

Closest source is a split-TU canonical C++ method that calls eCollision::ShapeMultiSphere with swapped OCS args and negates contact normals with the expected VFPU vneg.t loop. Manual attempts included direct if-call form, explicit hit local to fill the beqz delay slot, reordered count/index/pointer locals, register hints, sched=1 vs sched=2, and a local context-seed experiment; best result is 21/140 bytes different. The current best has the right control-flow size and delay slots, but uses a0/a1 swapped through the loop and emits two pre-call setup moves in the opposite order from expected.

Ran the permuter for 300s with --save-best: 11,328 candidates generated, 5,581 compiled, zero improvements, best stayed at 21 bytes. This matches the documented 140B Collide context-dependent register-allocation drift from the 2026-05-05 session notes; source-level leverage appears exhausted short of recreating original TU allocator context or a compiler fix.


## Disassembly

```asm
697a8:	27bdfff0 	addiu	sp,sp,-16
697ac:	afb00000 	sw	s0,0(sp)
697b0:	afbf0004 	sw	ra,4(sp)
697b4:	01408025 	move	s0,t2
697b8:	00803025 	move	a2,a0
697bc:	00a02025 	move	a0,a1
697c0:	00c02825 	move	a1,a2
697c4:	01203025 	move	a2,t1
697c8:	01003825 	move	a3,t0
697cc:	0c008f08 	jal	0x23c20
697d0:	02004025 	move	t0,s0
697d4:	10400012 	beqz	v0,0x69820
697d8:	34050000 	li	a1,0x0
697dc:	8e040014 	lw	a0,20(s0)
697e0:	00a4202a 	slt	a0,a1,a0
697e4:	10800009 	beqz	a0,0x6980c
697e8:	26040020 	addiu	a0,s0,32
697ec:	d8860000 	lv.q        C120, 0x0($a0)
697f0:	d0028606 	vneg.t      C120, C120
697f4:	f8860000 	sv.q        C120, 0x0($a0)
697f8:	24a50001 	addiu	a1,a1,1
697fc:	8e060014 	lw	a2,20(s0)
69800:	00a6302a 	slt	a2,a1,a2
69804:	14c0fff9 	bnez	a2,0x697ec
69808:	24840040 	addiu	a0,a0,64
6980c:	34020001 	li	v0,0x1
69810:	8fb00000 	lw	s0,0(sp)
69814:	8fbf0004 	lw	ra,4(sp)
69818:	03e00008 	jr	ra
6981c:	27bd0010 	addiu	sp,sp,16
69820:	00001025 	move	v0,zero
69824:	8fb00000 	lw	s0,0(sp)
69828:	8fbf0004 	lw	ra,4(sp)
6982c:	03e00008 	jr	ra
69830:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eMultiSphereShape::Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x000697a8, Size: 140B
// Obj: eAll_psp.obj

s32 eCollision__ShapeMultiSphere_consteShaperef_consteMultiSphereShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(?, s32, ?, ?); /* extern */

s32 eMultiSphereShape__Collide_consteCylinderShapeptr_int_int_constmOCSref_constmOCSref_eCollisionContactInfoptrconst(s32 arg0, ? arg1) {
    s32 var_a1;
    void *temp_s0;

    temp_s0 = M2C_ERROR(/* Read from unset register $t2 */);
    var_a1 = 0;
    if (eCollision__ShapeMultiSphere_consteShaperef_consteMultiSphereShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(arg1, arg0, M2C_ERROR(/* Read from unset register $t1 */), M2C_ERROR(/* Read from unset register $t0 */)) != 0) {
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
