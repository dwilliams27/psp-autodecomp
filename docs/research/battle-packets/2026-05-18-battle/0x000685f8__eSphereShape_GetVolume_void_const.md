# Battle Packet: `eSphereShape::GetVolume(void) const`

## Target

- Address: `0x000685f8`
- Size: `32` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSphereShape`
- Method family: `GetVolume`
- Leaf: `True`

## Queue Metadata

- score: `163`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 2B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eSphereShape.cpp`
- Header(s): `include/eSphereShape.h`
- Allowed source(s): `src/eSphereShape.cpp`
- Split-TU prefix: `src/eSphereShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eSphereShape.h`

```cpp
#ifndef ESPHERESHAPE_H
#define ESPHERESHAPE_H

struct mVec3;
struct mOCS;
struct mBox;
class cBase;
class cFile;
class cMemPool;
class cType;
class eShape;
class eBoxShape;
class eCollisionSupport;
class eMultiSphereShape;
class eCapsuleShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;

class eSphereShape {
public:
    char _pad[0x74];
    float mRadius;  // 0x74

    eSphereShape(cBase *);

    void AssignCopy(const cBase *);
    int CanSweep(void) const;
    const cType *GetType(void) const;
    float GetVolume(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    int NeedsRollingFriction(float *) const;
    void GetAABB(mBox *, const mOCS &) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, eCollisionSupport *out) const;
    void GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const;
    void GetInertialTensor(float mass, mVec3 *out) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eSphereShape *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00067afc` 88B `eSphereShape::Write(cFile &) const` — `src/eSphereShape.cpp`
- `0x00067b54` 216B `eSphereShape::Read(cFile &, cMemPool *)` — `src/eSphereShape.cpp`
- `0x00067c2c` 52B `eSphereShape::eSphereShape(cBase *)` — `src/eSphereShape.cpp`
- `0x00067c60` 124B `eSphereShape::~eSphereShape(void)` — `src/eSphereShape_dtor.cpp`
- `0x00067cdc` 172B `eSphereShape::GetCollisionHandler(const mCollideInfo &) const` — `src/gcUI.cpp`

## Matched Method Exemplars

### Exemplar 1: `eAudioGroup::GetVolume(void) const`

- Address: `0x0001d6c0`
- Size: `48` bytes
- Source: `src/eAudioGroup.cpp`

```cpp
// eAudioGroup::GetVolume() const @ 0x0001d6c0 (48 bytes)
// ----------------------------------------------------------------------
float eAudioGroup::GetVolume() const {
    float v = 1.0f;
    for (int i = 0; i < 3; i++) {
        v *= mFaders[i].mCurrent;
    }
    return v;
}
```

### Exemplar 2: `eCylinderShape::GetVolume(void) const`

- Address: `0x00074828`
- Size: `48` bytes
- Source: `src/eCylinderShape.cpp`

```cpp
// eCylinderShape::GetVolume(void) const — 0x00074828
#pragma control sched=1
float eCylinderShape::GetVolume(void) const {
    float radiusSq = mRadius * mRadius;
    __asm__ volatile("" : "+f"(radiusSq));

    float area = radiusSq * 3.1415927f;
    __asm__ volatile("" : "+f"(area));

    float height = mHalfHeight * 2.0f;
    __asm__ volatile("" : "+f"(height));

    return area * height;
}
```

### Exemplar 3: `eTriangleShape::GetVolume(void) const`

- Address: `0x0006ff24`
- Size: `8` bytes
- Source: `src/eTriangleShape_GetVolume.cpp`

```cpp
float eTriangleShape::GetVolume(void) const {
    return 0.0f;
}
```

### Exemplar 4: `eMeshShape::GetVolume(void) const`

- Address: `0x001f4b68`
- Size: `8` bytes
- Source: `src/eMeshShape_GetVolume.cpp`

```cpp
float eMeshShape::GetVolume(void) const {
    return 0.0f;
}
```


## Prior Failure Notes

### Note 1

- session `971c684b`

Improved eSphereShape::GetVolume from the prior 19/32-byte mismatch to a 2/32-byte near-match by splitting the cube into temporaries and adding empty FPU operand barriers before the constant multiply. The current output has the expected constant scheduling and final multiply: lwc1 f12, mul.s f13,f12,f12, constant loaded into f14 after the cube, and the return-delay-slot mul.s f0,f0,f14.

The only remaining difference is the second multiply operand order: expected encodes mul.s f0,f13,f12, while SNC continues to emit mul.s f0,f12,f13. I tried chained expressions, explicit temps, reversed source operands, duplicated radius temporaries, declaration-order changes, +f empty asm constraints in both operand orders, sched=1 manual compilation, and a 300s permuter run from the 2-byte baseline (4023 compiled candidates, 0 improvements). This appears to be a commutative FPU operand-order codegen divergence; matching it without real FPU inline asm does not look possible from C source shapes tested.

### Note 2

- session `8d286f30`

GetVolume: 19/32 bytes differ. Function is trivially 3 mul.s + constant multiply. Tried 4 source expression forms: (1) chained 'r*r*r*C' single expr, (2) paren-grouped '(r*r*r)*C', (3) explicit temp variables 'r2=r*r; r3=r2*r; r3*C', (4) reversed 'C*(r*r*r)'. ALL produce identical SNC output with -Xsched=2.

Diff analysis: expected computes mul.s f13,f12,f12 then mul.s f0,f13,f12 (triple product into $f0) and loads the constant into $f14 AFTER the triple product completes. Our output loads the constant into $f0 interleaved between multiplies (lui, mul, ori, mtc1, mul), resulting in triple product going to $f12 and final mul.s f0 = f12 * f0.

This is a register allocation / instruction scheduling difference that source restructuring cannot change. The expected scheduling pattern (compute all muls first, then constant) matches the less-aggressive -Xsched=1 behavior described in docs/research/snc-fpu-scheduling.md. Function is at 0x685f8 in the transition zone (0x040000-0x06e000) — SCHED HINT in the task suggested sched=1 may be needed for this class but agents cannot modify the Makefile. Recommend a human add an eSphereShape% sched=1 override to Makefile and re-run, BUT note that GetSupport (0x6883c) and Collide (0x684d8) in the same class match cleanly with sched=2, so the entire class cannot simply be switched.


## Disassembly

```asm
685f8:	c48c0074 	lwc1	$f12,116(a0)
685fc:	460c6342 	mul.s	$f13,$f12,$f12
68600:	460c6802 	mul.s	$f0,$f13,$f12
68604:	3c044086 	lui	a0,0x4086
68608:	34840a92 	ori	a0,a0,0xa92
6860c:	44847000 	mtc1	a0,$f14
68610:	03e00008 	jr	ra
68614:	460e0002 	mul.s	$f0,$f0,$f14
```

## m2c Starting Point

```c
// Decompiled: eSphereShape::GetVolume(void) const
// Address: 0x000685f8, Size: 32B
// Obj: eAll_psp.obj

f32 eSphereShape__GetVolume_voidconst(void *arg0) {
    f32 temp_f12;

    temp_f12 = arg0->unk74;
    return temp_f12 * temp_f12 * temp_f12 * 4.1887903f;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
