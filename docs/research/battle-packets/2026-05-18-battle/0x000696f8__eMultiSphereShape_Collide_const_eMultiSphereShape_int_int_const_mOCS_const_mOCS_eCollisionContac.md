# Battle Packet: `eMultiSphereShape::Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

## Target

- Address: `0x000696f8`
- Size: `140` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMultiSphereShape`
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
- `0x00027358` `untried` 668B `eCollision::MultiSphereMultiSphere(const eMultiSphereShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *) static`

## Placement

- Canonical source: `src/eMultiSphereShape.cpp`
- Header(s): `include/eMultiSphereShape.h`
- Allowed source(s): `src/eMultiSphereShape.cpp`, `src/eMultiSphereShape_Collide_eMultiSphereShape.cpp`
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

- session `6fa62855`; src `src/eMultiSphereShape_Collide_eMultiSphereShape.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/6fa62855/000696f8__agent_self_reported_failure__src_eMultiSphereShape_Collide_eMultiSphereShape.cpp`

Closest attempt is a split TU with a local eMultiSphereShape redeclaration and sched=1. It calls eCollision::MultiSphereMultiSphere(*shape, *this, ocs2, ocs1, info), then negates contact normals with the same small VFPU inline asm pattern used by nearby collision methods. A contact-pointer barrier fixed the call argument order, improving the diff from 21/140 to 17/140 bytes; sched=2 was worse, and a 120s permuter run found no improvement from the sched=2 baseline.

Remaining bytes are register-allocation/scheduling drift: expected moves t2 into s0 before saving this into a2, then uses a1 as the loop index and a0 as the count/pointer register. SNC consistently emits the same semantics with a0 as the loop index and a1 as the count/pointer register. Tried count locals inside/outside the hit block, do/while versus for, explicit pointer locals, register-variable hints, a contact barrier, and a same-TU experiment in src/eMultiSphereShape.cpp; the same-TU experiment produced the same 17-byte diff, so the split TU is not the root cause.


## Disassembly

```asm
696f8:	27bdfff0 	addiu	sp,sp,-16
696fc:	afb00000 	sw	s0,0(sp)
69700:	afbf0004 	sw	ra,4(sp)
69704:	01408025 	move	s0,t2
69708:	00803025 	move	a2,a0
6970c:	00a02025 	move	a0,a1
69710:	00c02825 	move	a1,a2
69714:	01203025 	move	a2,t1
69718:	01003825 	move	a3,t0
6971c:	0c009cd6 	jal	0x27358
69720:	02004025 	move	t0,s0
69724:	10400012 	beqz	v0,0x69770
69728:	34050000 	li	a1,0x0
6972c:	8e040014 	lw	a0,20(s0)
69730:	00a4202a 	slt	a0,a1,a0
69734:	10800009 	beqz	a0,0x6975c
69738:	26040020 	addiu	a0,s0,32
6973c:	d8860000 	lv.q        C120, 0x0($a0)
69740:	d0028606 	vneg.t      C120, C120
69744:	f8860000 	sv.q        C120, 0x0($a0)
69748:	24a50001 	addiu	a1,a1,1
6974c:	8e060014 	lw	a2,20(s0)
69750:	00a6302a 	slt	a2,a1,a2
69754:	14c0fff9 	bnez	a2,0x6973c
69758:	24840040 	addiu	a0,a0,64
6975c:	34020001 	li	v0,0x1
69760:	8fb00000 	lw	s0,0(sp)
69764:	8fbf0004 	lw	ra,4(sp)
69768:	03e00008 	jr	ra
6976c:	27bd0010 	addiu	sp,sp,16
69770:	00001025 	move	v0,zero
69774:	8fb00000 	lw	s0,0(sp)
69778:	8fbf0004 	lw	ra,4(sp)
6977c:	03e00008 	jr	ra
69780:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eMultiSphereShape::Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x000696f8, Size: 140B
// Obj: eAll_psp.obj

s32 eCollision__MultiSphereMultiSphere_consteMultiSphereShaperef_consteMultiSphereShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(?, s32, ?, ?); /* extern */

s32 eMultiSphereShape__Collide_consteMultiSphereShapeptr_int_int_constmOCSref_constmOCSref_eCollisionContactInfoptrconst(s32 arg0, ? arg1) {
    s32 var_a1;
    void *temp_s0;

    temp_s0 = M2C_ERROR(/* Read from unset register $t2 */);
    var_a1 = 0;
    if (eCollision__MultiSphereMultiSphere_consteMultiSphereShaperef_consteMultiSphereShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(arg1, arg0, M2C_ERROR(/* Read from unset register $t1 */), M2C_ERROR(/* Read from unset register $t0 */)) != 0) {
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
