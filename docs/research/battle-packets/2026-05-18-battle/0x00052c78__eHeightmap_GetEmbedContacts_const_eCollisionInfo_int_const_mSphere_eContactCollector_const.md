# Battle Packet: `eHeightmap::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const`

## Target

- Address: `0x00052c78`
- Size: `64` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eHeightmap`
- Method family: `GetEmbedContacts`
- Leaf: `True`

## Queue Metadata

- score: `163`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 7B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `7`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `7`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eHeightmap.cpp`
- Header(s): `include/eHeightmap.h`
- Allowed source(s): `src/eHeightmap.cpp`
- Split-TU prefix: `src/eHeightmap_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eHeightmap.h`

```cpp
#ifndef EHEIGHTMAP_H
#define EHEIGHTMAP_H

#include "eDynamicGeom.h"

class cBase;
class cMemPool;
class cType;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class cFile;

class eHeightmap : public eGeom {
public:
    struct _DelRec { short offset; short pad; void (*fn)(void *, void *); };
    eHeightmap(cBase *);
    ~eHeightmap(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        char *allocTable = *(char **)((char *)block + 0x1C);
        _DelRec *rec = (_DelRec *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = (char *)block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
    static eHeightmap *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void PlatformFree(void);
    const cType *GetType(void) const;
    int GetSurface(int) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const;
    void __attribute__((naked)) CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00052724` 140B `eHeightmap::Write(cFile &) const` — `src/eHeightmap.cpp`
- `0x000527b0` 284B `eHeightmap::Read(cFile &, cMemPool *)` — `src/eHeightmap_Read.cpp`
- `0x000528cc` 56B `eHeightmap::PlatformFree(void)` — `src/eHeightmap.cpp`
- `0x00052904` 56B `eHeightmap::eHeightmap(cBase *)` — `src/eHeightmap.cpp`
- `0x0005293c` 144B `eHeightmap::~eHeightmap(void)` — `src/eHeightmap.cpp`

## Matched Method Exemplars

### Exemplar 1: `eStaticModel::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const`

- Address: `0x00042760`
- Size: `64` bytes
- Source: `src/cCoreGlobals.cpp`

```cpp
void eStaticModel::GetEmbedContacts(const eCollisionInfo &info, int idx,
                                    const mSphere *sphere,
                                    eContactCollector *collector) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &,
               eContactCollector *))entry[1])
        (shape + *(short *)entry, idx, sphere, info, collector);
}
```

### Exemplar 2: `eDynamicModel::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const`

- Address: `0x000498d8`
- Size: `64` bytes
- Source: `src/eDynamicModel.cpp`

```cpp
void eDynamicModel::GetEmbedContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, eContactCollector *cc) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &, eContactCollector *))entry[1])(shape + *(short *)entry, idx, sphere, info, cc);
}
```

### Exemplar 3: `eCylinderShape::GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const`

- Address: `0x00074460`
- Size: `48` bytes
- Source: `src/eCylinderShape.cpp`

```cpp
// eCylinderShape::GetEmbedContacts(...) const — 0x00074460
#pragma control sched=1
int eCylinderShape::GetEmbedContacts(int idx, const mSphere *sphere,
                                     const eCollisionInfo &collisionInfo,
                                     eContactCollector *collector) const {
    void *shape = (void *)_unk88;
    EmbedContactsVtableEntry *entry =
        (EmbedContactsVtableEntry *)(*(char **)((char *)shape + 4) + 0xB0);
    return entry->fn((char *)shape + entry->thisOffset, idx, sphere, collisionInfo,
                     collector);
}
```

### Exemplar 4: `eTriangleShape::GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const`

- Address: `0x0006fe38`
- Size: `8` bytes
- Source: `src/eTriangleShape_GetEmbedContacts.cpp`

```cpp
int eTriangleShape::GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const {
    return 0;
}
```


## Prior Failure Notes

### Note 1

- session `7b347bba`

Best result is a 7/64-byte mismatch with identical control flow, size, prologue, epilogue, and call target pattern. Expected keeps the eCollisionInfo pointer in t1 and uses t2 for the slot offset/function: move t1,a1; lw a0,4(t1); lw a1,4(a0); lh/lw t2; jalr t2 with move a3,t1 in the delay slot. The best C++ form keeps info in t0 and uses t1 for the slot offset/function, giving the same instructions with registers shifted down: move t0,a1; lw a0,4(t0); lw a1,4(a0); lh/lw t1; jalr t1 with move a3,t0.

Manual attempts tried sched=2, sched=1, and sched=0; int* entry indexing, char* entry plus explicit short/function locals, struct slot access, volatile shape loads, empty asm clobbers, dummy live variables, and hard register-variable experiments. The hard register-variable forms either were ignored by SNC or produced invalid zero-register loads, so they were abandoned. A 300-second tools/permuter.py run compiled 4,407 candidates from this 7-byte baseline with zero improvement. This appears to be a stubborn SNC register allocation/lifetime ordering issue rather than a control-flow or scheduling issue.

### Note 2

- session `6e1dde42`

eHeightmap::GetEmbedContacts — 9/64 byte diff. Function is a virtual dispatch through info.shape->vtable[0xB0]. My compile produces the correct INSTRUCTION set (size matches: 64B vs expected 64B, same ops), but with different register allocation. Expected saves info in t1 and REUSES a1 for vt (vt loaded into a1, then a1+=0xB0, then a1 reused for idx arg before call); mine saves info in t0 and uses t1 for vt, a2 for entry. Same number of instructions, different temps. Tested all -Xsched (0,1,2) × -Xmopt × -Xxopt × -Xfcm combos — all pick t0 for info save. Source form used: `char *shape = ((char**)&info)[1]; int *entry = (int *)(((char**)shape)[1] + 0xB0); ((fn_t)entry[1])(shape + *(short*)entry, idx, sphere, info);` Also tried: explicit vt load via struct, separate fn/adj variables via `*(short*)(vt+0xB0)` and `*(fn_t*)(vt+0xB4)`, volatile casts. None changed the register allocation. The register allocator appears to base its choice on IR scheduling ordering, which differs from the expected. Best-guess theory: in the expected the compiler saw info's use in the delay slot as 'the most important surviving value' and assigned it to t1 (matching arg1 conventionally), but my source's syntax tree places info's save earlier in the allocator's order. Next agent could try: introducing a no-op that consumes t0 early, wrapping the shape pointer load in a volatile, or restructuring the call so `info` is referenced directly in the callee rather than via a saved pointer. The sched=0 variant is closest — just delay-slot nop + register off-by-one.


## Disassembly

```asm
52c78:	27bdfff0 	addiu	sp,sp,-16
52c7c:	afbf0000 	sw	ra,0(sp)
52c80:	00a04825 	move	t1,a1
52c84:	8d240004 	lw	a0,4(t1)
52c88:	8c850004 	lw	a1,4(a0)
52c8c:	24a500b0 	addiu	a1,a1,176
52c90:	84aa0000 	lh	t2,0(a1)
52c94:	008a2021 	addu	a0,a0,t2
52c98:	8caa0004 	lw	t2,4(a1)
52c9c:	00c02825 	move	a1,a2
52ca0:	00e03025 	move	a2,a3
52ca4:	0140f809 	jalr	t2
52ca8:	01203825 	move	a3,t1
52cac:	8fbf0000 	lw	ra,0(sp)
52cb0:	03e00008 	jr	ra
52cb4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eHeightmap::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const
// Address: 0x00052c78, Size: 64B
// Obj: eAll_psp.obj

void eHeightmap__GetEmbedContacts_consteCollisionInforef_int_constmSphereptr_eContactCollectorptrconst(void *arg1, ? arg2, ? arg3) {
    void *temp_a0;
    void *temp_a1;

    temp_a0 = arg1->unk4;
    temp_a1 = temp_a0->unk4;
    (temp_a1 + 0xB0)->unk4(temp_a0 + temp_a1->unkB0, arg2, arg3, arg1);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
