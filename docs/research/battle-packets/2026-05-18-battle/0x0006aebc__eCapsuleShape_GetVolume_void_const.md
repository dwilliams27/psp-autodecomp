# Battle Packet: `eCapsuleShape::GetVolume(void) const`

## Target

- Address: `0x0006aebc`
- Size: `72` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eCapsuleShape`
- Method family: `GetVolume`
- Leaf: `True`

## Queue Metadata

- score: `114`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 44B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `44`

## Failure Classification

- Primary: `constant_cse_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `constant_cse_codegen`
- Near miss: `44`
- Summary: Compiler hoists/reuses constants differently from target.
- Guidance: use as a focused research target, not a broad retry target.

## Placement

- Canonical source: `src/eCapsuleShape.cpp`
- Header(s): `include/eCapsuleShape.h`
- Allowed source(s): `src/eCapsuleShape.cpp`
- Split-TU prefix: `src/eCapsuleShape_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eCapsuleShape.h`

```cpp
#ifndef ECAPSULESHAPE_H
#define ECAPSULESHAPE_H

class cBase;
class cFile;
class cMemPool;
class cType;
struct mVec3;
struct mOCS;
class eShape;
class eBoxShape;
class eSphereShape;
class eMultiSphereShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;

class eCapsuleShape {
public:
    char _pad[0x80];
    float radius;       // offset 0x80
    float halfHeight;   // offset 0x84

    eCapsuleShape(cBase *);

    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    int CanSweep(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    int NeedsRollingFriction(float *) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;

    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eCapsuleShape *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00069f78` 100B `eCapsuleShape::Write(cFile &) const` — `src/eCapsuleShape.cpp`
- `0x00069fdc` 252B `eCapsuleShape::Read(cFile &, cMemPool *)` — `src/eCapsuleShape_Read.cpp`
- `0x0006a0d8` 68B `eCapsuleShape::eCapsuleShape(cBase *)` — `src/eCapsuleShape.cpp`
- `0x0006a11c` 124B `eCapsuleShape::~eCapsuleShape(void)` — `src/eCapsuleShape_dtor.cpp`
- `0x0006ad58` 36B `eCapsuleShape::Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eCapsuleShape.cpp`

## Matched Method Exemplars

### Exemplar 1: `eConvexHullShape::GetVolume(void) const`

- Address: `0x00071688`
- Size: `80` bytes
- Source: `src/eConvexHullShape.cpp`

```cpp
// eConvexHullShape::GetVolume(void) const @ 0x00071688
float eConvexHullShape::GetVolume(void) const {
    float tmp[4];
    if (*(float *)((char *)this + 0xA0) == 0.0f) {
        float mass = 1.0f;
        __asm__ volatile("" : "+f"(mass));
        GetInertialTensorAndVolume(mass, (mVec3 *)tmp, (float *)((char *)this + 0xA0));
    }
    return *(float *)((char *)this + 0xA0);
}
```

### Exemplar 2: `eBoxShape::GetVolume(void) const`

- Address: `0x0006de58`
- Size: `60` bytes
- Source: `src/eUser.cpp`

```cpp
float eBoxShape::GetVolume(void) const {
    int bits;
    float tmp[4];
    __asm__ volatile(
        "lui $a1, 0x4000\n"
        "mtc1 $a1, $f12\n"
        "mfc1 $a1, $f12\n"
        "mtv $a1, S100\n"
        : "=r"(bits) :: "$a1", "$f12"
    );
    __asm__ volatile(
        "lv.q C120, 0x80(%0)\n"
        "vscl.t C120, C120, S100\n"
        "sv.q C120, 0(%1)\n"
        :: "r"(this), "r"(tmp)
        : "memory"
    );
    return tmp[0] * tmp[1] * tmp[2];
}
```

### Exemplar 3: `eAudioGroup::GetVolume(void) const`

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

### Exemplar 4: `eCylinderShape::GetVolume(void) const`

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


## Prior Failure Notes

### Note 1

- session `40c4f721`; src `src/eCapsuleShape.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/40c4f721/0006aebc__agent_self_reported_failure__src_eCapsuleShape.cpp`

GetVolume — 44/72 bytes differ. Same as prior session (2aff2afe): every restructuring of the volume formula and every flag combination (-Xsched=0..3, -Xmopt=0, -Xxopt=0, -Xinline=0/1, -Xfcm=0/1, -Xalias=0/1, -O2..O5) produces identical output. Semantics are correct; only FPU instruction scheduling differs. Our SNC hoists the 4.1887903 constant (lui a0,0x4086 / ori / mtc1) into the gap between r2*pi and hh*2 muls, while expected loads it AFTER the r3 mul. Mirror sibling eMultiSphereShape::GetVolume has the same divergence. Verified compiler-version difference; no source-level fix reachable from C/C++.

### Note 2

- session `2aff2afe`

eCapsuleShape::GetVolume — 44/72 bytes differ. Tried several source forms (local r variable, direct radius*radius, single-return expression, various groupings) and all -Xsched levels (0..3), -Xmopt=0, -Xxopt=0, -Xinline=0/1, -Xfcm=0/1, -Xalias=0/1, and -O2..O5 — all produce identical output. The semantics are correct; only scheduling differs. My SNC hoists the `lui a1,0x4049` before `mul.s f13,f12,f12` and hoists `lui a0,0x4086`/`ori`/`mtc1 f17` into the gap between the r2*pi and hh*2 muls. Expected keeps the 4.1887903 constant load AFTER the r3 mul. Also, expected computes cyl BEFORE r3 (positions 11, 12) while mine puts r3 at position 13 and cyl at 15. eMultiSphereShape::GetVolume has identical source math and is also marked `failed` in the DB — same scheduler divergence. This looks like a compiler-version issue or hidden flag the project hasn't discovered yet; no source restructuring the agent can reach moves the remaining 11 words. Will match once the multisphere GetVolume scheduler mystery is solved (same class-likely same fix). Left the best attempt in place as decent decompiled source for training data.


## Disassembly

```asm
6aebc:	c48c0080 	lwc1	$f12,128(a0)
6aec0:	460c6342 	mul.s	$f13,$f12,$f12
6aec4:	3c054049 	lui	a1,0x4049
6aec8:	34a50fdb 	ori	a1,a1,0xfdb
6aecc:	44857000 	mtc1	a1,$f14
6aed0:	460e6b82 	mul.s	$f14,$f13,$f14
6aed4:	c48f0084 	lwc1	$f15,132(a0)
6aed8:	3c044000 	lui	a0,0x4000
6aedc:	44848000 	mtc1	a0,$f16
6aee0:	46107bc2 	mul.s	$f15,$f15,$f16
6aee4:	460f7002 	mul.s	$f0,$f14,$f15
6aee8:	460c6b02 	mul.s	$f12,$f13,$f12
6aeec:	3c044086 	lui	a0,0x4086
6aef0:	34840a92 	ori	a0,a0,0xa92
6aef4:	44848800 	mtc1	a0,$f17
6aef8:	46116302 	mul.s	$f12,$f12,$f17
6aefc:	03e00008 	jr	ra
6af00:	460c0000 	add.s	$f0,$f0,$f12
```

## m2c Starting Point

```c
// Decompiled: eCapsuleShape::GetVolume(void) const
// Address: 0x0006aebc, Size: 72B
// Obj: eAll_psp.obj

f32 eCapsuleShape__GetVolume_voidconst(void *arg0) {
    f32 temp_f12;
    f32 temp_f13;

    temp_f12 = arg0->unk80;
    temp_f13 = temp_f12 * temp_f12;
    return (temp_f13 * 3.1415927f * (arg0->unk84 * 2.0f)) + (temp_f13 * temp_f12 * 4.1887903f);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
