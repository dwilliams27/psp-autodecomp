# Battle Packet: `gcValExternalVariable::GetText(char *) const`

## Target

- Address: `0x003432d4`
- Size: `172` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValExternalVariable`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `159`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`

## Placement

- Canonical source: `src/gcValExternalVariable.cpp`
- Header(s): `include/gcValExternalVariable.h`
- Allowed source(s): `src/gcValExternalVariable.cpp`
- Split-TU prefix: `src/gcValExternalVariable_*.cpp`

## Class Header: `include/gcValExternalVariable.h`

Not found.

## Matched Same-Class Neighbors

- `0x00342ed0` 136B `gcValExternalVariable::New(cMemPool *, cBase *) static` — `src/gcValExternalVariable.cpp`
- `0x00342f58` 340B `gcValExternalVariable::GetType(void) const` — `src/gcValExternalVariable.cpp`
- `0x003430ac` 88B `gcValExternalVariable::Write(cFile &) const` — `src/gcValExternalVariable.cpp`
- `0x00343104` 220B `gcValExternalVariable::Read(cFile &, cMemPool *)` — `src/gcValExternalVariable.cpp`
- `0x00343698` 100B `gcValExternalVariable::~gcValExternalVariable(void)` — `src/gcValExternalVariable.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValEntityIsValid::GetText(char *) const`

- Address: `0x00335910`
- Size: `168` bytes
- Source: `src/gcValEntityIsValid.cpp`

```cpp
void gcValEntityIsValid::GetText(char *buf) const {
    const cTypeMethod *entityText =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x78);
    const char *entityBase = (const char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)entityText->fn)((void *)(entityBase + entityText->offset), buf);

    cStrAppend(buf, (const char *)0x36F34C);
    if (field_34) {
        const cTypeMethod *templateText =
            (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x38))->mType + 0x78);
        const char *templateBase = (const char *)this + 0x38;
        ((TextFn)templateText->fn)((void *)(templateBase + templateText->offset), buf);
        cStrAppend(buf, (const char *)0x36E440);
    }
    cStrAppend(buf, (const char *)0x36EBE4);
}
```

### Exemplar 2: `gcValPointValue::GetText(char *) const`

- Address: `0x003577e8`
- Size: `176` bytes
- Source: `src/gcValExternalVariable.cpp`

```cpp
// gcValPointValue::GetText(char *) const  @ 0x003577e8, 176B
// ─────────────────────────────────────────────────────────────────────────
void gcValPointValue::GetText(char *buf) const {
    const char *space = (const char *)0x36DAF0;
    cStrCat(buf, space);

    if (*(const int *)((const char *)this + 8) == 0) {
        cStrCat(buf, (const char *)0x36E6DC);
        char *base = (char *)this + 0x10;
        const cTypeTextMethod *method =
            (const cTypeTextMethod *)(*(const char **)((const char *)this + 0x14) + 0x78);
        method->fn(base + method->offset, buf);
        cStrCat(buf, (const char *)0x36DCEC);
    }

    cStrCat(buf, (const char *)0x36DADC);
    cStrCat(buf, space);
}
```

### Exemplar 3: `gcValLobbyMailInfo::GetText(char *) const`

- Address: `0x0034a588`
- Size: `184` bytes
- Source: `src/gcValLobbyMailInfo.cpp`

```cpp
// ── gcValLobbyMailInfo::GetText(char *) const @ 0x0034a588 ──
void gcValLobbyMailInfo::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36F4FC);

    int val = *(int *)((const char *)this + 0x08);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    int check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrAppend(buf, (const char *)0x36E060, (const char *)0x36DAF0);
}
```

### Exemplar 4: `gcValLobbyUserInfo::GetText(char *) const`

- Address: `0x0034f5c4`
- Size: `184` bytes
- Source: `src/gcValLobbyMailInfo.cpp`

```cpp
// ── gcValLobbyUserInfo::GetText(char *) const @ 0x0034f5c4 ──
void gcValLobbyUserInfo::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36F574);

    int val = *(int *)((const char *)this + 0x0C);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    int check = val;
    if (check != 0) {
        char *typeInfo = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrAppend(buf, (const char *)0x36E060, (const char *)0x36DAF0);
}
```


## Prior Failure Notes

### Note 1

- session `1938bb4d`; src `src/gcValExternalVariable.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/1938bb4d/003432d4__agent_self_reported_failure__src_gcValExternalVariable.cpp`

Closest source reconstructs the handle-table lookup and object-name selection with the same 172-byte control-flow shape except for the final selected string register. The emitted code keeps the selected text pointer in $a2, then fills the cStrCat jal delay slot with move $a1,$a2; the target keeps the selected pointer in $a1 throughout and has a nop in the jal delay slot. The remaining compare_func result is 10/172 bytes differing, with all lookup/control-flow bytes otherwise aligned.

Tried separate int handle plus pointer text, a single handle/text variable, unsigned selected-value variants, inverted empty-name branches, empty asm register constraints, memory scheduler barriers, and two 300-second permuter runs. The first permuter started from a worse 16-byte baseline and found an 11-byte variant; the second started from the 10-byte hand baseline and found no improvement. Root cause appears to be SNC register allocation/coalescing for the final cStrCat argument rather than a semantic control-flow issue.


## Disassembly

```asm
3432d4:	27bdfff0 	addiu	sp,sp,-16
3432d8:	00a03025 	move	a2,a1
3432dc:	8c850008 	lw	a1,8(a0)
3432e0:	afbf0000 	sw	ra,0(sp)
3432e4:	14a00003 	bnez	a1,0x3432f4
3432e8:	00c02025 	move	a0,a2
3432ec:	1000000d 	b	0x343324
3432f0:	34060000 	li	a2,0x0
3432f4:	30a6ffff 	andi	a2,a1,0xffff
3432f8:	3c070004 	lui	a3,0x4
3432fc:	00063080 	sll	a2,a2,0x2
343300:	24e78890 	addiu	a3,a3,-30576
343304:	00c73021 	addu	a2,a2,a3
343308:	8cc70000 	lw	a3,0(a2)
34330c:	10e00005 	beqz	a3,0x343324
343310:	34060000 	li	a2,0x0
343314:	8ce80030 	lw	t0,48(a3)
343318:	15050002 	bne	t0,a1,0x343324
34331c:	00000000 	nop
343320:	00e03025 	move	a2,a3
343324:	10c0000b 	beqz	a2,0x343354
343328:	00000000 	nop
34332c:	84c5001c 	lh	a1,28(a2)
343330:	2ca50001 	sltiu	a1,a1,1
343334:	30a500ff 	andi	a1,a1,0xff
343338:	10a00004 	beqz	a1,0x34334c
34333c:	24c50008 	addiu	a1,a2,8
343340:	3c050037 	lui	a1,0x37
343344:	10000001 	b	0x34334c
343348:	24a5dab8 	addiu	a1,a1,-9544
34334c:	10000007 	b	0x34336c
343350:	00000000 	nop
343354:	10a00004 	beqz	a1,0x343368
343358:	3c050037 	lui	a1,0x37
34335c:	3c050037 	lui	a1,0x37
343360:	10000002 	b	0x34336c
343364:	24a5dac4 	addiu	a1,a1,-9532
343368:	24a5dacc 	addiu	a1,a1,-9524
34336c:	0c0004da 	jal	0x1368
343370:	00000000 	nop
343374:	8fbf0000 	lw	ra,0(sp)
343378:	03e00008 	jr	ra
34337c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcValExternalVariable::GetText(char *) const
// Address: 0x003432d4, Size: 172B
// Obj: gcAll_psp.obj

? cStrCat_charptr_constcharptr(?, void *, void *, void *); /* extern */

void gcValExternalVariable__GetText_charptrconst(void *arg0, ? arg1, void *arg3) {
    s32 temp_a1;
    void *var_a1;
    void *var_a2;
    void *var_a3;

    var_a3 = arg3;
    temp_a1 = arg0->unk8;
    if (temp_a1 == 0) {
        var_a2 = NULL;
    } else {
        var_a3 = ((temp_a1 & 0xFFFF) * 4)->unk38890;
        var_a2 = NULL;
        if ((var_a3 != NULL) && (var_a3->unk30 == temp_a1)) {
            var_a2 = var_a3;
        }
    }
    if (var_a2 != NULL) {
        var_a1 = var_a2 + 8;
        if ((var_a2->unk1C == 0) & 0xFF) {
            var_a1 = (void *)0x36DAB8;
        }
    } else if (temp_a1 != 0) {
        var_a1 = (void *)0x36DAC4;
    } else {
        var_a1 = (void *)0x36DACC;
    }
    cStrCat_charptr_constcharptr(arg1, var_a1, var_a2, var_a3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
