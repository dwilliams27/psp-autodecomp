# Battle Packet: `gcValVariable::GetText(char *) const`

## Target

- Address: `0x0036ae00`
- Size: `172` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValVariable`
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
- Tags: `regalloc_drift`, `constant_cse_codegen`, `needs_layout`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`

## Placement

- Canonical source: `src/gcValVariable.cpp`
- Header(s): `include/gcValVariable.h`
- Allowed source(s): `src/gcValVariable.cpp`, `src/gcValVariable_GetText.cpp`
- Split-TU prefix: `src/gcValVariable_*.cpp`

## Class Header: `include/gcValVariable.h`

Not found.

## Matched Same-Class Neighbors

- `0x0036a9a8` 56B `gcValVariable::AssignCopy(const cBase *)` — `src/gcExternalCinematicGroup.cpp`
- `0x0036a9e0` 136B `gcValVariable::New(cMemPool *, cBase *) static` — `src/gcValVariable.cpp`
- `0x0036aa68` 340B `gcValVariable::GetType(void) const` — `src/gcValVariable.cpp`
- `0x0036abbc` 88B `gcValVariable::Write(cFile &) const` — `src/gcValVariable.cpp`
- `0x0036ac14` 220B `gcValVariable::Read(cFile &, cMemPool *)` — `src/gcValVariable.cpp`

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

- session `c9cfa829`; src `src/gcValVariable_GetText.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/c9cfa829/0036ae00__agent_self_reported_failure__src_gcValVariable_GetText.cpp`

Closest C attempt is a split-TU gcValVariable::GetText implementation that exactly matches the handle lookup and object-name control flow, but remains at 10/172 differing bytes. The byte diff is localized to register allocation for the final text pointer: generated code keeps the chosen string/object-name pointer in a2 and emits move a1,a2 in the cStrCat delay slot, while the original writes those same values into a1 at each branch target and leaves the call delay slot as nop.

Tried the in-class method, a split-TU local redeclaration, const char*/char*/void* text variants, typed and raw void* handle objects, D_00038890 symbol vs literal table access, register bindings for h/obj/text/out, object-branch goto restructuring, memory/input barriers, sched=1, and a 120s permuter run (1545 compiled candidates, no improvement). This looks like low-leverage SNC register allocation drift rather than a control-flow mismatch; all non-register behavior bytes line up.


## Disassembly

```asm
36ae00:	27bdfff0 	addiu	sp,sp,-16
36ae04:	00a03025 	move	a2,a1
36ae08:	8c850008 	lw	a1,8(a0)
36ae0c:	afbf0000 	sw	ra,0(sp)
36ae10:	14a00003 	bnez	a1,0x36ae20
36ae14:	00c02025 	move	a0,a2
36ae18:	1000000d 	b	0x36ae50
36ae1c:	34060000 	li	a2,0x0
36ae20:	30a6ffff 	andi	a2,a1,0xffff
36ae24:	3c070004 	lui	a3,0x4
36ae28:	00063080 	sll	a2,a2,0x2
36ae2c:	24e78890 	addiu	a3,a3,-30576
36ae30:	00c73021 	addu	a2,a2,a3
36ae34:	8cc70000 	lw	a3,0(a2)
36ae38:	10e00005 	beqz	a3,0x36ae50
36ae3c:	34060000 	li	a2,0x0
36ae40:	8ce80030 	lw	t0,48(a3)
36ae44:	15050002 	bne	t0,a1,0x36ae50
36ae48:	00000000 	nop
36ae4c:	00e03025 	move	a2,a3
36ae50:	10c0000b 	beqz	a2,0x36ae80
36ae54:	00000000 	nop
36ae58:	84c5001c 	lh	a1,28(a2)
36ae5c:	2ca50001 	sltiu	a1,a1,1
36ae60:	30a500ff 	andi	a1,a1,0xff
36ae64:	10a00004 	beqz	a1,0x36ae78
36ae68:	24c50008 	addiu	a1,a2,8
36ae6c:	3c050037 	lui	a1,0x37
36ae70:	10000001 	b	0x36ae78
36ae74:	24a5dab8 	addiu	a1,a1,-9544
36ae78:	10000007 	b	0x36ae98
36ae7c:	00000000 	nop
36ae80:	10a00004 	beqz	a1,0x36ae94
36ae84:	3c050037 	lui	a1,0x37
36ae88:	3c050037 	lui	a1,0x37
36ae8c:	10000002 	b	0x36ae98
36ae90:	24a5dac4 	addiu	a1,a1,-9532
36ae94:	24a5dacc 	addiu	a1,a1,-9524
36ae98:	0c0004da 	jal	0x1368
36ae9c:	00000000 	nop
36aea0:	8fbf0000 	lw	ra,0(sp)
36aea4:	03e00008 	jr	ra
36aea8:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcValVariable::GetText(char *) const
// Address: 0x0036ae00, Size: 172B
// Obj: gcAll_psp.obj

? cStrCat_charptr_constcharptr(?, void *, void *, void *); /* extern */

void gcValVariable__GetText_charptrconst(void *arg0, ? arg1, void *arg3) {
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
