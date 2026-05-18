# Battle Packet: `eSurfaceProperty::AssignCopy(const cBase *)`

## Target

- Address: `0x001f5f84`
- Size: `112` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSurfaceProperty`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `155`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `10`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x001ccefc` `untried` 348B `dcast dcast<eSurfaceProperty *>(const cBase *)`

## Placement

- Canonical source: `src/eSurfaceProperty.cpp`
- Header(s): `include/eSurfaceProperty.h`
- Allowed source(s): `src/eSurfaceProperty.cpp`
- Split-TU prefix: `src/eSurfaceProperty_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eSurfaceProperty.h`

Not found.

## Matched Same-Class Neighbors

- `0x0005364c` 88B `eSurfaceProperty::Write(cFile &) const` — `src/eSurfaceProperty.cpp`
- `0x000536a4` 208B `eSurfaceProperty::Read(cFile &, cMemPool *)` — `src/eSurfaceProperty.cpp`
- `0x001f5ff4` 160B `eSurfaceProperty::New(cMemPool *, cBase *) static` — `src/eSurfaceProperty.cpp`
- `0x001f6094` 220B `eSurfaceProperty::GetType(void) const` — `src/eSurfaceProperty.cpp`
- `0x001f6170` 100B `eSurfaceProperty::~eSurfaceProperty(void)` — `src/eCameraEffectLayer.cpp`

## Matched Method Exemplars

### Exemplar 1: `ePoint::AssignCopy(const cBase *)`

- Address: `0x00203b18`
- Size: `112` bytes
- Source: `src/ePoint.cpp`

```cpp
//   0x00203b18  ePoint::AssignCopy(const cBase *)   (112B)
//   0x00203b88  ePoint::New(cMemPool *, cBase *)    (124B)

inline void *operator new(unsigned int, void *p) { return p; }
```

### Exemplar 2: `eSky::AssignCopy(const cBase *)`

- Address: `0x002048d8`
- Size: `112` bytes
- Source: `src/eSky.cpp`

```cpp
//   0x002048d8  eSky::AssignCopy(const cBase *)             (112B)
//   0x00204948  eSky::New(cMemPool *, cBase *) static       (124B)
//   0x0005d1a8  eSky::~eSky(void)                           (356B)
//
// eSky inherits from cObject. Layout:
//   [0x00..0x44) cObject base
//   [0x44..0x48) cArrayBase<cHandleT<eGeomTemplate> >
//   [0x48..0x50) cBaseArray
//   [0x50..0x58) cBaseArray
//   [0x58..0x60) cBaseArray
// sizeof(eSky) = 0x60

class cBase;
class cFile;
class eGeomTemplate;
class cType;

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};
```

### Exemplar 3: `eDynamicFluidTemplate::AssignCopy(const cBase *)`

- Address: `0x002051ec`
- Size: `112` bytes
- Source: `src/eDynamicFluidTemplate.cpp`

```cpp
//   0x002051ec  eDynamicFluidTemplate::AssignCopy(const cBase *)        (112B)
//   0x0020525c  eDynamicFluidTemplate::New(cMemPool *, cBase *) static  (124B)

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};
```

### Exemplar 4: `eBipedControllerConfig::AssignCopy(const cBase *)`

- Address: `0x002083ec`
- Size: `104` bytes
- Source: `src/eBipedControllerConfig.cpp`

```cpp
// ── eBipedControllerConfig::AssignCopy(const cBase *) @ 0x002083ec ──
void eBipedControllerConfig::AssignCopy(const cBase *base) {
    eBipedControllerConfig *other = dcast<eBipedControllerConfig>(base);
    int *src = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    __asm__ volatile("" : "+r"(src), "+r"(dst));
    int w0 = src[0];
    int w1 = src[1];
    int w2 = src[2];
    dst[0] = w0;
    dst[1] = w1;
    dst[2] = w2;
    __asm__ volatile("" ::: "memory");
    int w3 = src[3];
    int w4 = src[4];
    int w5 = src[5];
    dst[3] = w3;
    dst[4] = w4;
    dst[5] = w5;
    mField20 = other->mField20;
}
```


## Prior Failure Notes

### Note 1

- session `c6bfcb18`

The existing best struct-copy source still compiles to the documented 10/112-byte mismatch. The generated code has the correct prologue, dcast call, base pointer setup, all six word loads/stores, tail copy, and epilogue, but SNC interleaves the first 24-byte struct copy as lw/lw/lw/sw/lw/sw/lw/sw/lw/sw/sw/sw. Expected is the same instructions and registers in two batches: three loads, three stores, three loads, three stores.

I tried a new scalar-temp formulation with two explicit groups of three int copies, plus a variant with an empty asm operand barrier to keep the source/destination base pointers live before the loads. The scalar form worsened the diff to 13/112 bytes because it delayed the destination addiu and changed copy registers; the operand barrier produced the same scalar object. Restored the prior best cName struct-copy formulation. This still looks like a scheduler/flag divergence, not bnel, and likely needs a per-class compiler flag override outside the allowed source edits.

### Note 2

- session `6632657a`

eSurfaceProperty::AssignCopy(const cBase*) — best diff is 10/112 bytes. Could not be matched despite extensive iteration. Approaches tried: (1) flat int[6] struct copy, (2) split into two W12 sub-structs (extra addiu's, worse), (3) named-field cName{int a,b,c,d,e,f}, (4) nested struct cName{W12 lo, hi}, (5) plain int* assign for tail (size mismatch, drops one addiu), (6) sched=0 (tail order matched but body & delay-slot worse), (7) sched=2 (worst body interleaving). Best result with #pragma sched=1 + cName{int m[6]} struct copy + __asm__ memory barrier + Tail{int x} struct for the 0x20 field. Permuter ran for 540 seconds total (180s + 360s, ~20.5k candidates compiled) — found the memory-barrier improvement (16→10 bytes) but no further progress.

ROOT CAUSE: scheduler divergence within the 24-byte struct copy. Expected emits 3lw,3sw,3lw,3sw batches with shared base pointers (a0=src+8, a1=dst+8 reused with offsets 0,4,8 then 12,16,20). Our SNC emits an interleaved schedule for the same single-struct copy: 3lw,1sw,1lw,1sw,1lw,1sw,1lw,3sw. Same instructions, same registers, just reordered. This is NOT bnel divergence (no branch-likely involved) — it is a scheduler heuristic difference for struct-copy basic blocks. Cannot be fixed with __asm__ memory barriers because the entire struct copy is a single compiler-emitted basic block (a barrier would force two W12 copies which then recompute the base pointers, yielding extra addiu's).

The exemplar gcEntityAttackHelper::AssignCopy at 0x25b818 also has interleaved scheduling (verified by disassembling its built .o), so the original SNC matched that one with the interleaved pattern — but for eSurfaceProperty at 0x1f5f84 the original chose the batched pattern. Likely a different -Xmopt/-Xxopt flag was used for eSurfaceProperty. Agents are not permitted to modify the Makefile, so this needs human intervention to add a per-class compiler-flag override (try -Xmopt=0 or other -Xmopt/-Xxopt combinations on eAll_psp's eSurfaceProperty translation unit). Once the flag is found, the source as-is should match. Stays 'failed' (not unmatchable_bnel) per CLAUDE.md norms — diff is 10 bytes which exceeds the ≤8-byte bnel criterion and the differing bytes are not bnel/beql vs bne/beq.


## Disassembly

```asm
1f5f84:	27bdfff0 	addiu	sp,sp,-16
1f5f88:	afb00000 	sw	s0,0(sp)
1f5f8c:	afbf0004 	sw	ra,4(sp)
1f5f90:	00808025 	move	s0,a0
1f5f94:	0c0733bf 	jal	0x1ccefc
1f5f98:	00a02025 	move	a0,a1
1f5f9c:	24440008 	addiu	a0,v0,8
1f5fa0:	26050008 	addiu	a1,s0,8
1f5fa4:	8c860000 	lw	a2,0(a0)
1f5fa8:	8c870004 	lw	a3,4(a0)
1f5fac:	8c880008 	lw	t0,8(a0)
1f5fb0:	aca60000 	sw	a2,0(a1)
1f5fb4:	aca70004 	sw	a3,4(a1)
1f5fb8:	aca80008 	sw	t0,8(a1)
1f5fbc:	8c86000c 	lw	a2,12(a0)
1f5fc0:	8c870010 	lw	a3,16(a0)
1f5fc4:	8c840014 	lw	a0,20(a0)
1f5fc8:	aca6000c 	sw	a2,12(a1)
1f5fcc:	aca70010 	sw	a3,16(a1)
1f5fd0:	aca40014 	sw	a0,20(a1)
1f5fd4:	26040020 	addiu	a0,s0,32
1f5fd8:	24450020 	addiu	a1,v0,32
1f5fdc:	8ca50000 	lw	a1,0(a1)
1f5fe0:	ac850000 	sw	a1,0(a0)
1f5fe4:	8fb00000 	lw	s0,0(sp)
1f5fe8:	8fbf0004 	lw	ra,4(sp)
1f5fec:	03e00008 	jr	ra
1f5ff0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eSurfaceProperty::AssignCopy(const cBase *)
// Address: 0x001f5f84, Size: 112B
// Obj: eAll_psp.obj

void *dcastdcast_eSurfacePropertyptr__constcBaseptr(?); /* extern */

void eSurfaceProperty__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    void *temp_a0;
    void *temp_a1;
    void *temp_v0;

    temp_v0 = dcastdcast_eSurfacePropertyptr__constcBaseptr(arg1);
    temp_a0 = temp_v0 + 8;
    temp_a1 = arg0 + 8;
    arg0->unk8 = (s32) temp_v0->unk8;
    temp_a1->unk4 = (s32) temp_a0->unk4;
    temp_a1->unk8 = (s32) temp_a0->unk8;
    temp_a1->unkC = (s32) temp_a0->unkC;
    temp_a1->unk10 = (s32) temp_a0->unk10;
    temp_a1->unk14 = (s32) temp_a0->unk14;
    arg0->unk20 = (s32) temp_v0->unk20;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
