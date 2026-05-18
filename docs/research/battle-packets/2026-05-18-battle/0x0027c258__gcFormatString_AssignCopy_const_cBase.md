# Battle Packet: `gcFormatString::AssignCopy(const cBase *)`

## Target

- Address: `0x0027c258`
- Size: `108` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcFormatString`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `162`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 2B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
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
- `0x00225844` `untried` 340B `dcast dcast<gcFormatString *>(const cBase *)`

## Placement

- Canonical source: `src/gcFormatString.cpp`
- Header(s): `include/gcFormatString.h`
- Allowed source(s): `src/gcFormatString.cpp`
- Split-TU prefix: `src/gcFormatString_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcFormatString.h`

Not found.

## Matched Same-Class Neighbors

- `0x0027c2c4` 156B `gcFormatString::New(cMemPool *, cBase *) static` — `src/gcFormatString.cpp`
- `0x0027c360` 220B `gcFormatString::GetType(void) const` — `src/gcFormatString.cpp`
- `0x0027c43c` 124B `gcFormatString::Write(cFile &) const` — `src/gcFormatString.cpp`
- `0x0027c94c` 64B `gcFormatString::GetName(char *) const` — `src/gcFormatString.cpp`
- `0x0027d058` 176B `gcFormatString::~gcFormatString(void)` — `src/gcFormatString.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEnumeration::AssignCopy(const cBase *)`

- Address: `0x00238d04`
- Size: `108` bytes
- Source: `src/gcEnumeration.cpp`

```cpp
//   0x00238d04 gcEnumeration::AssignCopy(const cBase *)       108B

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 2: `gcStreamedCinematicConfigGroup::AssignCopy(const cBase *)`

- Address: `0x00244a24`
- Size: `108` bytes
- Source: `src/gcStreamedCinematicConfigGroup.cpp`

```cpp
void gcStreamedCinematicConfigGroup::AssignCopy(const cBase *base) {
    gcStreamedCinematicConfigGroup *src = dcast(base);
    register _CopyBlob *srcBlob __asm__("$4") = (_CopyBlob *)((char *)src + 8);
    register _CopyBlob *dstBlob __asm__("$6") = (_CopyBlob *)((char *)this + 8);
    *dstBlob = *srcBlob;
    ((cBaseArray *)((char *)this + 0x20))->operator=(
        *(cBaseArray *)((char *)src + 0x20));
}
```

### Exemplar 3: `gcRegionSetGroup::AssignCopy(const cBase *)`

- Address: `0x00246090`
- Size: `108` bytes
- Source: `src/gcRegionSetGroup.cpp`

```cpp
//   0x00246090 gcRegionSetGroup::AssignCopy(const cBase *)
//   0x002460fc gcRegionSetGroup::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
```

### Exemplar 4: `gcGenericSubscriber::AssignCopy(const cBase *)`

- Address: `0x0027e6f0`
- Size: `108` bytes
- Source: `src/gcGenericSubscriber.cpp`

```cpp
void gcGenericSubscriber::AssignCopy(const cBase *base) {
    struct Layout {
        char _p0[0x08];
        int f08;
        int f0C;
        unsigned char f10;
        char _p2[3];
        int f14;
        int f18;
        int f1C;
        int f20;
    };
    gcGenericSubscriber *other = dcast(base);
    Layout *self_l = (Layout *)this;
    Layout *other_l = (Layout *)other;
    self_l->f08 = other_l->f08;
    self_l->f0C = other_l->f0C;
    self_l->f10 = other_l->f10;
    self_l->f14 = other_l->f14;
    self_l->f18 = other_l->f18;
    self_l->f1C = other_l->f1C;
    self_l->f20 = other_l->f20;
    ((cSubscriberArray *)((char *)this + 0x24))->operator=(*(cSubscriberArray *)((char *)other + 0x24));
}
```


## Prior Failure Notes

### Note 1

- session `7c1e82c7`

Improved the prior 4-byte register-allocation mismatch to 2/108 bytes by changing the second small-field copy to a cSubHandleT temporary. The current generated sequence matches expected scheduling and addressing exactly except the second scalar copy uses `lw a0, 0(a0)` / `sw a0, 0(a1)` where expected uses `lw a2, 0(a0)` / `sw a2, 0(a1)`. All following cBaseArray::operator= calls and the epilogue line up.

Tried field assignment, int pointer temporaries, split load/store temporaries, direct offset casts, explicit register-variable forms for `$4/$5/$6`, and a no-op asm constraint attempt; the latter worsened allocation and was reverted. Ran `python3 tools/permuter.py src/gcFormatString.cpp 0x0027c258 --time 300 --save-best`: 327.5s elapsed, 10128 candidates generated, 4464 compiled, no improvements, best remained 2 bytes. This still looks like a narrow SNC RA divergence, not a branch-likely issue.

### Note 2

- session `bf940222`

gcFormatString::AssignCopy(const cBase *), 108B. Reduced diff from 7 bytes to 4 bytes with __asm__ volatile("" ::: "memory") barrier between mHandle and mSubHandle assignments, but cannot eliminate the final 4-byte register-allocation divergence. Permuter ran for 5 minutes (5959 candidates compiled, 11304 generated, ~20/sec) and found NO improvement (best stayed at 4). The structural form (addiu+lw+addiu+sw pattern for both small-field copies, with cBaseArray::operator= JALs for the 0x10/0x18 fields) is correct and matches the expected disassembly exactly; only the register choice for the second small-field copy differs.

DIFF DETAIL (4 bytes at offsets ec, f0, f4, f8 in my output vs 27c288, 27c28c, 27c290, 27c294 in expected):
  Expected (a0=src, a2=val, a1=dst):
    27c288: addiu a0, s1, 12
    27c28c: lw    a2, 0(a0)    ; load to a2 (a0 still holds source addr)
    27c290: addiu a1, s0, 12
    27c294: sw    a2, 0(a1)
  Mine (a1=src/val, a0=dst):
    ec:     addiu a1, s1, 12
    f0:     lw    a1, 0(a1)    ; loads into same reg as src addr (overwrite)
    f4:     addiu a0, s0, 12
    f8:     sw    a1, 0(a0)

First small-field copy (offset +8) matches identically: addiu a0,s1,8 / lw a0,0(a0) / addiu a1,s0,8 / sw a0,0(a1). The pattern divergence is ONLY in the second copy.

APPROACHES TRIED:
  1. Plain `mPair.mHandle = o->mPair.mHandle; mPair.mSubHandle = o->mPair.mSubHandle;` -> 7-byte diff (scheduler interleaved next addiu before sw).
  2. With memory barrier between the two assignments -> 4-byte diff (sched fixed but RA still differs).
  3. `this->mPair = o->mPair;` (struct-level copy) -> compiled to 100B, wrong size.
  4. -Xsched=0 and -Xsched=1 also tried - both produce different patterns, neither matches.
  5. Permuter with --time 300 - no improvement found.

ROOT CAUSE HYPOTHESIS: This appears to be RA-divergence between our SNC version and the original. The compiler chose to overwrite the source-addr register (a1) with the loaded value, while the original chose a separate register (a2). Permuter cannot find a source structure that biases SNC to prefer (a0,a2,a1) over (a1,a1,a0) for the second copy. May need a different SNC version or flag we haven't discovered. NOT a bnel issue (no branch-likely involved); does not qualify for unmatchable_bnel classification per CLAUDE.md criteria. Marking failed for retry by future agent who may find a structural trick.


## Disassembly

```asm
27c258:	27bdfff0 	addiu	sp,sp,-16
27c25c:	afb00000 	sw	s0,0(sp)
27c260:	00808025 	move	s0,a0
27c264:	afb10004 	sw	s1,4(sp)
27c268:	afbf0008 	sw	ra,8(sp)
27c26c:	0c089611 	jal	0x225844
27c270:	00a02025 	move	a0,a1
27c274:	00408825 	move	s1,v0
27c278:	26240008 	addiu	a0,s1,8
27c27c:	8c840000 	lw	a0,0(a0)
27c280:	26050008 	addiu	a1,s0,8
27c284:	aca40000 	sw	a0,0(a1)
27c288:	2624000c 	addiu	a0,s1,12
27c28c:	8c860000 	lw	a2,0(a0)
27c290:	2605000c 	addiu	a1,s0,12
27c294:	aca60000 	sw	a2,0(a1)
27c298:	26040010 	addiu	a0,s0,16
27c29c:	0c001b9f 	jal	0x6e7c
27c2a0:	26250010 	addiu	a1,s1,16
27c2a4:	26040018 	addiu	a0,s0,24
27c2a8:	0c001b9f 	jal	0x6e7c
27c2ac:	26250018 	addiu	a1,s1,24
27c2b0:	8fb00000 	lw	s0,0(sp)
27c2b4:	8fb10004 	lw	s1,4(sp)
27c2b8:	8fbf0008 	lw	ra,8(sp)
27c2bc:	03e00008 	jr	ra
27c2c0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcFormatString::AssignCopy(const cBase *)
// Address: 0x0027c258, Size: 108B
// Obj: gcAll_psp.obj

? cBaseArray__operator_eq_constcBaseArrayref(void *, void *, s32); /* extern */
void *dcastdcast_gcFormatStringptr__constcBaseptr(?); /* extern */

void gcFormatString__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 temp_a2;
    void *temp_v0;

    temp_v0 = dcastdcast_gcFormatStringptr__constcBaseptr(arg1);
    arg0->unk8 = (s32) temp_v0->unk8;
    temp_a2 = temp_v0->unkC;
    arg0->unkC = temp_a2;
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0x10, temp_v0 + 0x10, temp_a2);
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0x18, temp_v0 + 0x18);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
