# Battle Packet: `gcDoReturn::Evaluate(void) const`

## Target

- Address: `0x002fa058`
- Size: `192` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoReturn`
- Method family: `Evaluate`
- Leaf: `True`

## Queue Metadata

- score: `137`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 51B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `51`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `tagged_pointer_shape`
- Near miss: `51`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/gcDoReturn.cpp`
- Header(s): `include/gcDoReturn.h`
- Allowed source(s): `src/gcDoReturn.cpp`
- Split-TU prefix: `src/gcDoReturn_*.cpp`

## Class Header: `include/gcDoReturn.h`

```cpp
#ifndef GCDORETURN_H
#define GCDORETURN_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoReturn {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
    ~gcDoReturn(void);
    static void operator delete(void *);
};

class gcDoSetValue {
public:
    gcDoSetValue(cBase *);
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    ~gcDoSetValue(void);
    static void operator delete(void *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    gcDoSetValue &operator=(const gcDoSetValue &);
};

class gcDoStateFunction {
public:
    ~gcDoStateFunction(void);
    static void operator delete(void *);
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    const cType *GetType(void) const;
    gcDoStateFunction &operator=(const gcDoStateFunction &);
};

class gcDoStateParentHandler {
public:
    ~gcDoStateParentHandler(void);
    static void operator delete(void *);
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    void GetText(char *) const;
    const cType *GetType(void) const;
};

struct gcDoSwitchCaseData {
    char _pad[0x0C];
    gcExpression **mChildren;
    char _pad2[0x04];
    gcExpression *mBranch;
};

class gcDoSwitchCase {
public:
    gcDoSwitchCase(cBase *);
    ~gcDoSwitchCase(void);
    static void operator delete(void *);
    const cType *GetType(void) const;
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

struct gcValCaseRangeData {
    char _pad[0x08];
    gcExpression **mChildren;
};

class gcValCaseRange {
public:
    gcValCaseRange(cBase *);
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    float Evaluate(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

class gcDoSwitch {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x002f9cf0` 144B `gcDoReturn::New(cMemPool *, cBase *) static` — `src/gcDoReturn_New.cpp`
- `0x002f9d80` 280B `gcDoReturn::GetType(void) const` — `src/gcDoReturn.cpp`
- `0x002f9e98` 124B `gcDoReturn::Write(cFile &) const` — `src/gcValCaseRange.cpp`
- `0x002fa118` 40B `gcDoReturn::GetText(char *) const` — `src/gcDoReturn.cpp`
- `0x002fa140` 8B `gcDoReturn::GetMaxChildren(void) const` — `src/gcDoReturn.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoEvaluation::Evaluate(void) const`

- Address: `0x0014b09c`
- Size: `144` bytes
- Source: `src/gcDoEvaluation.cpp`

```cpp
float gcDoEvaluation::Evaluate(void) const {
    int val = *(int *)((char *)this + 0xC);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    const void *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (const void *)val;
    }
    const gcExprVEntry *e = *(const gcExprVEntry **)((const char *)ptr + 4) + 14;
    float result = e->fn((const char *)ptr + e->adj);
    float r;
    if (result != 0.0f) {
        r = gcExpressionList_Evaluate((char *)this + 0x10);
    } else {
        r = gcExpressionList_Evaluate((char *)this + 0x18);
    }
    return r;
}
```

### Exemplar 2: `gcDoTableQuery::Evaluate(void) const`

- Address: `0x00306b30`
- Size: `96` bytes
- Source: `src/gcDoTableQuery.cpp`

```cpp
float gcDoTableQuery::Evaluate(void) const {
    gcDesiredTableTemplateHelper *table =
        ((const gcDesiredObjectT<gcDesiredTableTemplate, gcDesiredTableTemplateHelper, gcTableTemplate> *)((const char *)this + 0x0C))->Get(true);
    float result = 1.0f;
    if (table != 0) {
        ((gcTableTemplate *)table)->Query(
            *(const cBaseArrayT<gcValue *> *)((const char *)this + 0x20),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x28),
            *(const gcEvent *)((const char *)this + 0x30),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x4C),
            *(cBaseArrayT<gcValue *> *)((const char *)this + 0x68));
    }
    return result;
}
```

### Exemplar 3: `gcDoPortalActivate::Evaluate(void) const`

- Address: `0x003199a0`
- Size: `72` bytes
- Source: `src/gcDoPortalActivate.cpp`

```cpp
float gcDoPortalActivate::Evaluate(void) const {
    gcDesiredPortalHelper *portal =
        ((const gcDesiredObjectT<gcDesiredPortal, gcDesiredPortalHelper, ePortal> *)((const char *)this + 0x0C))->Get(true);
    if (portal != 0) {
        portal->Activate(*(const bool *)((const char *)this + 0x20));
    }
    return 1.0f;
}
```

### Exemplar 4: `gcDoBreak::Evaluate(void) const`

- Address: `0x00294608`
- Size: `44` bytes
- Source: `src/gcDoBreak.cpp`

```cpp
float gcDoBreak::Evaluate(void) const {
    int *flags = 0;
    gcDoBreakRoot *root = g_gcDoBreak_root;
    if (root != 0) {
        flags = root->flags;
    }
    *flags |= 2;
    return 1.0f;
}
```


## Prior Failure Notes

### Note 1

- session `e6ada47c`; src `src/gcDoReturn.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/e6ada47c/002fa058__agent_self_reported_failure__src_gcDoReturn.cpp`

192B Evaluate could not be matched byte-exact. Structural progress reduced diff from 64B → 52B → 50B via canonicalize-in-else pattern (mirroring gcExpressionList::Evaluate's flag-bit canonicalization with explicit `int raw = (val != 0); hasChild = ((unsigned char)raw) != 0;`) plus permuter (150s, 5040 candidates, best=8B by its metric but 50B by compare_func.py's metric). Remaining 50B diff is purely register allocation: my build assigns `ctx` to $a1 and `tag` to $a2; expected has `ctx` in $a2 and `tag` in $a1. The swap propagates throughout the function — every register reference shifts by one slot, producing ~50 bytes of identical-instruction-different-register diffs. Approaches tried: (1) declaration reordering of ctx/result/tag (made it worse: 78-92B); (2) `register asm()` GCC constraints (SNC silently ignores them); (3) bool vs unsigned char vs int canonicalization variants for hasTag/hasChild; (4) reordering of `if (ctx != 0)` block relative to tag computation; (5) permuter random restructurings.

Root cause is almost certainly SNC compiler-version divergence in greedy register allocator priorities — same problem class as the bnel branch-likely heuristic divergence documented in docs/decisions/006. Per CLAUDE.md the unmatchable-classification gate requires ≤8B diff specifically in bnel/beql, so this 50B register-allocation diff doesn't qualify and stays `failed`. Cannot be fixed at source level. Source code structure left in place as the closest reasonable C++ form — future work would either patch pspcor.exe register-allocation priorities or wait for a compiler-version match.

### Note 2

- session `f1184f8b`

gcDoReturn::Evaluate(void) const — 192 bytes, gcAll_psp.obj.

Got to 184 bytes (8 bytes / 2 instructions short). The function logic is fully decompiled: loads this->mChild (offset 0x0C), resolves tagged pointer, loads g_gcEvalCtx and evalState, dispatches through vtable at typeInfo+0x70 (DispatchEntry struct with short offset + func pointer), stores result to EvalResult (flags|=1, val=float), returns val.

The blocking issue is beqzl vs beqz in the hasChild computation. Expected code uses `beqz a2, L1; sltu a2,zero,a0` (regular branch, delay slot always executes, sltu overwrites the tested register a2). Our SNC generates `move a3,a2; li a2,0; beqzl a3,+1; sltu a2,zero,a0` (branch-likely, saves hasTag to a3 first, delay only executes when taken). The beqzl version saves 2 instructions (-8 bytes) — an optimization our SNC makes but the original compiler didn't.

Also tried 'always-compute bool then override' pattern which gets correct 192-byte size but has completely different structure (linear sltu+andi+sltu+bnezl override vs expected branchy beqz+sltu+b+li+andi+sltu) and different register allocation throughout (ctx in a1 vs expected a2). The register swap propagates through the entire function causing 67/192 bytes to differ.

Tried: if/else, ternary, direct tag test, compute-then-override, pre-compute nz, __asm__ volatile barriers, register asm constraints, -Xsched=1, -O5. Ran permuter for 5 minutes (5597 candidates) — no improvement beyond 51 bytes differ. The beqzl choice appears to be a genuine compiler version heuristic difference in SNC's delay slot scheduler.

### Note 3

- session `3da12355`

Closest match: 184 bytes compiled vs 192 expected (8 bytes short). Frame size (16 bytes), register usage (s0 + ra), prologue, ctx null check (bnezl a2; lw s0, 16(a2)), tagged pointer child extraction, vtable dispatch at typeInfo+0x70 (with DispatchEntry struct for correct offset loads), float result storage (swc1 f12, 4(s0) via EvalResult struct), flag update (ori/sw), and epilogue all match correctly.

The blocking issue is in the hasChild computation. Expected uses beqz (regular branch, delay slot always executes) with `sltu a2,zero,a0` in the delay slot, followed by `andi a2,a2,0xff; sltu a2,zero,a2` in the !hasTag path. My code generates beqzl (branch likely, delay annulled when not taken), which eliminates the sltu in the hasTag path and omits the andi+sltu follow-up in the !hasTag path — saving 2 instructions but not matching. The `(bool)val` expression in `hasChild = (bool)val` should produce `sltu + andi 0xff`, and the subsequent `if (hasChild)` should add another `sltu + beqz`. Instead, SNC's beqzl optimization collapses this to just `sltu` in the annulled delay slot + `beqz`. Also, the `lui` for the global address uses a1 instead of a2 (expected uses a2 for both lui and lw).

Recommendation: The beqz vs beqzl choice seems to be driven by SNC's delay slot optimization heuristics. The expected code's beqz forces the sltu to always execute, creating the need for the andi+sltu cleanup. Try: (1) computing `val != 0` before the if/else (always-compute pattern), (2) using the hasChild result in an intermediate expression that forces the full bool conversion, (3) checking if a different variable ordering changes SNC's beqz/beqzl preference.


## Disassembly

```asm
2fa058:	27bdfff0 	addiu	sp,sp,-16
2fa05c:	8c84000c 	lw	a0,12(a0)
2fa060:	3c060038 	lui	a2,0x38
2fa064:	8cc6d7b4 	lw	a2,-10316(a2)
2fa068:	afb00000 	sw	s0,0(sp)
2fa06c:	34100000 	li	s0,0x0
2fa070:	30850001 	andi	a1,a0,0x1
2fa074:	afbf0004 	sw	ra,4(sp)
2fa078:	54c00001 	bnezl	a2,0x2fa080
2fa07c:	8cd00010 	lw	s0,16(a2)
2fa080:	34060000 	li	a2,0x0
2fa084:	54a00001 	bnezl	a1,0x2fa08c
2fa088:	34060001 	li	a2,0x1
2fa08c:	10c00003 	beqz	a2,0x2fa09c
2fa090:	0004302b 	sltu	a2,zero,a0
2fa094:	10000003 	b	0x2fa0a4
2fa098:	34060000 	li	a2,0x0
2fa09c:	30c600ff 	andi	a2,a2,0xff
2fa0a0:	0006302b 	sltu	a2,zero,a2
2fa0a4:	10c00012 	beqz	a2,0x2fa0f0
2fa0a8:	00a03025 	move	a2,a1
2fa0ac:	34050000 	li	a1,0x0
2fa0b0:	54c00001 	bnezl	a2,0x2fa0b8
2fa0b4:	34050001 	li	a1,0x1
2fa0b8:	10a00005 	beqz	a1,0x2fa0d0
2fa0bc:	00802825 	move	a1,a0
2fa0c0:	34050000 	li	a1,0x0
2fa0c4:	8ca40004 	lw	a0,4(a1)
2fa0c8:	10000003 	b	0x2fa0d8
2fa0cc:	24840070 	addiu	a0,a0,112
2fa0d0:	8ca40004 	lw	a0,4(a1)
2fa0d4:	24840070 	addiu	a0,a0,112
2fa0d8:	84860000 	lh	a2,0(a0)
2fa0dc:	8c870004 	lw	a3,4(a0)
2fa0e0:	00e0f809 	jalr	a3
2fa0e4:	00a62021 	addu	a0,a1,a2
2fa0e8:	10000002 	b	0x2fa0f4
2fa0ec:	46000306 	mov.s	$f12,$f0
2fa0f0:	44806000 	mtc1	zero,$f12
2fa0f4:	e60c0004 	swc1	$f12,4(s0)
2fa0f8:	8e040000 	lw	a0,0(s0)
2fa0fc:	c6000004 	lwc1	$f0,4(s0)
2fa100:	34840001 	ori	a0,a0,0x1
2fa104:	ae040000 	sw	a0,0(s0)
2fa108:	8fb00000 	lw	s0,0(sp)
2fa10c:	8fbf0004 	lw	ra,4(sp)
2fa110:	03e00008 	jr	ra
2fa114:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcDoReturn::Evaluate(void) const
// Address: 0x002fa058, Size: 192B
// Obj: gcAll_psp.obj

f32 gcDoReturn__Evaluate_voidconst(void *arg0) {
    f32 (*temp_a3)(s32, s32, s16, ?);
    f32 var_f12;
    s16 temp_a2_2;
    s32 temp_a0;
    s32 temp_a1;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a2_2;
    void *temp_a2;
    void *var_a0;
    void *var_s0;

    temp_a0 = arg0->unkC;
    temp_a2 = *(void **)0x37D7B4;
    var_s0 = NULL;
    temp_a1 = temp_a0 & 1;
    if (temp_a2 != NULL) {
        var_s0 = temp_a2->unk10;
    }
    var_a2 = 0;
    if (temp_a1 != 0) {
        var_a2 = 1;
    }
    if (var_a2 != 0) {
        var_a2_2 = 0;
    } else {
        var_a2_2 = ((temp_a0 != 0) & 0xFF) != 0;
    }
    if (var_a2_2 != 0) {
        var_a1 = 0;
        if (temp_a1 != 0) {
            var_a1 = 1;
        }
        var_a1_2 = temp_a0;
        if (var_a1 != 0) {
            var_a1_2 = 0;
            var_a0 = *4;
        } else {
            var_a0 = var_a1_2->unk4;
        }
        temp_a2_2 = var_a0->unk70;
        temp_a3 = (var_a0 + 0x70)->unk4;
        var_f12 = temp_a3(var_a1_2 + temp_a2_2, var_a1_2, temp_a2_2, temp_a3);
    } else {
        var_f12 = 0.0f;
    }
    var_s0->unk4 = var_f12;
    var_s0->unk0 = (s32) (var_s0->unk0 | 1);
    return var_s0->unk4;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
