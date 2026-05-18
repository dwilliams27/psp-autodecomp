# Battle Packet: `gcDoEntityActivatePartialController::GetText(char *) const`

## Target

- Address: `0x002a13e0`
- Size: `292` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoEntityActivatePartialController`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 26B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `26`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `tagged_pointer_shape`
- Near miss: `26`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Placement

- Canonical source: `src/gcDoEntityActivatePartialController.cpp`
- Header(s): `include/gcDoEntityActivatePartialController.h`
- Allowed source(s): `src/gcDoEntityActivatePartialController.cpp`, `src/gcDoEntityActivatePartialController_GetText.cpp`
- Split-TU prefix: `src/gcDoEntityActivatePartialController_*.cpp`

## Class Header: `include/gcDoEntityActivatePartialController.h`

Not found.

## Matched Same-Class Neighbors

- `0x002a0d98` 292B `gcDoEntityActivatePartialController::New(cMemPool *, cBase *) static` — `src/gcDoEntityActivatePartialController.cpp`
- `0x002a0ebc` 280B `gcDoEntityActivatePartialController::GetType(void) const` — `src/gcDoEntityActivatePartialController.cpp`
- `0x002a0fd4` 152B `gcDoEntityActivatePartialController::Write(cFile &) const` — `src/gcDoEntityActivatePartialController.cpp`
- `0x002a1a34` 496B `gcDoEntityActivatePartialController::~gcDoEntityActivatePartialController(void)` — `src/gcDoEntityActivatePartialController_dtor.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoEntitySetPrimaryController::GetText(char *) const`

- Address: `0x002cae40`
- Size: `316` bytes
- Source: `src/gcValHasPartialController.cpp`

```cpp
void gcDoEntitySetPrimaryController::GetText(char *buf) const {
    register const gcDoEntitySetPrimaryController *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    cTypeMethod *entityText =
        (cTypeMethod *)((char *)*(void **)((const char *)self + 0x10) + 0x78);
    char *entityBase = (char *)self + 0x0C;
    ((void (*)(void *, char *))entityText->fn)(entityBase + entityText->offset,
                                               out);

    unsigned char usePrimary = *(unsigned char *)((const char *)self + 0x3D);
    unsigned char enabled = *(unsigned char *)((const char *)self + 0x3C);
    if (usePrimary != 0) {
        const char *fmt = (const char *)0x36EA8C;
        const char *state;
        if (enabled != 0) {
            state = (const char *)0x36E428;
        } else {
            state = (const char *)0x36EAA8;
        }
        cStrAppend(out, fmt, state);
    } else {
        const char *fmt = (const char *)0x36EAAC;
        const char *state;
        if (enabled != 0) {
            state = (const char *)0x36E428;
        } else {
            state = (const char *)0x36EAA8;
        }
        cStrAppend(out, fmt, state);

        int val = *(int *)((const char *)self + 0x38);
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
            slot->fn((char *)check + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrAppend(out, (const char *)0x36DCEC);
    }
}
```

### Exemplar 2: `gcDoEntityAssignPath::GetText(char *) const`

- Address: `0x002b1a20`
- Size: `248` bytes
- Source: `src/gcDoEntityAssignPath.cpp`

```cpp
void gcDoEntityAssignPath::GetText(char *buf) const {
    char text[256];

    char *typeInfo0 = *(char **)((const char *)this + 0x14);
    GetTextRec *rec0 = (GetTextRec *)(typeInfo0 + 0x78);
    char *base0 = (char *)this + 0x10;
    rec0->fn(base0 + rec0->offset, buf);

    int flag = (*(int *)((const char *)this + 0x0C) & 8) != 0;
    flag &= 0xFF;
    if (flag != 0) {
        cStrCat(buf, (const char *)0x36E694);
        return;
    }

    char *typeInfo1 = *(char **)((const char *)this + 0x40);
    text[0] = '\0';
    GetTextRec *rec1 = (GetTextRec *)(typeInfo1 + 0x78);
    char *base1 = (char *)this + 0x3C;
    rec1->fn(base1 + rec1->offset, text);

    cStrAppend(buf, (const char *)0x36E6A8, text);

    int empty = *(int *)((const char *)this + 0x60) == 0;
    empty &= 0xFF;
    if (empty == 0) {
        cStrAppend(buf, (const char *)0x36E6B8);
    }
    cStrAppend(buf, (const char *)0x36DCEC);
}
```

### Exemplar 3: `gcDoUISetTextSprite::GetText(char *) const`

- Address: `0x00315c34`
- Size: `248` bytes
- Source: `src/gcDoEntityAssignPath.cpp`

```cpp
void gcDoUISetTextSprite::GetText(char *buf) const {
    char text[256];
    text[0] = *text = '\0';

    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->GetText(text);
    cStrAppend(buf, (const char *)0x36F108, text);

    int val = *(int *)((const char *)this + 0x18);
    int owned = 0;
    if (val & 1) {
        owned = 1;
    }
    if (owned != 0) {
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

    cStrCat(buf, (const char *)0x36DAD8);

    char *typeInfo2 = *(char **)((const char *)this + 0x20);
    GetTextRec *rec2 = (GetTextRec *)(typeInfo2 + 0x78);
    char *base2 = (char *)this + 0x1C;
    rec2->fn(base2 + rec2->offset, buf);

    cStrCat(buf, (const char *)0x36DCEC);
}
```

### Exemplar 4: `gcDoStatsTrackingOp::GetText(char *) const`

- Address: `0x00304220`
- Size: `236` bytes
- Source: `src/gcDoStatsTrackingOp.cpp`

```cpp
void gcDoStatsTrackingOp::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36EF60, (const char *)0x36DAF0);

    int flag = 0;
    int op = *(int *)((const char *)this + 0x0C);
    if (op == 7 || op == 9 || op == 8) {
        flag = 1;
    }
    int useValue = flag & 0xFF;
    if (useValue != 0) {
        int val = *(int *)((const char *)this + 0x10);
        int owned = 0;
        if (val & 1) {
            owned = 1;
        }
        if (owned != 0) {
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
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}
```


## Prior Failure Notes

### Note 1

- session `237053a9`; src `src/gcDoEntityActivatePartialController_GetText.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/237053a9/002a13e0__agent_self_reported_failure__src_gcDoEntityActivatePartialController_GetText.cpp`

Closest attempt is a split-TU canonical C++ method in src/gcDoEntityActivatePartialController_GetText.cpp. It reconstructs the full 292-byte control-flow shape and compares at 26/292 bytes different after the helper-call bool scheduling fix. Existing matched methods in src/gcDoEntityActivatePartialController.cpp still compare cleanly.

Tried direct in-class implementation, split TU, several tagged-pointer rewrites, bool/int flag variations, separate objCheck variables, empty asm scheduling barriers, and a 300s permuter pass; the permuter improved the baseline from 41 to 37 bytes before the manual helper-call fix brought it to 26. The remaining diff mirrors gcValEntityVariable::GetText: the target keeps the inner tag flag/check in a1/a2 and uses move a1,a0 plus beqzl, while this TU allocates the flag to s1 and uses beqz a0 with a nearby addiu/lw sequence. Likely root cause is SNC register-allocation/branch-likely drift in the inner object rendering idiom.


## Disassembly

```asm
2a13e0:	27bdfef0 	addiu	sp,sp,-272
2a13e4:	afb10104 	sw	s1,260(sp)
2a13e8:	00808825 	move	s1,a0
2a13ec:	8e240010 	lw	a0,16(s1)
2a13f0:	afb00100 	sw	s0,256(sp)
2a13f4:	a3a00000 	sb	zero,0(sp)
2a13f8:	00a08025 	move	s0,a1
2a13fc:	24840078 	addiu	a0,a0,120
2a1400:	84850000 	lh	a1,0(a0)
2a1404:	2626000c 	addiu	a2,s1,12
2a1408:	8c870004 	lw	a3,4(a0)
2a140c:	00c52021 	addu	a0,a2,a1
2a1410:	afbf0108 	sw	ra,264(sp)
2a1414:	00e0f809 	jalr	a3
2a1418:	02002825 	move	a1,s0
2a141c:	34060001 	li	a2,0x1
2a1420:	26240044 	addiu	a0,s1,68
2a1424:	0c043a56 	jal	0x10e958
2a1428:	03a02825 	move	a1,sp
2a142c:	8e240050 	lw	a0,80(s1)
2a1430:	34050000 	li	a1,0x0
2a1434:	30860001 	andi	a2,a0,0x1
2a1438:	54c00001 	bnezl	a2,0x2a1440
2a143c:	34050001 	li	a1,0x1
2a1440:	10a00003 	beqz	a1,0x2a1450
2a1444:	0004202b 	sltu	a0,zero,a0
2a1448:	10000003 	b	0x2a1458
2a144c:	34040000 	li	a0,0x0
2a1450:	308400ff 	andi	a0,a0,0xff
2a1454:	0004202b 	sltu	a0,zero,a0
2a1458:	1080001f 	beqz	a0,0x2a14d8
2a145c:	3c050037 	lui	a1,0x37
2a1460:	3c050037 	lui	a1,0x37
2a1464:	03a02025 	move	a0,sp
2a1468:	0c000470 	jal	0x11c0
2a146c:	24a5dcbc 	addiu	a1,a1,-9028
2a1470:	8e240050 	lw	a0,80(s1)
2a1474:	34050000 	li	a1,0x0
2a1478:	30860001 	andi	a2,a0,0x1
2a147c:	54c00001 	bnezl	a2,0x2a1484
2a1480:	34050001 	li	a1,0x1
2a1484:	10a00003 	beqz	a1,0x2a1494
2a1488:	00000000 	nop
2a148c:	10000001 	b	0x2a1494
2a1490:	34040000 	li	a0,0x0
2a1494:	00802825 	move	a1,a0
2a1498:	50a0000b 	beqzl	a1,0x2a14c8
2a149c:	3c050037 	lui	a1,0x37
2a14a0:	8ca60004 	lw	a2,4(a1)
2a14a4:	03a02825 	move	a1,sp
2a14a8:	24c600d0 	addiu	a2,a2,208
2a14ac:	84c70000 	lh	a3,0(a2)
2a14b0:	8cc60004 	lw	a2,4(a2)
2a14b4:	00c0f809 	jalr	a2
2a14b8:	00872021 	addu	a0,a0,a3
2a14bc:	10000006 	b	0x2a14d8
2a14c0:	3c050037 	lui	a1,0x37
2a14c4:	3c050037 	lui	a1,0x37
2a14c8:	03a02025 	move	a0,sp
2a14cc:	0c0004da 	jal	0x1368
2a14d0:	24a5db24 	addiu	a1,a1,-9436
2a14d4:	3c050037 	lui	a1,0x37
2a14d8:	3c060037 	lui	a2,0x37
2a14dc:	02002025 	move	a0,s0
2a14e0:	03a03825 	move	a3,sp
2a14e4:	24a5e498 	addiu	a1,a1,-7016
2a14e8:	0c000470 	jal	0x11c0
2a14ec:	24c6daf0 	addiu	a2,a2,-9488
2a14f0:	8fb00100 	lw	s0,256(sp)
2a14f4:	8fb10104 	lw	s1,260(sp)
2a14f8:	8fbf0108 	lw	ra,264(sp)
2a14fc:	03e00008 	jr	ra
2a1500:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcDoEntityActivatePartialController::GetText(char *) const
// Address: 0x002a13e0, Size: 292B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, s32, void *); /* extern */
? cStrCat_charptr_constcharptr(void *, ?, s32);     /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcDoEntityActivatePartialController__GetText_charptrconst(void *arg0, void *arg1) {
    ? (*temp_a2_5)(s32, void *, ?, s16);
    ? (*temp_a3)(void *, void *, void *, ?);
    s16 temp_a3_2;
    s32 temp_a0_2;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    void *temp_a0;
    void *temp_a2;
    void *temp_a2_4;

    temp_a0 = arg0->unk10;
    temp_a2 = arg0 + 0xC;
    temp_a3 = (temp_a0 + 0x78)->unk4;
    temp_a3(temp_a2 + temp_a0->unk78, arg1, temp_a2, temp_a3);
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x44, sp, 1);
    temp_a0_2 = arg0->unk50;
    var_a1 = 0;
    temp_a2_2 = temp_a0_2 & 1;
    if (temp_a2_2 != 0) {
        var_a1 = 1;
    }
    if (var_a1 != 0) {
        var_a0 = 0;
    } else {
        var_a0 = ((temp_a0_2 != 0) & 0xFF) != 0;
    }
    if (var_a0 != 0) {
        cStrAppend_charptr_constcharptr_...(sp, 0x36DCBC, temp_a2_2);
        var_a0_2 = arg0->unk50;
        var_a1_2 = 0;
        temp_a2_3 = var_a0_2 & 1;
        if (temp_a2_3 != 0) {
            var_a1_2 = 1;
        }
        if (var_a1_2 != 0) {
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            temp_a2_4 = var_a0_2->unk4;
            temp_a3_2 = temp_a2_4->unkD0;
            temp_a2_5 = (temp_a2_4 + 0xD0)->unk4;
            temp_a2_5(var_a0_2 + temp_a3_2, sp, temp_a2_5, temp_a3_2);
        } else {
            cStrCat_charptr_constcharptr(sp, 0x36DB24, temp_a2_3);
        }
    }
    cStrAppend_charptr_constcharptr_...(arg1, 0x36E498, 0x36DAF0, sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
