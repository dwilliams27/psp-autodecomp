# Battle Packet: `gcString::AssignCopy(const cBase *)`

## Target

- Address: `0x0023a890`
- Size: `156` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcString`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `170`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 5B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `5`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `5`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0022046c` `untried` 280B `dcast dcast<gcString *>(const cBase *)`
- `0x00233a7c` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<wchar_t>::operator=(const cArrayBase<cArrayBase> &)`

## Placement

- Canonical source: `src/gcString.cpp`
- Header(s): `include/gcString.h`
- Allowed source(s): `src/gcString.cpp`
- Split-TU prefix: `src/gcString_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcString.h`

Not found.

## Matched Same-Class Neighbors

- `0x000d6214` 264B `gcString::Write(cFile &) const` — `src/gcString.cpp`
- `0x000d675c` 52B `gcString::gcString(cBase *)` — `src/eRenderSurface.cpp`
- `0x0023a92c` 124B `gcString::New(cMemPool *, cBase *) static` — `src/gcString.cpp`
- `0x0023a9a8` 160B `gcString::GetType(void) const` — `src/gcString.cpp`
- `0x0023aa48` 56B `gcString::GetName(char *) const` — `src/gcMsgCheckSynchronization.cpp`

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

- session `cc9d98af`

Improved gcString::AssignCopy(const cBase *) from the prior 11-byte mismatch to a 5-byte mismatch. The useful manual change was splitting the 0x0C destination pointer into dst0C before computing src + 0x10, which matches the expected scheduling through the store at 0x23a8d0. Remaining mismatch is only the two 0x10/0x14 scalar value registers: expected loads/stores use lw a1,0(a2) / sw a1,0(a0) and lw a2,0(a2) / sw a2,0(a1), while the best C shape still uses a0 for both loaded values. Tried declaration-order variants, direct pointer assignment, explicit register binding for a1/a2, and aggregate IntPair assignment; aggregate got the value registers but shortened the function and used sw ...,4(a0), so it is not viable.

Ran `python3 tools/permuter.py src/gcString.cpp 0x0023a890 --time 300 --save-best` from the 5-byte baseline. It generated 11,088 candidates, compiled 4,356, and found zero improvements. This is not a bnel issue; control flow and calls match, and the remaining problem is a narrow SNC register-allocation/source-shape issue in the middle two-word copy sequence.

### Note 2

- session `370a140f`

Implemented the full high-level AssignCopy behavior: dcast to gcString, copy scalar fields at 0x08 and 0x0C, copy the two-word field at 0x10/0x14, assign the two cArrayBase<wchar_t> wrappers at 0x18 and 0x1C, then copy the scalar at 0x20. The current source compiles to the correct 156B size and all control flow/calls match, but 11 bytes differ in the register allocation for the 0x10/0x14 scalar copies. Expected uses a2 as the source pointer for both words, a1/a2 as loaded values, and a0/a1 as destination pointers; the compiled source uses a0 for the first source/value and a1 as the destination, then a2 only for the second source pointer.

Tried direct member copies, volatile pointer copies, empty register barriers, a two-word aggregate assignment, aggregate base barriers, explicit pointer pipelining, local register variable hints, sched=1 around AssignCopy, and predeclared array assignment operands to change register pressure. The best manual form is the current pointer-pipeline source with an 11-byte diff. Ran `python3 tools/permuter.py src/gcString.cpp 0x0023a890 --time 300 --save-best`; it generated 9,888 candidates, compiled 4,032, and found zero improvement over the 11-byte baseline. This is not a bnel issue; it appears to be a narrow SNC register-allocation/source-shape problem in the middle scalar copy sequence.


## Disassembly

```asm
23a890:	27bdfff0 	addiu	sp,sp,-16
23a894:	afb00000 	sw	s0,0(sp)
23a898:	00808025 	move	s0,a0
23a89c:	afb10004 	sw	s1,4(sp)
23a8a0:	afbf0008 	sw	ra,8(sp)
23a8a4:	0c08811b 	jal	0x22046c
23a8a8:	00a02025 	move	a0,a1
23a8ac:	00408825 	move	s1,v0
23a8b0:	26240008 	addiu	a0,s1,8
23a8b4:	8c840000 	lw	a0,0(a0)
23a8b8:	26050008 	addiu	a1,s0,8
23a8bc:	aca40000 	sw	a0,0(a1)
23a8c0:	2624000c 	addiu	a0,s1,12
23a8c4:	8c840000 	lw	a0,0(a0)
23a8c8:	2605000c 	addiu	a1,s0,12
23a8cc:	26260010 	addiu	a2,s1,16
23a8d0:	aca40000 	sw	a0,0(a1)
23a8d4:	8cc50000 	lw	a1,0(a2)
23a8d8:	26040010 	addiu	a0,s0,16
23a8dc:	26260014 	addiu	a2,s1,20
23a8e0:	ac850000 	sw	a1,0(a0)
23a8e4:	8cc60000 	lw	a2,0(a2)
23a8e8:	26050014 	addiu	a1,s0,20
23a8ec:	aca60000 	sw	a2,0(a1)
23a8f0:	26040018 	addiu	a0,s0,24
23a8f4:	0c08ce9f 	jal	0x233a7c
23a8f8:	26250018 	addiu	a1,s1,24
23a8fc:	2604001c 	addiu	a0,s0,28
23a900:	0c08ce9f 	jal	0x233a7c
23a904:	2625001c 	addiu	a1,s1,28
23a908:	26240020 	addiu	a0,s1,32
23a90c:	8c840000 	lw	a0,0(a0)
23a910:	26050020 	addiu	a1,s0,32
23a914:	aca40000 	sw	a0,0(a1)
23a918:	8fb00000 	lw	s0,0(sp)
23a91c:	8fb10004 	lw	s1,4(sp)
23a920:	8fbf0008 	lw	ra,8(sp)
23a924:	03e00008 	jr	ra
23a928:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcString::AssignCopy(const cBase *)
// Address: 0x0023a890, Size: 156B
// Obj: gcAll_psp.obj

? cArrayBase_cArrayBase_refcArrayBase_wchar_t___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *, s32); /* extern */
void *dcastdcast_gcStringptr__constcBaseptr(?);     /* extern */

void gcString__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 temp_a2;
    void *temp_v0;

    temp_v0 = dcastdcast_gcStringptr__constcBaseptr(arg1);
    arg0->unk8 = (s32) temp_v0->unk8;
    arg0->unkC = (s32) temp_v0->unkC;
    arg0->unk10 = (s32) temp_v0->unk10;
    temp_a2 = temp_v0->unk14;
    arg0->unk14 = temp_a2;
    cArrayBase_cArrayBase_refcArrayBase_wchar_t___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x18, temp_v0 + 0x18, temp_a2);
    cArrayBase_cArrayBase_refcArrayBase_wchar_t___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x1C, temp_v0 + 0x1C);
    arg0->unk20 = (s32) temp_v0->unk20;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
