# Battle Packet: `eMultiSphereShape::GetVolume(void) const`

## Target

- Address: `0x00069998`
- Size: `72` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMultiSphereShape`
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

- Canonical source: `src/eMultiSphereShape.cpp`
- Header(s): `include/eMultiSphereShape.h`
- Allowed source(s): `src/eMultiSphereShape.cpp`
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

- session `20de5b46`; src `src/eMultiSphereShape.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/20de5b46/00069998__agent_self_reported_failure__src_eMultiSphereShape.cpp`

eMultiSphereShape::GetVolume(void) const (72B) — formula pi*r^2*(2h) + (4/3)*pi*r^3 where r=mRadius@0x80, h=mHalfLength@0x84. Implemented as: r2 = mRadius*mRadius; r2pi = r2*3.1415927f; doubled = mHalfLength*2.0f; cyl = r2pi*doubled; r3 = r2*mRadius; return cyl + r3*4.1887903f; — under #pragma control sched=1. Result: 44/72 bytes differ (essentially the entire function). Pure FPU instruction scheduling issue — SNC reorders the multiply chain. Tried multiple expression decompositions (separated `2*h` early, fused steps, alt constant order). Whole-file -Xsched=1 build was tested for Function 3 and confirmed Makefile override would NOT fix scheduling — the issue is deeper than a sched mode setting. See docs/research/snc-fpu-scheduling.md before next attempt. Next agent: try permuter from current decomposition; consider if 4.1887903f and 3.1415927f need different literal forms (PI vs 4*PI/3 expressed as combined constant). All edits reverted.

### Note 2

- session `360bcb4f`

GetVolume: 72B function, 12 of 18 words (~48B) differ even with #pragma control sched=1 wrapping. The function formula (V = pi*r^2 * (2h) + (4/3)*pi*r^3) is correctly expressed as C source with all constants (3.1415927f, 2.0f, 4.1887903f) matching the expected hex. The issue is pure instruction scheduling: SNC at -Xsched=2 (file default) aggressively interleaves lwc1/lui/ori/mtc1 loads with the mul.s FPU operations, while the expected output has less-aggressive scheduling where r2*pi is computed, THEN halfLength is loaded, THEN doubled is computed, THEN the cyl multiply. Adding '#pragma control sched=1' reduced the diff from 13 to 12 mismatched words but did not close the gap, suggesting the pragma alone does not fully emulate whole-file -Xsched=1 compilation context (the transition-zone hint in the task warned of this). The single mul.s f0 operand order also differs (f15,f14 vs expected f14,f15) — tried swapping source expression order (doubled*r2pi vs r2pi*doubled) with no change; SNC always commutes it to the scheduler's preferred form. Tried: (1) baseline sched=2 (53B diff), (2) #pragma sched=1 (48B diff), (3) swapping cyl operand order. NEXT AGENT: per the task's SCHED HINT, this class likely needs a whole-file Makefile override: add '$(BUILD_DIR)/src/eMultiSphereShape%.o: CFLAGS := $(ECFLAGS)' under the existing sched=1 overrides in the Makefile. If that doesn't fix it, the exact expression tree may need restructuring (try a single return expression with explicit parens, or try float-typed intermediate casts). Do NOT modify the Makefile from within the agent — flag it for a human.


## Disassembly

```asm
69998:	c48c0080 	lwc1	$f12,128(a0)
6999c:	460c6342 	mul.s	$f13,$f12,$f12
699a0:	3c054049 	lui	a1,0x4049
699a4:	34a50fdb 	ori	a1,a1,0xfdb
699a8:	44857000 	mtc1	a1,$f14
699ac:	460e6b82 	mul.s	$f14,$f13,$f14
699b0:	c48f0084 	lwc1	$f15,132(a0)
699b4:	3c044000 	lui	a0,0x4000
699b8:	44848000 	mtc1	a0,$f16
699bc:	46107bc2 	mul.s	$f15,$f15,$f16
699c0:	460f7002 	mul.s	$f0,$f14,$f15
699c4:	460c6b02 	mul.s	$f12,$f13,$f12
699c8:	3c044086 	lui	a0,0x4086
699cc:	34840a92 	ori	a0,a0,0xa92
699d0:	44848800 	mtc1	a0,$f17
699d4:	46116302 	mul.s	$f12,$f12,$f17
699d8:	03e00008 	jr	ra
699dc:	460c0000 	add.s	$f0,$f0,$f12
```

## m2c Starting Point

```c
// Decompiled: eMultiSphereShape::GetVolume(void) const
// Address: 0x00069998, Size: 72B
// Obj: eAll_psp.obj

f32 eMultiSphereShape__GetVolume_voidconst(void *arg0) {
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
