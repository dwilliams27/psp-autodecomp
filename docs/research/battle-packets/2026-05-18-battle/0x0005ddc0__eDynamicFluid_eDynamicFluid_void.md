# Battle Packet: `eDynamicFluid::~eDynamicFluid(void)`

## Target

- Address: `0x0005ddc0`
- Size: `176` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicFluid`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `148`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 17B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `17`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `17`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00044cd0` `matched` 168B `eDynamicGeom::~eDynamicGeom(void)`

## Placement

- Canonical source: `src/eDynamicFluid.cpp`
- Header(s): `include/eDynamicFluid.h`
- Allowed source(s): `src/eDynamicFluid.cpp`
- Split-TU prefix: `src/eDynamicFluid_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eDynamicFluid.h`

```cpp
#ifndef EDYNAMICFLUID_H
#define EDYNAMICFLUID_H

#include "eDynamicGeom.h"

class cFile;
class cMemPool;
class eCamera;
class mFrustum;
class eCameraBins;
template <class T> class cHandleT;
class eMaterial;
class eDrawInfo;

class cTimeValue {
public:
    int mTime;
};

class eDynamicFluid : public eDynamicGeom {
public:
    eDynamicFluid(cBase *);
    ~eDynamicFluid();

    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const;
    void Draw(const eDrawInfo &) const;
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void Update(cTimeValue);

    static cBase *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0005dccc` 188B `eDynamicFluid::Read(cFile &, cMemPool *)` — `src/eDynamicFluid.cpp`
- `0x0005dd88` 56B `eDynamicFluid::eDynamicFluid(cBase *)` — `src/eDynamicFluid.cpp`
- `0x0005de70` 60B `eDynamicFluid::Update(cTimeValue)` — `src/eDynamicFluid.cpp`
- `0x0005deac` 8B `eDynamicFluid::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const` — `src/eMovie.cpp`
- `0x0005deb4` 8B `eDynamicFluid::Draw(const eDrawInfo &) const` — `src/eMovie.cpp`

## Matched Method Exemplars

### Exemplar 1: `eMovie::~eMovie(void)`

- Address: `0x00056d78`
- Size: `176` bytes
- Source: `src/eMovie.cpp`

```cpp
// --- eMovie::~eMovie(void) @ 0x00056d78 ---
eMovie::~eMovie() {
    Close();
    D_0037D2E0 = 0;
    D_0037D2F0 = 0;
    void *platformBase = (char *)this + 8;
    if (platformBase != 0) {
        ((cFilePlatform *)((char *)this + 0x98))->~cFilePlatform();
    }
}
```

### Exemplar 2: `eDisplayList::~eDisplayList(void)`

- Address: `0x001db6e4`
- Size: `176` bytes
- Source: `src/eDisplayList.cpp`

```cpp
eDisplayList::~eDisplayList(void) {
    void *cMemPoolSub = (char *)this + 0x20;
    if (cMemPoolSub != 0) {
        *(void **)((char *)this + 0xB8) = eDisplayListvirtualtable;
        cFastMemAllocator_dtor((char *)this + 0xBC, 2);
        cMemPool_dtor(cMemPoolSub, 0);
    }
}
```

### Exemplar 3: `eFluidVolume::~eFluidVolume(void)`

- Address: `0x0003c60c`
- Size: `172` bytes
- Source: `src/eFluidVolume.cpp`

```cpp
eFluidVolume::~eFluidVolume() {
    mClassDesc = eFluidVolumeclassdesc;
    if (mRef != 0) {
        char *type = ((char **)mRef)[1];
        DtorDispatchEntry *rec = (DtorDispatchEntry *)(type + 0x50);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn((char *)mRef + off, (void *)3);
        mRef = 0;
    }
    mClassDesc = cBaseclassdesc;
}
```

### Exemplar 4: `eDynamicGeom::~eDynamicGeom(void)`

- Address: `0x00044cd0`
- Size: `168` bytes
- Source: `src/eDynamicGeom.cpp`

```cpp
eDynamicGeom::~eDynamicGeom() {
    *(void **)((char *)this + 4) = eDynamicGeomvirtualtable;
    Detach();
    DetachChildren();
}
```


## Prior Failure Notes

### Note 1

- session `1a22a2d3`; src `src/eDynamicFluid.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/1a22a2d3/0005ddc0__agent_self_reported_failure__src_eDynamicFluid.cpp`

Canonical form structurally correct: inline operator delete in header (per eFluidVolume.h pattern with _DelRec/_PoolBlk struct member access) + canonical ~eDynamicFluid() body. File-scope __asm__(.word 0x1000ffff/.word 0) restores trailing 8B trap padding so symbol is exactly 176B. Result: 17/176 byte diff confined to the operator delete inlining region after the parent eDynamicGeom dtor jal. Mine allocates a2=allocTable, a3=offset and moves a1,s1 EARLY (right after the lw allocTable). Expected allocates a1=allocTable, a2=offset and moves a1,s1 in the JALR delay slot. The same operator delete source matches in eFluidVolume.cpp (where there is no parent dtor jal), so the divergence is specific to register pressure across the parent-dtor call boundary in the inlined deleting form. #pragma control sched=1 wraps the dtor; tried sched=1 around operator delete in .cpp — that lost the trap padding (out-of-line emit). Tried struct member access vs explicit casts vs intermediate variables — SNC's allocator tie-breaking still prefers a2/a3 over a1/a2. ctor MATCH(56), GetType MATCH(276), Read MATCH(188), Update MATCH(60). Pre-existing baseline mismatches (Write 15/76, New 47/124) are unrelated to this dtor work.

### Note 2

- session `405a48d1`

eDynamicFluid::~eDynamicFluid(void) — expected 176B, compiled .o is 168B (42 instructions). All 42 instructions match semantically when compiled with #pragma control sched=1 + memory barriers (register allocation, scheduling, branch-likely pattern, virtual-call dispatch for fluid child deletion, and DeleteRecord tail-call all identical). The 8-byte discrepancy is trailing `b 0xFFFF; nop` alignment padding at 0x5de68 that aligns the next function (Update @ 0x5de70) to a 16-byte boundary — this padding is in the DB-recorded size but SNC does not emit it inside .o output. Attempted fixes: return+for(;;) loop (produced 1/176 diff but wrong beqz target), inverted if branch, volatile guard variable, explicit inline-asm .align directive — none produced an exact byte match. compare_func.py consequently fell back to the nearest 168-byte function (eDynamicGeom::~eDynamicGeom) for diffing, reporting a spurious 99/168 diff. Likely requires either a compare_func.py change to ignore trailing inter-function alignment padding, or a SNC-side flag to emit section-trailing nops.


## Disassembly

```asm
5ddc0:	27bdfff0 	addiu	sp,sp,-16
5ddc4:	afb00000 	sw	s0,0(sp)
5ddc8:	afb10004 	sw	s1,4(sp)
5ddcc:	afbf0008 	sw	ra,8(sp)
5ddd0:	00808825 	move	s1,a0
5ddd4:	1080001f 	beqz	a0,0x5de54
5ddd8:	00a08025 	move	s0,a1
5dddc:	3c040038 	lui	a0,0x38
5dde0:	24842378 	addiu	a0,a0,9080
5dde4:	ae240004 	sw	a0,4(s1)
5dde8:	8e2400f0 	lw	a0,240(s1)
5ddec:	5080000a 	beqzl	a0,0x5de18
5ddf0:	02202025 	move	a0,s1
5ddf4:	8c850004 	lw	a1,4(a0)
5ddf8:	24a50050 	addiu	a1,a1,80
5ddfc:	84a60000 	lh	a2,0(a1)
5de00:	00862021 	addu	a0,a0,a2
5de04:	8ca60004 	lw	a2,4(a1)
5de08:	00c0f809 	jalr	a2
5de0c:	34050003 	li	a1,0x3
5de10:	ae2000f0 	sw	zero,240(s1)
5de14:	02202025 	move	a0,s1
5de18:	0c011334 	jal	0x44cd0
5de1c:	00002825 	move	a1,zero
5de20:	32040001 	andi	a0,s0,0x1
5de24:	1080000b 	beqz	a0,0x5de54
5de28:	00000000 	nop
5de2c:	0c0011e8 	jal	0x47a0
5de30:	02202025 	move	a0,s1
5de34:	8c440024 	lw	a0,36(v0)
5de38:	8c85001c 	lw	a1,28(a0)
5de3c:	24a50030 	addiu	a1,a1,48
5de40:	84a60000 	lh	a2,0(a1)
5de44:	00862021 	addu	a0,a0,a2
5de48:	8ca60004 	lw	a2,4(a1)
5de4c:	00c0f809 	jalr	a2
5de50:	02202825 	move	a1,s1
5de54:	8fb00000 	lw	s0,0(sp)
5de58:	8fb10004 	lw	s1,4(sp)
5de5c:	8fbf0008 	lw	ra,8(sp)
5de60:	03e00008 	jr	ra
5de64:	27bd0010 	addiu	sp,sp,16
5de68:	1000ffff 	b	0x5de68
5de6c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eDynamicFluid::~eDynamicFluid(void)
// Address: 0x0005ddc0, Size: 176B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *); /* extern */
? eDynamicGeom___dtor_eDynamicGeom_void(void *, ?); /* extern */

void eDynamicFluid___dtor_eDynamicFluid_void(void *arg0, s32 arg1) {
    ? (*temp_a2)(void *, ?, ?);
    ? (*temp_a2_2)(void *, void *, ?);
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1;
    void *temp_a1_2;

    if (arg0 != NULL) {
        arg0->unk4 = 0x382378;
        temp_a0 = arg0->unkF0;
        if (temp_a0 != NULL) {
            temp_a1 = temp_a0->unk4;
            temp_a2 = (temp_a1 + 0x50)->unk4;
            temp_a2(temp_a0 + temp_a1->unk50, 3, temp_a2);
            arg0->unkF0 = NULL;
        }
        eDynamicGeom___dtor_eDynamicGeom_void(arg0, 0);
        if (arg1 & 1) {
            temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a1_2 = temp_a0_2->unk1C;
            temp_a2_2 = (temp_a1_2 + 0x30)->unk4;
            temp_a2_2(temp_a0_2 + temp_a1_2->unk30, arg0, temp_a2_2);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
