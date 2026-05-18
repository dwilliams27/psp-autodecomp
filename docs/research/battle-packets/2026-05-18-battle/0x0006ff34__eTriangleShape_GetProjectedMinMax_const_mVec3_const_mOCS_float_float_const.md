# Battle Packet: `eTriangleShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const`

## Target

- Address: `0x0006ff34`
- Size: `124` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eTriangleShape`
- Method family: `GetProjectedMinMax`
- Leaf: `True`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eTriangleShape.cpp`
- Header(s): `include/eTriangleShape.h`
- Allowed source(s): `src/eTriangleShape.cpp`
- Split-TU prefix: `src/eTriangleShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eTriangleShape.h`

```cpp
#ifndef ETRIANGLESHAPE_H
#define ETRIANGLESHAPE_H

class mSphere;
class mCollideInfo;
class eCollisionInfo;
class eCollisionContactInfo;
class mVec3;
class eContactCollector;
class eShape;
class mOCS;
class mPlane;
class eDragAreaUtil;

class eTriangleShape {
public:
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const;
    int GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    float GetVolume(void) const;
    void GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *, mVec3 *) const;
    int CanSweep(void) const;
};

#endif
```

## Matched Same-Class Neighbors

- `0x0006fbe8` 188B `eTriangleShape::Read(cFile &, cMemPool *)` — `src/eTriangleShape.cpp`
- `0x0006fe28` 8B `eTriangleShape::CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const` — `src/eTriangleShape_CastSphere.cpp`
- `0x0006fe30` 8B `eTriangleShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const` — `src/eTriangleShape_GetSweptContacts.cpp`
- `0x0006fe38` 8B `eTriangleShape::GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const` — `src/eTriangleShape_GetEmbedContacts.cpp`
- `0x0006fee0` 8B `eTriangleShape::Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eTriangleShape_Collide.cpp`

## Matched Method Exemplars

### Exemplar 1: `eShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const`

- Address: `0x001e0fdc`
- Size: `16` bytes
- Source: `src/eShape.cpp`

```cpp
void eShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *min, float *max) const {
    *max = 0.0f;
    *min = 0.0f;
}
```

### Exemplar 2: `eHeightmapShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const`

- Address: `0x001f4d94`
- Size: `8` bytes
- Source: `src/eHeightmapShape.cpp`

```cpp
void eHeightmapShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const {
}
```


## Prior Failure Notes

### Note 1

- session `1b76949c`

Kept the semantically correct C loop for eTriangleShape::GetProjectedMinMax, with small VFPU asm blocks only for the dot products. The stable generated code has the right stores, floating compares, bc1tl/bc1fl min/max updates, and loop shape, but SNC allocates the triangle vertex cursor in a2 and the loop counter in a0 while the target uses a0 for the cursor and a2 for the counter. This leaves the same core register-allocation mismatch as the prior session, plus the first addiu/lv.q order differs when the cursor is compiler-owned.

New attempts tried hardcoding the initial $a0 cursor update in inline asm, removing and adding $a0 clobbers, binding the cursor/counter through register asm outputs, reserving $a0 with a zero-instruction dummy asm, changing for versus do-while loop shape, explicit inline li for the counter, and .set noreorder around the first VFPU block. The hardcoded-cursor variants could make the first dot sequence or the loop cursor/counter split closer, but SNC then reused $a0/$a1 for C locals and corrupted the later dot inputs or added a precheck, so they were not left in the source. The remaining blocker is still SNC local register allocation around the inline VFPU asm, not the high-level min/max logic.

### Note 2

- session `ef3b4906`

Reached a stable 10/124-byte mismatch. The generated code has the correct control flow, stores, compares, branch-likely instructions, and VFPU dot-product sequence, but SNC allocates the triangle vertex pointer to a2 and the loop counter to a0; the target uses a0 for the vertex pointer and a2 for the loop counter. The remaining byte differences are those swapped register fields: addiu/lv.q use a2 where target uses a0, mfv/mtc1 use a0 where target uses a2 for the first dot, and loop increment/slti use a0 where target uses a2.

Manual attempts included fixed register locals with both "$a0"/"$a2" and "a0"/"a2" spellings, swapping inline-asm output operand order, hardcoding the VFPU transfer scratch registers ($a2/$t1), removing fixed register locals, changing sched scope for New versus the sched=1 projection function, and preserving the C loop/min/max structure to avoid pure assembly. The permuter then ran for 300 seconds (12,360 candidates generated, 3,311 compiled) and found zero improvement. Root cause appears to be local register allocation around the inline VFPU dot asm rather than expression order or branch structure.


## Disassembly

```asm
6ff34:	d8a60000 	lv.q        C120, 0x0($a1)
6ff38:	24840080 	addiu	a0,a0,128
6ff3c:	d8870000 	lv.q        C130, 0x0($a0)
6ff40:	64878604 	vdot.t      S100, C120, C130
6ff44:	48660004 	mfv         $a2, S100
6ff48:	44866000 	mtc1	a2,$f12
6ff4c:	e4ec0000 	swc1	$f12,0(a3)
6ff50:	e50c0000 	swc1	$f12,0(t0)
6ff54:	34060001 	li	a2,0x1
6ff58:	24840010 	addiu	a0,a0,16
6ff5c:	d8860000 	lv.q        C120, 0x0($a0)
6ff60:	d8a70000 	lv.q        C130, 0x0($a1)
6ff64:	64868704 	vdot.t      S100, C130, C120
6ff68:	48690004 	mfv         $t1, S100
6ff6c:	44896000 	mtc1	t1,$f12
6ff70:	c4ed0000 	lwc1	$f13,0(a3)
6ff74:	460d603c 	c.lt.s	$f12,$f13
6ff78:	00000000 	nop
6ff7c:	45030001 	bc1tl	0x6ff84
6ff80:	e4ec0000 	swc1	$f12,0(a3)
6ff84:	c50d0000 	lwc1	$f13,0(t0)
6ff88:	460d603e 	c.le.s	$f12,$f13
6ff8c:	00000000 	nop
6ff90:	45020001 	bc1fl	0x6ff98
6ff94:	e50c0000 	swc1	$f12,0(t0)
6ff98:	24c60001 	addiu	a2,a2,1
6ff9c:	28c90003 	slti	t1,a2,3
6ffa0:	1520ffee 	bnez	t1,0x6ff5c
6ffa4:	24840010 	addiu	a0,a0,16
6ffa8:	03e00008 	jr	ra
6ffac:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eTriangleShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const
// Address: 0x0006ff34, Size: 124B
// Obj: eAll_psp.obj

void eTriangleShape__GetProjectedMinMax_constmVec3ref_constmOCSref_floatptr_floatptrconst(s32 arg0, f32 arg2, f32 *arg3) {
    f32 temp_f12;
    s32 var_a2;

    __asm__ volatile("lv.q C120, ($a1)");
    __asm__ volatile("lv.q C130, ($a0)");
    __asm__ volatile("vdot.t S100, C120, C130");
    __asm__ volatile("mfv $a2, S100");
    *arg3 = arg2;
    *M2C_ERROR(/* Read from unset register $t0 */) = arg2;
    var_a2 = 1;
    do {
        __asm__ volatile("lv.q C120, ($a0)");
        __asm__ volatile("lv.q C130, ($a1)");
        __asm__ volatile("vdot.t S100, C130, C120");
        __asm__ volatile("mfv $t1, S100");
        temp_f12 = M2C_ERROR(/* Read from unset register $t1 */);
        if (temp_f12 < *arg3) {
            *arg3 = temp_f12;
        }
        if (!(temp_f12 <= *M2C_ERROR(/* Read from unset register $t0 */))) {
            *M2C_ERROR(/* Read from unset register $t0 */) = temp_f12;
        }
        var_a2 += 1;
    } while (var_a2 < 3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
