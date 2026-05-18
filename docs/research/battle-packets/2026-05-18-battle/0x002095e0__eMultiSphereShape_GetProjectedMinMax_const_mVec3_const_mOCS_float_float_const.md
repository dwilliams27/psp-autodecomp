# Battle Packet: `eMultiSphereShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const`

## Target

- Address: `0x002095e0`
- Size: `144` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMultiSphereShape`
- Method family: `GetProjectedMinMax`
- Leaf: `True`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eMultiSphereShape.cpp`
- Header(s): `include/eMultiSphereShape.h`
- Allowed source(s): `src/eMultiSphereShape.cpp`
- Split-TU prefix: `src/eMultiSphereShape_*.cpp`

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

- session `23cb19c9`

Kept the existing reconstructed algorithm and retested it against expected/bin/002095e0.bin. I also tried moving the VFPU dot outputs through integer bit temporaries so SNC's normal float-use path, rather than the inline-asm FPU output allocator, would choose the mtc1 destination. That produced identical code to the previous near miss: 13 differing bytes, all the same f13/f14 allocation swap. Expected has dot1 in $f13 and radius in $f14; compiled code has dot1 in $f14 and radius in $f13, which then cascades through sub/add/mov/c.le encodings.

No control-flow or algorithmic bytes differ. The remaining blocker still appears to be SNC FPU register allocation around inline asm outputs, consistent with the prior session notes. The integer-output experiment did not escape that allocator choice.

### Note 2

- session `2009bcee`

GetProjectedMinMax is a VFPU+scalar FPU interleaved function that projects two sphere centers along an axis and computes min/max. The algorithm is fully understood and correctly implemented with inline asm for VFPU operations (vscl.t, vadd.t, vdot.t, lv.q, mtv, mfv) and C for scalar math (sub.s, add.s, neg.s, comparisons).

The 13-byte mismatch is entirely due to SNC's inline asm FPU register allocator consistently assigning $f14 to the asm output instead of $f13. All 13 differing bytes are f13<->f14 register encoding swaps — the instruction sequence, scheduling, and algorithm are identical. The expected code has dot1 in f13 and radius in f14; my code consistently produces dot1 in f14 and radius in f13.

Approaches tried: (1) output constraint reordering, (2) f14 clobber to force f13 allocation, (3) register variable hints (asm("$f13")), (4) __builtin_ftovs/__builtin_vstof native VFPU builtins (changed VFPU register to S024 instead of S100, worse), (5) split mfv/mtc1 into separate asm blocks, (6) loading radius inside asm block, (7) loading offset inside asm block with early-clobber, (8) permuter with 2000+ candidates (no improvement), (9) all compiler flag variations (-Xmopt, -Xxopt). None changed the f14 allocation. SNC appears to use pair-based FPU register allocation for asm outputs, always assigning even registers (f14) before odd (f13). The original binary may have been compiled with a mechanism (builtin or native VFPU type scheduling) that goes through the compiler's normal allocation path rather than the asm output path. eCapsuleShape::GetProjectedMinMax (same size, same algorithm) has the same f13/f14 assignment in the expected binary.


## Disassembly

```asm
2095e0:	c48c0084 	lwc1	$f12,132(a0)
2095e4:	44096000 	mfc1	t1,$f12
2095e8:	48e90004 	mtv         $t1, S100
2095ec:	d8c60030 	lv.q        C120, 0x30($a2)
2095f0:	d8c70020 	lv.q        C130, 0x20($a2)
2095f4:	65048708 	vscl.t      C200, C130, S100
2095f8:	60088608 	vadd.t      C200, C120, C200
2095fc:	d8a90000 	lv.q        C210, 0x0($a1)
209600:	64898804 	vdot.t      S100, C200, C210
209604:	48650004 	mfv         $a1, S100
209608:	44856800 	mtc1	a1,$f13
20960c:	c48e0080 	lwc1	$f14,128(a0)
209610:	460e6bc1 	sub.s	$f15,$f13,$f14
209614:	460e6b40 	add.s	$f13,$f13,$f14
209618:	46006307 	neg.s	$f12,$f12
20961c:	44046000 	mfc1	a0,$f12
209620:	48e40004 	mtv         $a0, S100
209624:	65048707 	vscl.t      C130, C130, S100
209628:	60078606 	vadd.t      C120, C120, C130
20962c:	64898604 	vdot.t      S100, C120, C210
209630:	48640004 	mfv         $a0, S100
209634:	44848000 	mtc1	a0,$f16
209638:	460e8301 	sub.s	$f12,$f16,$f14
20963c:	460e8380 	add.s	$f14,$f16,$f14
209640:	460c783c 	c.lt.s	$f15,$f12
209644:	00000000 	nop
209648:	45030001 	bc1tl	0x209650
20964c:	46007b06 	mov.s	$f12,$f15
209650:	e4ec0000 	swc1	$f12,0(a3)
209654:	46007306 	mov.s	$f12,$f14
209658:	460c683e 	c.le.s	$f13,$f12
20965c:	00000000 	nop
209660:	45020001 	bc1fl	0x209668
209664:	46006b06 	mov.s	$f12,$f13
209668:	03e00008 	jr	ra
20966c:	e50c0000 	swc1	$f12,0(t0)
```

## m2c Starting Point

```c
// Decompiled: eMultiSphereShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const
// Address: 0x002095e0, Size: 144B
// Obj: eAll_psp.obj

void eMultiSphereShape__GetProjectedMinMax_constmVec3ref_constmOCSref_floatptr_floatptrconst(void *arg0, f32 arg1, f32 *arg3) {
    f32 temp_f12;
    f32 temp_f13;
    f32 temp_f14;
    f32 temp_f15;
    f32 var_f12;
    f32 var_f12_2;

    __asm__ volatile("mtv $t1, S100");
    __asm__ volatile("lv.q C120, 0x30($a2)");
    __asm__ volatile("lv.q C130, 0x20($a2)");
    __asm__ volatile("vscl.t C200, C130, S100");
    __asm__ volatile("vadd.t C200, C120, C200");
    __asm__ volatile("lv.q C210, ($a1)");
    __asm__ volatile("vdot.t S100, C200, C210");
    __asm__ volatile("mfv $a1, S100");
    temp_f14 = arg0->unk80;
    temp_f15 = arg1 - temp_f14;
    temp_f13 = arg1 + temp_f14;
    temp_f12 = -arg0->unk84;
    __asm__ volatile("mtv $a0, S100");
    __asm__ volatile("vscl.t C130, C130, S100");
    __asm__ volatile("vadd.t C120, C120, C130");
    __asm__ volatile("vdot.t S100, C120, C210");
    __asm__ volatile("mfv $a0, S100");
    var_f12 = temp_f12 - temp_f14;
    if (temp_f15 < var_f12) {
        var_f12 = temp_f15;
    }
    *arg3 = var_f12;
    var_f12_2 = temp_f12 + temp_f14;
    if (!(temp_f13 <= var_f12_2)) {
        var_f12_2 = temp_f13;
    }
    *M2C_ERROR(/* Read from unset register $t0 */) = var_f12_2;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
