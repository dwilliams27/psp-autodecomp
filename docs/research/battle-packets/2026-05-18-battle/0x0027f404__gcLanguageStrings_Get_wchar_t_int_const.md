# Battle Packet: `gcLanguageStrings::Get(wchar_t *, int) const`

## Target

- Address: `0x0027f404`
- Size: `160` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcLanguageStrings`
- Method family: `Get`
- Leaf: `False`

## Queue Metadata

- score: `159`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0000165c` `untried` 76B `cStrCopy(wchar_t *, const char *, int)`
- `0x00007dec` `untried` 188B `cLanguage::GetLanguageNameLocalized(cLanguage::cLanguages) static`

## Placement

- Canonical source: `src/gcLanguageStrings.cpp`
- Header(s): `include/gcLanguageStrings.h`
- Allowed source(s): `src/gcLanguageStrings.cpp`
- Split-TU prefix: `src/gcLanguageStrings_*.cpp`

## Class Header: `include/gcLanguageStrings.h`

Not found.

## Matched Same-Class Neighbors

- `0x0027f17c` 140B `gcLanguageStrings::New(cMemPool *, cBase *) static` — `src/gcLanguageStrings.cpp`
- `0x0027f208` 220B `gcLanguageStrings::GetType(void) const` — `src/gcLanguageStrings.cpp`
- `0x0027f2e4` 88B `gcLanguageStrings::Write(cFile &) const` — `src/gcLanguageStrings.cpp`
- `0x0027f33c` 200B `gcLanguageStrings::Read(cFile &, cMemPool *)` — `src/gcLanguageStrings.cpp`
- `0x0027f4a4` 176B `gcLanguageStrings::GetName(char *) const` — `src/gcLanguageStrings.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcGameStrings::Get(wchar_t *, int) const`

- Address: `0x0027dd34`
- Size: `128` bytes
- Source: `src/gcGameStrings.cpp`

```cpp
// gcGameStrings::Get(wchar_t *, int) const  @ 0x0027dd34, 128B
// ============================================================
void gcGameStrings::Get(wchar_t *buf, int size) const {
    int v = this->mValue;
    if (v <= 0) {
        if (v >= 0) {
            cStrCopy(buf, cGetBuildDateTime(), size);
        }
    } else if (v < 2) {
        cStrCopy(buf, cGetBuildInfo(), size);
    }
}
```

### Exemplar 2: `gcLobbyConfigStrings::Get(wchar_t *, int) const`

- Address: `0x002806ac`
- Size: `124` bytes
- Source: `src/gcLobbyConfigStrings.cpp`

```cpp
// ── gcLobbyConfigStrings::Get(wchar_t *, int) const  @ 0x002806ac, 124B ──
struct gcLobbyTypeNameEntry {
    short offset;
    short pad;
    char *(*fn)(void *);
};
```

### Exemplar 3: `gcStringTableValue::Get(wchar_t *, int) const`

- Address: `0x00289c38`
- Size: `124` bytes
- Source: `src/gcStringTableValue.cpp`

```cpp
void gcStringTableValue::Get(wchar_t *buf, int size) const {
    gcString *p = this->mDesired.Get(true);
    const wchar_t *src;
    if (p != 0) {
        src = (p->mText != 0) ? p->mText : gcStringTableValueEmptyStr;
    } else {
        src = gcStringTableValueEmptyStr;
    }
    cStrCopy(buf, src, size);
}
```

### Exemplar 4: `gcTableColumnFloat::Get(int, wchar_t *, int) const`

- Address: `0x00271eac`
- Size: `88` bytes
- Source: `src/gcTableColumnFloat.cpp`

```cpp
float gcTableColumnFloat::Get(int row) const {
    return mValues.mData[row];
}
```


## Prior Failure Notes

### Note 1

- session `9ba4fbb1`; src `src/gcLanguageStrings.cpp`; snapshot `logs/failure_snapshots/match_20260510_120905/9ba4fbb1/0027f404__agent_self_reported_failure__src_gcLanguageStrings.cpp`

Matched 148/160 bytes (12 bytes diff). Function size matches (160B), but 6 instructions differ in the conditional-zero block and the dispatch-pointer use. Iterations: (1) Initial `int v=...; int p=v; if (flag) p=0;` produced 132B (size mismatch) with bnez and no duplicate sv.q-style branch; (2) Restructured to `int p; if (flag==0) p=v; else p=0;` got 160B size with 26 byte diff and beqz+nop+b+li branching pattern; (3) Tried adding `int mDesired` field to gcLanguageStrings class with proper gcStringValue layout (`void *_b0; void *_b4;`) to enable member-access codegen — no change to compile output, AND it broke the matched destructor and other matched siblings via class-layout drift, so reverted; (4) Permuter ran 300s with 4078 candidates compiled, found 2 improvements going from 26 → 12 bytes diff. The improvement (saved): reorder local declarations (`int flag = 0; int v = ...;`) and add `__asm__ volatile("" ::: "memory")` at the end of the else branch in the conditional-zero block — this triggered the expected `lw a0, 8(a0)` first scheduling and the 4-instruction beqz/nop/b/li sequence at 0x2c-0x38; (5) Permuter also incidentally swapped `if (keep != 0 && val != 0)` to `if (val != 0 && keep != 0)` in the destructor body which broke its 2-byte match — reverted that change since the dtor was previously matched. Remaining 12-byte diff: at 0x2c the beqz target offset differs by 1 (3→4), at 0x34/0x38 the `li a0, 0` and `b` are swapped (compiler put body before b, expected put body in b's delay slot), at 0x3c we have nop where expected has `move a1, a0`, and at 0x40/0x48 the dispatch loads from a0 where expected uses a1 (ours skips the explicit move-to-a1 and uses a0 directly through the vtable walk). This is a register-allocation drift where the EBOOT's compiler chose to materialize `p` in a fresh register (a1) and emitted an explicit move, while our SNC keeps `p` in a0 and reuses it for the dispatch — both are valid codegen paths. Pre-existing AssignCopy 4-byte size mismatch (608 vs 612) was confirmed via `git stash` to predate this session and is unrelated to the Get changes.


## Disassembly

```asm
27f404:	27bdfff0 	addiu	sp,sp,-16
27f408:	8c840008 	lw	a0,8(a0)
27f40c:	afb10004 	sw	s1,4(sp)
27f410:	00a08825 	move	s1,a1
27f414:	afb00000 	sw	s0,0(sp)
27f418:	34050000 	li	a1,0x0
27f41c:	30870001 	andi	a3,a0,0x1
27f420:	00c08025 	move	s0,a2
27f424:	afbf0008 	sw	ra,8(sp)
27f428:	54e00001 	bnezl	a3,0x27f430
27f42c:	34050001 	li	a1,0x1
27f430:	10a00003 	beqz	a1,0x27f440
27f434:	00000000 	nop
27f438:	10000001 	b	0x27f440
27f43c:	34040000 	li	a0,0x0
27f440:	00802825 	move	a1,a0
27f444:	50a0000a 	beqzl	a1,0x27f470
27f448:	44806000 	mtc1	zero,$f12
27f44c:	8ca50004 	lw	a1,4(a1)
27f450:	24a50070 	addiu	a1,a1,112
27f454:	84a60000 	lh	a2,0(a1)
27f458:	8ca50004 	lw	a1,4(a1)
27f45c:	00a0f809 	jalr	a1
27f460:	00862021 	addu	a0,a0,a2
27f464:	4600030d 	trunc.w.s	$f12,$f0
27f468:	10000003 	b	0x27f478
27f46c:	44046000 	mfc1	a0,$f12
27f470:	4600630d 	trunc.w.s	$f12,$f12
27f474:	44046000 	mfc1	a0,$f12
27f478:	0c001f7b 	jal	0x7dec
27f47c:	00000000 	nop
27f480:	02202025 	move	a0,s1
27f484:	00402825 	move	a1,v0
27f488:	0c000597 	jal	0x165c
27f48c:	02003025 	move	a2,s0
27f490:	8fb00000 	lw	s0,0(sp)
27f494:	8fb10004 	lw	s1,4(sp)
27f498:	8fbf0008 	lw	ra,8(sp)
27f49c:	03e00008 	jr	ra
27f4a0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcLanguageStrings::Get(wchar_t *, int) const
// Address: 0x0027f404, Size: 160B
// Obj: gcAll_psp.obj

s32 cLanguage__GetLanguageNameLocalized_cLanguage__cLanguagesstatic(s32, s32); /* extern */
? cStrCopy_wchar_tptr_constcharptr_int(?, s32, ?);  /* extern */

void gcLanguageStrings__Get_wchar_tptr_intconst(void *arg0, ? arg1, ? arg2) {
    f32 (*temp_a1_2)(s32, ?, s16, s32);
    s16 temp_a2;
    s32 temp_a3;
    s32 var_a0;
    s32 var_a1;
    s32 var_f12;
    void *temp_a1;

    var_a0 = arg0->unk8;
    var_a1 = 0;
    temp_a3 = var_a0 & 1;
    if (temp_a3 != 0) {
        var_a1 = 1;
    }
    if (var_a1 != 0) {
        var_a0 = 0;
    }
    if (var_a0 == 0) {
        var_f12 = (s32) 0.0f;
    } else {
        temp_a1 = var_a0->unk4;
        temp_a2 = temp_a1->unk70;
        temp_a1_2 = (temp_a1 + 0x70)->unk4;
        var_f12 = (s32) temp_a1_2(var_a0 + temp_a2, temp_a1_2, temp_a2, temp_a3);
    }
    cStrCopy_wchar_tptr_constcharptr_int(arg1, cLanguage__GetLanguageNameLocalized_cLanguage__cLanguagesstatic(var_f12, var_f12), arg2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
