# Battle Packet: `eConvexHullShape::Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

## Target

- Address: `0x00071424`
- Size: `140` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eConvexHullShape`
- Method family: `Collide`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 21B; correct function size; codegen-specific diagnosis; manageable size`
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
- `0x000277e8` `untried` 500B `eCollision::MultiSphereConvexHull(const eMultiSphereShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *) static`

## Placement

- Canonical source: `src/eConvexHullShape.cpp`
- Header(s): `include/eConvexHullShape.h`
- Allowed source(s): `src/eConvexHullShape.cpp`
- Split-TU prefix: `src/eConvexHullShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eConvexHullShape.h`

Not found.

## Matched Same-Class Neighbors

- `0x000707d4` 92B `eConvexHullShape::eConvexHullShape(cBase *)` — `src/eConvexHullShape.cpp`
- `0x00071618` 40B `eConvexHullShape::Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/gcCameraModes_OnEntitySnappedFollowWithAvoidancegcCameragcCamera.cpp`
- `0x00071640` 72B `eConvexHullShape::GetInertialTensor(float, mVec3 *) const` — `src/eConvexHullShape.cpp`
- `0x00071688` 80B `eConvexHullShape::GetVolume(void) const` — `src/eConvexHullShape.cpp`
- `0x0020b024` 124B `eConvexHullShape::New(cMemPool *, cBase *) static` — `src/eConvexHullShape.cpp`

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

- session `9397cdc0`; src `src/eConvexHullShape.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/9397cdc0/00071424__agent_self_reported_failure__src_eConvexHullShape.cpp`

Closest source is the standard negate-normal delegate placed in the eConvexHullShape low-address method cluster: call eCollision::MultiSphereConvexHull(*shape, *this, ocs2, ocs1, info), then loop over info+0x20 with stride 0x40 and vneg.t each contact normal. This compiles to the correct size and all existing eConvexHullShape/eKeyframedController matches in the file remain matched, but the target remains 21/140 bytes different. The diff is register/scheduling only: setup around the collision call has move s0,t2 and a2/a3 ordering different from expected, and the loop uses a0 for the counter and a1 for the contact pointer where expected uses a1 for the counter and a0 for the pointer.

Tried the direct exemplar form, explicit hit local with loop index before and inside the hit block, early-return control flow, predeclared pointer/index locals, swapped comparison spelling, moving the method into original address order near ctor/GetInertialTensor, and one bounded permuter run. The permuter generated 9504 candidates, compiled 4356, and found no improvement over the 21-byte baseline. This appears to be the known eAll 140-byte collide delegate context-seeded register allocation issue described in docs/sessions/2026-05-03.md.


## Disassembly

```asm
71424:	27bdfff0 	addiu	sp,sp,-16
71428:	afb00000 	sw	s0,0(sp)
7142c:	afbf0004 	sw	ra,4(sp)
71430:	01408025 	move	s0,t2
71434:	00803025 	move	a2,a0
71438:	00a02025 	move	a0,a1
7143c:	00c02825 	move	a1,a2
71440:	01203025 	move	a2,t1
71444:	01003825 	move	a3,t0
71448:	0c009dfa 	jal	0x277e8
7144c:	02004025 	move	t0,s0
71450:	10400012 	beqz	v0,0x7149c
71454:	34050000 	li	a1,0x0
71458:	8e040014 	lw	a0,20(s0)
7145c:	00a4202a 	slt	a0,a1,a0
71460:	10800009 	beqz	a0,0x71488
71464:	26040020 	addiu	a0,s0,32
71468:	d8860000 	lv.q        C120, 0x0($a0)
7146c:	d0028606 	vneg.t      C120, C120
71470:	f8860000 	sv.q        C120, 0x0($a0)
71474:	24a50001 	addiu	a1,a1,1
71478:	8e060014 	lw	a2,20(s0)
7147c:	00a6302a 	slt	a2,a1,a2
71480:	14c0fff9 	bnez	a2,0x71468
71484:	24840040 	addiu	a0,a0,64
71488:	34020001 	li	v0,0x1
7148c:	8fb00000 	lw	s0,0(sp)
71490:	8fbf0004 	lw	ra,4(sp)
71494:	03e00008 	jr	ra
71498:	27bd0010 	addiu	sp,sp,16
7149c:	00001025 	move	v0,zero
714a0:	8fb00000 	lw	s0,0(sp)
714a4:	8fbf0004 	lw	ra,4(sp)
714a8:	03e00008 	jr	ra
714ac:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eConvexHullShape::Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x00071424, Size: 140B
// Obj: eAll_psp.obj

s32 eCollision__MultiSphereConvexHull_consteMultiSphereShaperef_consteConvexHullShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(?, s32, ?, ?); /* extern */

s32 eConvexHullShape__Collide_consteMultiSphereShapeptr_int_int_constmOCSref_constmOCSref_eCollisionContactInfoptrconst(s32 arg0, ? arg1) {
    s32 var_a1;
    void *temp_s0;

    temp_s0 = M2C_ERROR(/* Read from unset register $t2 */);
    var_a1 = 0;
    if (eCollision__MultiSphereConvexHull_consteMultiSphereShaperef_consteConvexHullShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(arg1, arg0, M2C_ERROR(/* Read from unset register $t1 */), M2C_ERROR(/* Read from unset register $t0 */)) != 0) {
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
