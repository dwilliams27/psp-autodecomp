# Battle Packet: `eSphereShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const`

## Target

- Address: `0x001e0d50`
- Size: `48` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSphereShape`
- Method family: `GetProjectedMinMax`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eSphereShape.cpp`
- Header(s): `include/eSphereShape.h`
- Allowed source(s): `src/eSphereShape.cpp`
- Split-TU prefix: `src/eSphereShape_*.cpp`

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

- session `fc8f129e`

Left the clean limited-inline-asm source in place; it remains at the known 8/48 byte mismatch where our output emits mtc1 a1,$f12 immediately after mfv a1,S100, while expected interleaves lwc1 $f13,0x74(a0) between mfv and mtc1. I tried integer-bit reinterpretation, radius-before-dot source ordering, FPU dependency barriers, fixed/register-bound FPU variables, read/write FPU asm operands, explicit lwc1+mtc1 asm outputs, and an asm-tail sanity test. The asm-tail test can force the desired lwc1/mtc1/sub/store sequence, but then inline asm cannot place the final swc1 in the jr delay slot unless it also takes over the return, which would be an asm-heavy/non-training match and was backed out.

The permuter was run for 300.2s with --save-best: 14,016 candidates generated, 339 compiled, best stayed at 8 bytes, improvements 0. Root cause appears to be an SNC scheduler/register-allocation quirk around the VFPU mfv -> FPU mtc1 transfer: C forms that keep the function clean either preserve the right FPU registers but schedule mtc1 too early, or force the radius load first but swap f12/f13.

### Note 2

- session `8d286f30`

GetProjectedMinMax: 8/48 bytes differ. Only 2 instructions swapped: expected has 'lwc1 f13,116(a0)' BEFORE 'mtc1 a1,f12' (lwc1 mRadius interleaved between mfv and mtc1 to fill mtc1 latency slot). Our output has mtc1 immediately after mfv, then lwc1.

Tried: (1) single asm with mfv+mtc1 inside (volatile asm is opaque — SNC cannot split them). (2) split into two asm blocks — first block outputs dotInt via '=r'(mfv $a1, S100), second block 'mtc1 %1,%0' with '=f' output. Still emits mtc1 immediately after mfv. (3) non-volatile mtc1 asm — same result. (4) Native VFPU types: 'typedef int vs_t __attribute__((mode(VS)))', asm outputs '=x'(dotVs), then 'float dot = __builtin_vstof(dotVs);'. SNC still emits mfv and mtc1 as a tight pair without scheduling lwc1 between them.

The mfv/mtc1 dependency chain through $a1 appears to cause SNC's scheduler to keep them adjacent even when it has free latitude (lwc1 f13 touches neither $a1 nor $f12, so it SHOULD be reorderable). This may be a scheduler quirk specific to the VFPU→FPU transfer pattern. Function is in eAll_psp.obj at 0x1e0d50 (outside the sched=1 zone) so sched=2 is correct. Possibly fixable by permuter exploring alternate source forms I didn't try — recommend running the permuter on this one.


## Disassembly

```asm
1e0d50:	d8c60030 	lv.q        C120, 0x30($a2)
1e0d54:	d8a70000 	lv.q        C130, 0x0($a1)
1e0d58:	64878604 	vdot.t      S100, C120, C130
1e0d5c:	48650004 	mfv         $a1, S100
1e0d60:	c48d0074 	lwc1	$f13,116(a0)
1e0d64:	44856000 	mtc1	a1,$f12
1e0d68:	460d6341 	sub.s	$f13,$f12,$f13
1e0d6c:	e4ed0000 	swc1	$f13,0(a3)
1e0d70:	c48d0074 	lwc1	$f13,116(a0)
1e0d74:	460d6300 	add.s	$f12,$f12,$f13
1e0d78:	03e00008 	jr	ra
1e0d7c:	e50c0000 	swc1	$f12,0(t0)
```

## m2c Starting Point

```c
// Decompiled: eSphereShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const
// Address: 0x001e0d50, Size: 48B
// Obj: eAll_psp.obj

void eSphereShape__GetProjectedMinMax_constmVec3ref_constmOCSref_floatptr_floatptrconst(void *arg0, f32 arg1, f32 *arg3) {
    __asm__ volatile("lv.q C120, 0x30($a2)");
    __asm__ volatile("lv.q C130, ($a1)");
    __asm__ volatile("vdot.t S100, C120, C130");
    __asm__ volatile("mfv $a1, S100");
    *arg3 = arg1 - arg0->unk74;
    *M2C_ERROR(/* Read from unset register $t0 */) = arg1 + arg0->unk74;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
