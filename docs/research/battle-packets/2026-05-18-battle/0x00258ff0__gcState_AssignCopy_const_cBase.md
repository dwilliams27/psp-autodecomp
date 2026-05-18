# Battle Packet: `gcState::AssignCopy(const cBase *)`

## Target

- Address: `0x00258ff0`
- Size: `156` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcState`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `190`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 2B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006e7c` `matched` 228B `cBaseArray::operator=(const cBaseArray &)`
- `0x00222f24` `untried` 348B `dcast dcast<gcState *>(const cBase *)`

## Placement

- Canonical source: `src/gcState.cpp`
- Header(s): `include/gcState.h`
- Allowed source(s): `src/gcState.cpp`
- Split-TU prefix: `src/gcState_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcState.h`

Not found.

## Matched Same-Class Neighbors

- `0x0010ac68` 124B `gcState::Write(cFile &) const` — `src/gcState.cpp`
- `0x0010ae68` 152B `gcState::gcState(cBase *)` — `src/gcState.cpp`
- `0x0010af00` 164B `gcState::GetName(void) const` — `src/gcState.cpp`
- `0x0025908c` 124B `gcState::New(cMemPool *, cBase *) static` — `src/gcState.cpp`
- `0x00259108` 228B `gcState::GetType(void) const` — `src/gcState.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcRigidBodyControllerTemplate::AssignCopy(const cBase *)`

- Address: `0x0031f1e4`
- Size: `156` bytes
- Source: `src/gcRigidBodyControllerTemplate.cpp`

```cpp
void gcRigidBodyControllerTemplate::AssignCopy(const cBase *src) {
    gcRigidBodyControllerTemplate *other =
        (gcRigidBodyControllerTemplate *)dcastdcast_gcRigidBodyControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);
}
```

### Exemplar 2: `gcSimpleControllerTemplate::AssignCopy(const cBase *)`

- Address: `0x0031fd50`
- Size: `156` bytes
- Source: `src/gcSimpleControllerTemplate.cpp`

```cpp
// ── gcSimpleControllerTemplate::AssignCopy(const cBase *) @ 0x0031fd50 ──
void gcSimpleControllerTemplate::AssignCopy(const cBase *src) {
    gcSimpleControllerTemplate *other =
        (gcSimpleControllerTemplate *)dcastdcast_gcSimpleControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);
}
```

### Exemplar 3: `gcStationaryControllerTemplate::AssignCopy(const cBase *)`

- Address: `0x003203a8`
- Size: `156` bytes
- Source: `src/gcStationaryControllerTemplate.cpp`

```cpp
// ── gcStationaryControllerTemplate::AssignCopy(const cBase *) @ 0x003203a8 ──
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcStationaryControllerTemplate::AssignCopy(const cBase *src) {
    gcStationaryControllerTemplate *other =
        (gcStationaryControllerTemplate *)dcastdcast_gcStationaryControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);
}
```

### Exemplar 4: `gcEntityGeomConfig::AssignCopy(const cBase *)`

- Address: `0x002672ec`
- Size: `152` bytes
- Source: `src/gcEntityGeomConfig.cpp`

```cpp
//   0x002672EC  gcEntityGeomConfig::AssignCopy(const cBase *)           (152B)
//   0x00125ACC  gcEntityGeomConfig::Write(cFile &) const                (132B)
//   0x0031CA38  gcGeomTrailConfig::AssignCopy(const cBase *)            (152B)

class cBase;
class cFile;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
```


## Prior Failure Notes

### Note 1

- session `753c670f`; src `src/gcState.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/753c670f/00258ff0__agent_self_reported_failure__src_gcState.cpp`

gcState::AssignCopy(const cBase *) — stuck at the same 2-byte mismatch as the prior agent. Diff is exclusively in the third (offset 0x28) scalar copy: expected emits `lw a2, 0(a2); sw a2, 0(a1)` (reusing the source-pointer register a2 for the loaded value), my source emits `lw a0, 0(a2); sw a0, 0(a1)` (compiler picks a0 for the load value). All other 154 bytes match including the 6-int unrolled copy at offsets 8-0x1F, the 0x20/0x24 individual copies (which exhibit the alternating a0/a1/a2 reg-allocation pattern), the addiu+lw materialization style, and the cBaseArray::operator= call setup with delay-slot fill.

Approaches tried this session: (1) baseline explicit-pointer style (matches prior 2-byte diff, 156B); (2) simple `*p=*q` form (132B - compiler used base+offset addressing directly via `lw a0, 32(v0)` rather than addiu materialization, so wrong size but interestingly DID pick a2 for the 0x28 value); (3) struct copy of 12 bytes (140B - similar issue); (4) explicit pointers without the prior memory barrier; (5) const-qualified source pointers (still 2 bytes); (6) walking pointer with post-increment (worse, 17 bytes). Loop variant didn't unroll (152B with t0 instead of a0/a2). Prior agent ran 9432-iteration permuter from the same baseline with zero improvement.

Root cause appears to be SNC's register allocation heuristic for the third int copy. The simple form picks a0/a1/a2 sequentially across the 3 copies (matching the expected register choice for 0x28) but optimizes away the addiu materialization. The explicit-pointer form forces addiu materialization (matching the expected size and overall instruction sequence) but allocates a0 for the third value instead of a2. Bridging these two requires nudging the scheduler — possibly via cross-file context (call graph, inlining decisions) that affects SNC's register-pressure lookahead. Not a bnel/beql divergence so doesn't qualify for unmatchable classification.

### Note 2

- session `e373c1d2`

Implemented gcState::AssignCopy(const cBase *) as a clean C++ near-match. The first 0x18-byte scalar block, offsets 0x20 and 0x24, and the final cBaseArray assignment all match structurally; the remaining mismatch is exactly 2 bytes in the final 0x28 scalar copy. Expected uses `lw a2,0(a2)` followed by `sw a2,0(a1)`, while the clean source emits `lw a0,0(a2)` followed by `sw a0,0(a1)`.

Tried manual restructurings with unrolled scalar loads, pointer-shaped copies, cHandle struct assignment for 0x24/0x28, delayed cBaseArray pointer setup, non-emitting asm barriers/register nudges, and a temporary aliased 3-argument call to the mangled cBaseArray operator to force `a2` liveness. The alias variant fixed the value register but changed the destination register or call-argument scheduling, so it was reverted to the cleaner 2-byte near-match. Ran the permuter twice, including a 300s run from the 2-byte baseline; it generated 9,432 candidates and found no improvement.


## Disassembly

```asm
258ff0:	27bdfff0 	addiu	sp,sp,-16
258ff4:	afb00000 	sw	s0,0(sp)
258ff8:	00808025 	move	s0,a0
258ffc:	afbf0004 	sw	ra,4(sp)
259000:	0c088bc9 	jal	0x222f24
259004:	00a02025 	move	a0,a1
259008:	24440008 	addiu	a0,v0,8
25900c:	8c850000 	lw	a1,0(a0)
259010:	8c870004 	lw	a3,4(a0)
259014:	26060008 	addiu	a2,s0,8
259018:	8c880008 	lw	t0,8(a0)
25901c:	acc50000 	sw	a1,0(a2)
259020:	8c85000c 	lw	a1,12(a0)
259024:	acc70004 	sw	a3,4(a2)
259028:	8c870010 	lw	a3,16(a0)
25902c:	acc80008 	sw	t0,8(a2)
259030:	8c840014 	lw	a0,20(a0)
259034:	acc5000c 	sw	a1,12(a2)
259038:	acc70010 	sw	a3,16(a2)
25903c:	acc40014 	sw	a0,20(a2)
259040:	24440020 	addiu	a0,v0,32
259044:	8c840000 	lw	a0,0(a0)
259048:	26050020 	addiu	a1,s0,32
25904c:	24460024 	addiu	a2,v0,36
259050:	aca40000 	sw	a0,0(a1)
259054:	8cc50000 	lw	a1,0(a2)
259058:	26040024 	addiu	a0,s0,36
25905c:	24460028 	addiu	a2,v0,40
259060:	ac850000 	sw	a1,0(a0)
259064:	8cc60000 	lw	a2,0(a2)
259068:	26050028 	addiu	a1,s0,40
25906c:	aca60000 	sw	a2,0(a1)
259070:	2604002c 	addiu	a0,s0,44
259074:	0c001b9f 	jal	0x6e7c
259078:	2445002c 	addiu	a1,v0,44
25907c:	8fb00000 	lw	s0,0(sp)
259080:	8fbf0004 	lw	ra,4(sp)
259084:	03e00008 	jr	ra
259088:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcState::AssignCopy(const cBase *)
// Address: 0x00258ff0, Size: 156B
// Obj: gcAll_psp.obj

? cBaseArray__operator_eq_constcBaseArrayref(void *, void *, s32, s32); /* extern */
void *dcastdcast_gcStateptr__constcBaseptr(?);      /* extern */

void gcState__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 temp_a2_2;
    s32 temp_a3;
    void *temp_a0;
    void *temp_a2;
    void *temp_v0;

    temp_v0 = dcastdcast_gcStateptr__constcBaseptr(arg1);
    temp_a0 = temp_v0 + 8;
    temp_a2 = arg0 + 8;
    arg0->unk8 = (s32) temp_v0->unk8;
    temp_a2->unk4 = (s32) temp_a0->unk4;
    temp_a3 = temp_a0->unk10;
    temp_a2->unk8 = (s32) temp_a0->unk8;
    temp_a2->unkC = (s32) temp_a0->unkC;
    temp_a2->unk10 = temp_a3;
    temp_a2->unk14 = (s32) temp_a0->unk14;
    arg0->unk20 = (s32) temp_v0->unk20;
    arg0->unk24 = (s32) temp_v0->unk24;
    temp_a2_2 = temp_v0->unk28;
    arg0->unk28 = temp_a2_2;
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0x2C, temp_v0 + 0x2C, temp_a2_2, temp_a3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
