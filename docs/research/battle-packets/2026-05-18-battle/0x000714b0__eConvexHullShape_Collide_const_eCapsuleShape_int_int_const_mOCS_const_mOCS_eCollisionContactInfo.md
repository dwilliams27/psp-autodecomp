# Battle Packet: `eConvexHullShape::Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const`

## Target

- Address: `0x000714b0`
- Size: `140` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eConvexHullShape`
- Method family: `Collide`
- Leaf: `False`

## Queue Metadata

- score: `139`
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
- `0x00094c38` `untried` 2372B `eCollision::CapsuleConvexHull(const eCapsuleShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *) static`

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

- session `0624aacd`; src `src/eConvexHullShape.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/0624aacd/000714b0__agent_self_reported_failure__src_eConvexHullShape.cpp`

Implemented the canonical negate-normal delegate in C++: call eCollision::CapsuleConvexHull(*shape, *this, ocs2, ocs1, info), then loop over contact normals at info+0x20 with stride 0x40 using only the required VFPU lv.q/vneg.t/sv.q inline asm. The closest attempt is 21/140 bytes different. The remaining differences are register-allocation and scheduling only: my build emits move a2,a0 before move s0,t2, assigns a3=t0 before a2=t1, and uses a0 as the loop counter with a1 as the contact pointer; the target uses s0 first, assigns a2=t1 before a3=t0, and uses a1 as the counter with a0 as the pointer. Control flow, call target, contact-count reloads, and return paths are otherwise the same shape.

Tried the direct-if source shape from the matched eSphereShape 140B delegates, scoping the counter inside the hit branch, declaring the pointer before the counter, and fixed-register variables/raw $a0 VFPU asm. These either worsened to 47/83/87-byte diffs or produced a different loop CFG. A 300s permuter run generated 9,312 candidates, compiled 4,432, and found zero improvement over the 21-byte baseline. This appears to be the documented eConvexHullShape 140B Collide context/register-allocation drift rather than missing C logic.


## Disassembly

```asm
714b0:	27bdfff0 	addiu	sp,sp,-16
714b4:	afb00000 	sw	s0,0(sp)
714b8:	afbf0004 	sw	ra,4(sp)
714bc:	01408025 	move	s0,t2
714c0:	00803025 	move	a2,a0
714c4:	00a02025 	move	a0,a1
714c8:	00c02825 	move	a1,a2
714cc:	01203025 	move	a2,t1
714d0:	01003825 	move	a3,t0
714d4:	0c02530e 	jal	0x94c38
714d8:	02004025 	move	t0,s0
714dc:	10400012 	beqz	v0,0x71528
714e0:	34050000 	li	a1,0x0
714e4:	8e040014 	lw	a0,20(s0)
714e8:	00a4202a 	slt	a0,a1,a0
714ec:	10800009 	beqz	a0,0x71514
714f0:	26040020 	addiu	a0,s0,32
714f4:	d8860000 	lv.q        C120, 0x0($a0)
714f8:	d0028606 	vneg.t      C120, C120
714fc:	f8860000 	sv.q        C120, 0x0($a0)
71500:	24a50001 	addiu	a1,a1,1
71504:	8e060014 	lw	a2,20(s0)
71508:	00a6302a 	slt	a2,a1,a2
7150c:	14c0fff9 	bnez	a2,0x714f4
71510:	24840040 	addiu	a0,a0,64
71514:	34020001 	li	v0,0x1
71518:	8fb00000 	lw	s0,0(sp)
7151c:	8fbf0004 	lw	ra,4(sp)
71520:	03e00008 	jr	ra
71524:	27bd0010 	addiu	sp,sp,16
71528:	00001025 	move	v0,zero
7152c:	8fb00000 	lw	s0,0(sp)
71530:	8fbf0004 	lw	ra,4(sp)
71534:	03e00008 	jr	ra
71538:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eConvexHullShape::Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x000714b0, Size: 140B
// Obj: eAll_psp.obj

s32 eCollision__CapsuleConvexHull_consteCapsuleShaperef_consteConvexHullShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(?, s32, ?, ?); /* extern */

s32 eConvexHullShape__Collide_consteCapsuleShapeptr_int_int_constmOCSref_constmOCSref_eCollisionContactInfoptrconst(s32 arg0, ? arg1) {
    s32 var_a1;
    void *temp_s0;

    temp_s0 = M2C_ERROR(/* Read from unset register $t2 */);
    var_a1 = 0;
    if (eCollision__CapsuleConvexHull_consteCapsuleShaperef_consteConvexHullShaperef_constmOCSref_constmOCSref_eCollisionContactInfoptrstatic(arg1, arg0, M2C_ERROR(/* Read from unset register $t1 */), M2C_ERROR(/* Read from unset register $t0 */)) != 0) {
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
