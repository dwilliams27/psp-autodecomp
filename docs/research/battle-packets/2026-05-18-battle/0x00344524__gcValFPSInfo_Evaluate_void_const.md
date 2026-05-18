# Battle Packet: `gcValFPSInfo::Evaluate(void) const`

## Target

- Address: `0x00344524`
- Size: `108` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValFPSInfo`
- Method family: `Evaluate`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 2B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`, `needs_layout`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/gcValFPSInfo.cpp`
- Header(s): `include/gcValFPSInfo.h`
- Allowed source(s): `src/gcValFPSInfo.cpp`
- Split-TU prefix: `src/gcValFPSInfo_*.cpp`

## Class Header: `include/gcValFPSInfo.h`

```cpp
#ifndef GCVALFPSINFO_H
#define GCVALFPSINFO_H

class cBase;
class cType;

class gcValFPSInfo {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    float Evaluate(void) const;
    const cType *GetType(void) const;

    int pad0;
    int pad4;
    float f8;
};

#endif
```

## Matched Same-Class Neighbors

- `0x00344358` 48B `gcValFPSInfo::AssignCopy(const cBase *)` — `src/gcValFPSInfo.cpp`
- `0x00344388` 132B `gcValFPSInfo::New(cMemPool *, cBase *) static` — `src/gcValFPSInfo.cpp`
- `0x0034440c` 280B `gcValFPSInfo::GetType(void) const` — `src/gcValFPSInfo.cpp`
- `0x00344590` 40B `gcValFPSInfo::GetText(char *) const` — `src/gcValFPSInfo.cpp`
- `0x003445f4` 100B `gcValFPSInfo::~gcValFPSInfo(void)` — `src/gcValFPSInfo.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValLobbySessionStatus::Evaluate(void) const`

- Address: `0x0034e7c4`
- Size: `108` bytes
- Source: `src/gcValLobbySessionStatus.cpp`

```cpp
// ── gcValLobbySessionStatus::Evaluate(void) const @ 0x0034e7c4 ──
float gcValLobbySessionStatus::Evaluate(void) const {
    void *lobby = nwNetwork::GetLobby();
    if (lobby == 0) {
        return 0.0f;
    }
    DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x80);
    short off = e->offset;
    int (*fn)(void *) = e->fn;
    int status = fn((char *)lobby + off);
    return (float)(this->mField8 == status);
}
```

### Exemplar 2: `gcValEntityIsLocallyControlled::Evaluate(void) const`

- Address: `0x00334e08`
- Size: `80` bytes
- Source: `src/gcValEntityIsLocallyControlled.cpp`

```cpp
// ── gcValEntityIsLocallyControlled::Evaluate(void) const @ 0x00334E08 (80B) ──
float gcValEntityIsLocallyControlled::Evaluate(void) const {
    gcEntity *entity = ((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    return (entity != 0 && entity->IsLocallyControlled()) ? 1.0f : 0.0f;
}
```

### Exemplar 3: `gcValEntityHasConstants::Evaluate(void) const`

- Address: `0x0032fd28`
- Size: `76` bytes
- Source: `src/gcValEntityHasConstants.cpp`

```cpp
float gcValEntityHasConstants::Evaluate(void) const {
    void *entity = ((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    float result;
    if (entity != 0) {
        result = (float)gcEntity_HasConstants(entity,
                                             *(cHandle *)((const char *)this + 52));
    } else {
        result = 0.0f;
    }
    return result;
}
```

### Exemplar 4: `gcValEntityHasVariables::Evaluate(void) const`

- Address: `0x00331ad4`
- Size: `76` bytes
- Source: `src/gcValEntityHasVariables.cpp`

```cpp
// ── gcValEntityHasVariables::Evaluate(void) const @ 0x00331ad4 ──
float gcValEntityHasVariables::Evaluate(void) const {
    void *entity = ((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    float result;
    if (entity != 0) {
        result = (float)gcEntity_HasVariables(entity,
                                             *(cHandle *)((const char *)this + 52));
    } else {
        result = 0.0f;
    }
    return result;
}
```


## Prior Failure Notes

### Note 1

- session `8cd5908c`

Still 2/108 bytes off, exactly at the first global load for g_fps_pInstance: expected uses lui/lw with a1 as both HI-base and destination, while SNC keeps emitting lui a0; lw a1, 0(a0); li a0, 0. The rest of the function body, including the bnezl null-deref pattern, all integer-to-float conversions, stores to obj+0x18/0x1c/0x20, and the final g_fps_rate load, matches byte-for-byte.

New attempts beyond the prior session: empty asm keeping this live before and after the first load, empty asm clobbering $a0, register-bound locals for inst/obj, sched=1 around only Evaluate, pointer-to-global load through a register-bound pointer, volatile and array declarations for g_fps_pInstance, and a register-bound self in $a0 with a +r empty asm. All converged to the same 2-byte diff. A full 300s permuter run generated 11088 candidates, compiled 3820, and found zero improvements. This still appears to be an SNC allocator/version heuristic around dead this in an unused const method, not a source-level control-flow or scheduling issue.

### Note 2

- session `6cfc9fc7`

Evaluate is 2 bytes off — a pure register-allocation divergence that neither source restructuring nor the permuter could fix. The function reads a pointer from global 0x37D7B4 (declared as extern void *g_fps_pInstance), dereferences its field at offset 0x10 (if non-null), writes (float)x + (float)y to obj->0x18, (float)x to obj->0x1C, (float)y to obj->0x20 where x=*0x37D0F8 and y=*0x37D0FC, and returns *(float*)0x37D100. The control flow, branch-likely pattern (bnezl for the null-check-and-dereference), and all FP scheduling of mtc1/cvt.s.w/add.s/swc1 sequences match exactly. Body of function from offset 0x08 through 0x68 is byte-identical to expected.

The 2-byte diff is at offsets 0x01 and 0x05 — the base register for the very first lui/lw pair that loads g_fps_pInstance. Expected produces 'lui a1, HI; lw a1, LO(a1)' (same register for HI-base and destination, leaving a0/this alone). Mine produces 'lui a0, HI; lw a1, LO(a0)' (clobbers a0/this immediately as HI-base). The subsequent 'li a0, 0' (for var obj=0) then re-uses a0. Both are functionally equivalent; SNC's allocator just picks a0 vs a1 for the HI temp.

Approaches tried: (a) extern void *g_fps_pInstance, (b) literal *(void**)0x37D7B4, (c) structured pointer type (extern FPSHolder *g_fps_pInstance with struct layout), (d) ternary for obj assignment, (e) nested-if control flow, (f) declaring obj before inst (which swaps registers but doesn't align with expected — ends up with 'lui a0; lw a0, 0(a0); li a1, 0' instead of 'lui a1; lw a1, 0(a1); li a0, 0'), (g) split declaration vs combined init, (h) (void)this; dummy use, (i) gcValFPSInfo::s_pInstance static class member. All produce the same 2-byte diff. Permuter ran 120s / 992 candidates compiled, found zero improvements. The other global accesses (g_fps_x, g_fps_y, g_fps_rate) all match exactly with a1/a3/a0 base registers respectively; only the very first global load diverges.

Root cause: SNC's register allocator uses different heuristics for the first lui/lw of a function when 'this' is live-in on a const method but unused. In expected, the allocator reserves a0 for 'this' through the load and uses a1 as HI temp (and folds to same reg as dest). In our SNC build, the allocator decides 'this' is dead and uses a0 as HI temp. This may be a minor SNC version difference, or a compiler-flag difference we haven't identified. A follow-up agent might try: (1) adding -Xsched=1 override specifically for this file (though sched=2 is default for gcAll_psp), (2) checking if a different -Xvfpumatrix N affects allocation, (3) seeing if declaring g_fps_pInstance as a class-static with a specific mangled name (e.g. some sibling class::s_p...) changes first-use ordering, (4) testing whether the real source defines g_fps_pInstance in a header that is also included as a template/inline forcing different codegen timing.


## Disassembly

```asm
344524:	3c050038 	lui	a1,0x38
344528:	8ca5d7b4 	lw	a1,-10316(a1)
34452c:	34040000 	li	a0,0x0
344530:	54a00001 	bnezl	a1,0x344538
344534:	8ca40010 	lw	a0,16(a1)
344538:	10800012 	beqz	a0,0x344584
34453c:	3c050038 	lui	a1,0x38
344540:	8ca6d0f8 	lw	a2,-12040(a1)
344544:	3c070038 	lui	a3,0x38
344548:	44866000 	mtc1	a2,$f12
34454c:	8ce6d0fc 	lw	a2,-12036(a3)
344550:	46806320 	cvt.s.w	$f12,$f12
344554:	44866800 	mtc1	a2,$f13
344558:	46806b60 	cvt.s.w	$f13,$f13
34455c:	460d6300 	add.s	$f12,$f12,$f13
344560:	e48c0018 	swc1	$f12,24(a0)
344564:	8ca5d0f8 	lw	a1,-12040(a1)
344568:	44856000 	mtc1	a1,$f12
34456c:	46806320 	cvt.s.w	$f12,$f12
344570:	e48c001c 	swc1	$f12,28(a0)
344574:	8ce5d0fc 	lw	a1,-12036(a3)
344578:	44856000 	mtc1	a1,$f12
34457c:	46806320 	cvt.s.w	$f12,$f12
344580:	e48c0020 	swc1	$f12,32(a0)
344584:	3c040038 	lui	a0,0x38
344588:	03e00008 	jr	ra
34458c:	c480d100 	lwc1	$f0,-12032(a0)
```

## m2c Starting Point

```c
// Decompiled: gcValFPSInfo::Evaluate(void) const
// Address: 0x00344524, Size: 108B
// Obj: gcAll_psp.obj

f32 gcValFPSInfo__Evaluate_voidconst(void) {
    void *temp_a1;
    void *var_a0;

    temp_a1 = *(void **)0x37D7B4;
    var_a0 = NULL;
    if (temp_a1 != NULL) {
        var_a0 = temp_a1->unk10;
    }
    if (var_a0 != NULL) {
        var_a0->unk18 = (f32) ((f32) *(s32 *)0x37D0F8 + (f32) *(s32 *)0x37D0FC);
        var_a0->unk1C = (f32) *(s32 *)0x37D0F8;
        var_a0->unk20 = (f32) *(s32 *)0x37D0FC;
    }
    return *(f32 *)0x37D100;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
