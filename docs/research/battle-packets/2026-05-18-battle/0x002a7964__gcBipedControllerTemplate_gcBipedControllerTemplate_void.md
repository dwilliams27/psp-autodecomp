# Battle Packet: `gcBipedControllerTemplate::~gcBipedControllerTemplate(void)`

## Target

- Address: `0x002a7964`
- Size: `204` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcBipedControllerTemplate`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 25B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `25`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `25`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x000070bc` `untried` 240B `cBaseArray::RemoveAll(void)`
- `0x001ac690` `matched` 36B `free`

## Placement

- Canonical source: `src/gcBipedControllerTemplate.cpp`
- Header(s): `include/gcBipedControllerTemplate.h`
- Allowed source(s): `src/gcBipedControllerTemplate.cpp`
- Split-TU prefix: `src/gcBipedControllerTemplate_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/gcBipedControllerTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x00140f84` 88B `gcBipedControllerTemplate::Write(cFile &) const` — `src/gcBipedControllerTemplate.cpp`
- `0x001410d8` 68B `gcBipedControllerTemplate::gcBipedControllerTemplate(cBase *)` — `src/gcBipedControllerTemplate.cpp`
- `0x002a7854` 272B `gcBipedControllerTemplate::GetType(void) const` — `src/gcBipedControllerTemplate.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEntityControllerTemplate::~gcEntityControllerTemplate(void)`

- Address: `0x0025d4c8`
- Size: `204` bytes
- Source: `src/gcEntityControllerTemplate.cpp`

```cpp
//   0x0025d4c8 gcEntityControllerTemplate::~gcEntityControllerTemplate(void)

class cBase;
class cFile;
class cMemPool;
class gcEnumeration;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 2: `gcCreatureControllerTemplate::~gcCreatureControllerTemplate(void)`

- Address: `0x002940b8`
- Size: `204` bytes
- Source: `src/gcCreatureControllerTemplate.cpp`

```cpp
// -- gcCreatureControllerTemplate::~gcCreatureControllerTemplate(void) --
gcCreatureControllerTemplate::~gcCreatureControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    cBaseArray *arr1 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *arr0 = (cBaseArray *)((char *)this + 0x08);
    if (arr1 != 0) {
        arr1->RemoveAll();
    }
    if (arr0 != 0) {
        arr0->RemoveAll();
    }
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}
```

### Exemplar 3: `gcFlyingControllerTemplate::~gcFlyingControllerTemplate(void)`

- Address: `0x0031b730`
- Size: `204` bytes
- Source: `src/gcFlyingControllerTemplate.cpp`

```cpp
//   gcFlyingControllerTemplate::~gcFlyingControllerTemplate(void)    @ 0x0031B730  (204B)
//
// Inherits gcCreatureControllerTemplate.

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

### Exemplar 4: `gcDesiredEnumerationEntry::~gcDesiredEnumerationEntry(void)`

- Address: `0x0025b744`
- Size: `212` bytes
- Source: `src/gcDesiredEnumerationEntry.cpp`

```cpp
gcDesiredEnumerationEntry::~gcDesiredEnumerationEntry() {
    *(char **)((char *)this + 4) = gcDesiredEnumerationEntryvirtualtable;
    char *slot = (char *)this + 0x14;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }
    *(char **)((char *)this + 4) = gcDesiredEnumerationEntry_cBase_vtable;
}
```


## Prior Failure Notes

### Note 1

- session `6a6e21cb`; src `src/gcBipedControllerTemplate.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/6a6e21cb/002a7964__agent_self_reported_failure__src_gcBipedControllerTemplate.cpp`

Same blocker as prior attempt: 196B / 204B (8 bytes short). The first 196 bytes match byte-exact. Missing trailing trap pair `b $; nop` (1000ffff 00000000) at offsets 0xc4/0xc8 AFTER the jr ra epilogue. Prior agent exhausted 8+ approaches (in-class operator delete, file-scope inline operator delete, symbol-folded vptr stores, explicit local-variable null-check pointers, __builtin_unreachable [SNC error 0020], for(;;) loop after delete [moved trap to wrong location, 25-byte diff], noreturn attribute on free [silently ignored]). This session: did not retry — the prior agent's analysis is exhaustive and correctly characterizes the divergence as structural. SNC's data-flow analyzer emits a post-epilogue trailing trap when the deleting destructor inlines operator delete via the cMemPool dispatch table with an `else free(p)` fallback. Many other unmatched dtors share this same trailing-trap pattern (ePhysicsControllerTemplate at 0x4bab4, eDynamicFluid at 0x5ddc0, eCylinderShape at 0x74320, eShadowFillModelMtl at 0x7f3d8, eHeightmapData at 0x8fb70). Per CLAUDE.md, this is not a bnel-related divergence so cannot be classified `unmatchable_bnel`. Recommend solving ePhysicsControllerTemplate dtor first — finding the right shape there will likely solve all of these together. Body source as committed is byte-exact for the first 196B of 204B expected.

### Note 2

- session `11cbde0c`

196B vs expected 204B — 8 bytes short. The first 196 bytes are byte-exact (vptr swap to gcEntityControllerTemplate dtor classdesc at 0x388738; explicit p1=this+0x1C, p2=this+0x8 locals using s1/s2 with beqz null-checks before each cBaseArray::RemoveAll call; vptr swap to cBase dtor classdesc at 0x37E6A8; andi a0,s1,1 deleting-tail check; inlined operator delete with cMemPool::GetPoolFromPtr -> rec->fn dispatch on the non-null path and free(p) on the else path; converging epilogue restoring s0/s1/s2/ra and addiu sp,sp,16 in jr ra delay slot). The missing 8 bytes are an unreachable `b $; nop` trap pair (1000ffff 00000000) AT 0xc4 / 0xc8 in the expected, AFTER the jr ra epilogue. Approaches tested: (1) in-class static operator delete with else-free fallback (196B, missing trap); (2) file-scope inline operator delete with same body (196B — identical bytes, current state); (3) symbol-folded vptr stores (declared gcEntityControllerTemplate_dtor_classdesc and cBase_dtor_classdesc as separate extern symbols so SNC fuses the +offset into a single addiu lo16 reloc, vs &array[off] which generated lui+addiu+addiu); (4) explicit local-variable null-check pointers p1/p2 to force the s2/beqz idiom for both RemoveAll calls; (5) added `__builtin_unreachable()` after dtor body — SNC error 0020: identifier undefined (SNC doesn't support GCC builtins); (6) added `for (;;) {}` after operator delete inside the inline function — produced 204B but moved the trap BEFORE the epilogue at 0xf0 (with for-loop body in flow), wrong placement (25 byte diff); (7) declared free with __attribute__((noreturn)) — SNC silently ignored the attribute (still 196B, identical bytes); (8) various combinations of (1)-(7). Searched all built .o files for the trap pattern — only _pure_error_ and __abort_execution have it, both immediately after a noreturn-style call to abort(). No matched destructor in the codebase emits a trailing post-epilogue trap; checked gcRegion::~gcRegion (172B, no trap), eRigidBodyControllerTemplate::~eRigidBodyControllerTemplate (124B, no trap). Many UNMATCHED dtors in expected/ have this same trailing-trap pattern (ePhysicsControllerTemplate at 0x4bab4, eDynamicFluid at 0x5ddc0, eCylinderShape at 0x74320, eShadowFillModelMtl at 0x7f3d8, eHeightmapData at 0x8fb70) — they all have inlined operator delete with a free() fallback. Hypothesis: SNC emits the trailing trap only when the deleting destructor inlines operator delete via the cMemPool dispatch table AND there is an `else free(p)` fallback path. The trap is emitted at the end of the function body as a 'fall-through guard', possibly because SNC's data-flow analyzer concludes the post-epilogue location is dead-code-reachable through some control-flow transform. Cannot be classified unmatchable_bnel per CLAUDE.md criteria — diff is not bnel-related. Permuter not run because the divergence is structural (two missing post-epilogue instructions), not a regalloc/scheduling diff that random source mutations could discover. Possible angles for next agent: (a) investigate ePhysicsControllerTemplate dtor at 0x4bab4 since it has the same pattern AND is unmatched — trying to match it will probably solve this one too; (b) try a different operator delete signature shape (e.g., taking size_t as second param, or as a static class method with a wrapper); (c) try wrapping the inlined operator delete in a separate noinline helper; (d) experiment with -Xxopt / -Xmopt flag variations on this single function; (e) check if -Xsched=1 produces the trap (project default is sched=2 for gcAll_psp.obj — see docs/decisions/003-compiler-flags.md).


## Disassembly

```asm
2a7964:	27bdfff0 	addiu	sp,sp,-16
2a7968:	afb00000 	sw	s0,0(sp)
2a796c:	00808025 	move	s0,a0
2a7970:	afb10004 	sw	s1,4(sp)
2a7974:	afb20008 	sw	s2,8(sp)
2a7978:	afbf000c 	sw	ra,12(sp)
2a797c:	10800024 	beqz	a0,0x2a7a10
2a7980:	00a08825 	move	s1,a1
2a7984:	3c040039 	lui	a0,0x39
2a7988:	24848738 	addiu	a0,a0,-30920
2a798c:	ae040004 	sw	a0,4(s0)
2a7990:	2604001c 	addiu	a0,s0,28
2a7994:	10800003 	beqz	a0,0x2a79a4
2a7998:	26120008 	addiu	s2,s0,8
2a799c:	0c001c2f 	jal	0x70bc
2a79a0:	00000000 	nop
2a79a4:	12400004 	beqz	s2,0x2a79b8
2a79a8:	3c040038 	lui	a0,0x38
2a79ac:	0c001c2f 	jal	0x70bc
2a79b0:	02402025 	move	a0,s2
2a79b4:	3c040038 	lui	a0,0x38
2a79b8:	2484e6a8 	addiu	a0,a0,-6488
2a79bc:	ae040004 	sw	a0,4(s0)
2a79c0:	32240001 	andi	a0,s1,0x1
2a79c4:	10800012 	beqz	a0,0x2a7a10
2a79c8:	00000000 	nop
2a79cc:	0c0011e8 	jal	0x47a0
2a79d0:	02002025 	move	a0,s0
2a79d4:	00402025 	move	a0,v0
2a79d8:	1080000b 	beqz	a0,0x2a7a08
2a79dc:	00000000 	nop
2a79e0:	8c840024 	lw	a0,36(a0)
2a79e4:	02002825 	move	a1,s0
2a79e8:	8c86001c 	lw	a2,28(a0)
2a79ec:	24c60030 	addiu	a2,a2,48
2a79f0:	84c70000 	lh	a3,0(a2)
2a79f4:	8cc60004 	lw	a2,4(a2)
2a79f8:	00c0f809 	jalr	a2
2a79fc:	00872021 	addu	a0,a0,a3
2a7a00:	10000003 	b	0x2a7a10
2a7a04:	00000000 	nop
2a7a08:	0c06b1a4 	jal	0x1ac690
2a7a0c:	02002025 	move	a0,s0
2a7a10:	8fb00000 	lw	s0,0(sp)
2a7a14:	8fb10004 	lw	s1,4(sp)
2a7a18:	8fb20008 	lw	s2,8(sp)
2a7a1c:	8fbf000c 	lw	ra,12(sp)
2a7a20:	03e00008 	jr	ra
2a7a24:	27bd0010 	addiu	sp,sp,16
2a7a28:	1000ffff 	b	0x2a7a28
2a7a2c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcBipedControllerTemplate::~gcBipedControllerTemplate(void)
// Address: 0x002a7964, Size: 204B
// Obj: gcAll_psp.obj

? cBaseArray__RemoveAll_void(void *);               /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *); /* extern */
? free(void *);                                     /* extern */

void gcBipedControllerTemplate___dtor_gcBipedControllerTemplate_void(void *arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    s16 temp_a3;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a2;
    void *temp_s2;
    void *temp_v0;

    if (arg0 != NULL) {
        arg0->unk4 = 0x388738;
        temp_a0 = arg0 + 0x1C;
        temp_s2 = arg0 + 8;
        if (temp_a0 != NULL) {
            cBaseArray__RemoveAll_void(temp_a0);
        }
        if (temp_s2 != NULL) {
            cBaseArray__RemoveAll_void(temp_s2);
        }
        arg0->unk4 = 0x37E6A8;
        if (arg1 & 1) {
            temp_v0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0);
            if (temp_v0 != NULL) {
                temp_a0_2 = temp_v0->unk24;
                temp_a2 = temp_a0_2->unk1C;
                temp_a3 = temp_a2->unk30;
                temp_a2_2 = (temp_a2 + 0x30)->unk4;
                temp_a2_2(temp_a0_2 + temp_a3, arg0, temp_a2_2, temp_a3);
                return;
            }
            free(arg0);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
