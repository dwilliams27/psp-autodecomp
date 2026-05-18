# Battle Packet: `gcValHasPartialController::GetText(char *) const`

## Target

- Address: `0x00346654`
- Size: `312` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValHasPartialController`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `123`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 23B; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `23`

## Failure Classification

- Primary: `tagged_pointer_shape`
- Action: `research`
- Confidence: `medium`
- Tags: `tagged_pointer_shape`, `retryable_nearmiss`
- Near miss: `23`
- Summary: Tagged/nullable owned-pointer branch-shape issue.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Placement

- Canonical source: `src/gcValHasPartialController.cpp`
- Header(s): `include/gcValHasPartialController.h`
- Allowed source(s): `src/gcValHasPartialController.cpp`
- Split-TU prefix: `src/gcValHasPartialController_*.cpp`

## Class Header: `include/gcValHasPartialController.h`

Not found.

## Matched Same-Class Neighbors

- `0x00346044` 296B `gcValHasPartialController::New(cMemPool *, cBase *) static` — `src/gcValHasPartialController.cpp`
- `0x0034616c` 280B `gcValHasPartialController::GetType(void) const` — `src/gcValHasPartialController.cpp`
- `0x00346284` 152B `gcValHasPartialController::Write(cFile &) const` — `src/gcValHasPartialController.cpp`
- `0x0034631c` 328B `gcValHasPartialController::Read(cFile &, cMemPool *)` — `src/gcValHasPartialController.cpp`
- `0x00346cbc` 476B `gcValHasPartialController::~gcValHasPartialController(void)` — `src/gcValHasPartialController.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValEntitySoundFrequency::GetText(char *) const`

- Address: `0x0033e020`
- Size: `276` bytes
- Source: `src/gcValEntitySoundFrequency.cpp`

```cpp
void gcValEntitySoundFrequency::GetText(char *buf) const {
    cTypeMethod *entityText =
        (cTypeMethod *)((char *)((gcDesiredObject *)((char *)this + 8))->mType +
                        0x78);
    char *entityBase = (char *)this + 8;
    ((void (*)(void *, char *))entityText->fn)(entityBase + entityText->offset,
                                               buf);

    const char *fmt = (const char *)0x36F3B0;
    const char *state;
    if (*(unsigned char *)((char *)this + 0x38) != 0) {
        state = (const char *)0x36F3C0;
    } else if (*(unsigned char *)((char *)this + 0x39) != 0) {
        state = (const char *)0x36F3C8;
    } else {
        state = (const char *)0x36F3D0;
    }

    cStr text;
    char *built = cStr_ctor(&text, fmt, state);
    __asm__ volatile("" : "+r"(built));
    cStrAppend(buf, built);

    int val = *(int *)((char *)this + 0x34);
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

    cStrAppend(buf, (const char *)0x36E2E8);
}
```

### Exemplar 2: `gcValObjectIsValid::GetText(char *) const`

- Address: `0x00356a30`
- Size: `268` bytes
- Source: `src/gcValObjectIsValid.cpp`

```cpp
void gcValObjectIsValid::GetText(char *buf) const {
    int obj = mObject;
    int flag = 0;
    int tag = obj & 1;
    if (tag != 0) {
        flag = 1;
    }
    int valid;
    if (flag == 0) goto text_not_tagged;
    valid = 0;
    goto text_valid_done;
text_not_tagged:
    valid = obj != 0;
    valid &= 0xFF;
    valid = valid != 0;
text_valid_done:
    if (valid != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }
        int base;
        char *type;
        if (flag2 == 0) goto text_untagged_base;
        base = 0;
        type = *(char **)(base + 4);
        goto text_type_done;
text_untagged_base:
        base = obj;
        type = *(char **)(base + 4);
text_type_done:
        cTypeMethod *text = (cTypeMethod *)(type + 0x78);
        short textOff = text->offset;
        register void *textBase __asm__("$5") = (char *)base + textOff;
        __asm__ volatile("" : "+r"(textBase));
        void (*textFn)(void *, char *) = (void (*)(void *, char *))text->fn;
        textFn(textBase, buf);

        obj = mObject;
        int flag3 = 0;
        const char *fmt = (const char *)0x36F6CC;
        int tag2 = obj & 1;
        if (tag2 != 0) {
            flag3 = 1;
        }
        int base2 = obj;
        char *type2;
        if (flag3 != 0) {
            base2 = 0;
            type2 = *(char **)(base2 + 4);
        } else {
            type2 = *(char **)(base2 + 4);
        }
        cTypeMethod *name = (cTypeMethod *)(type2 + 0x80);
        short nameOff = name->offset;
        NamedObject *(*nameFn)(void *) = (NamedObject *(*)(void *))name->fn;
        cStrAppend(buf, fmt, nameFn((char *)base2 + nameOff)->name);
    } else {
        cStrAppend(buf, (const char *)0x36F6E0);
    }
}
```

### Exemplar 3: `gcValUIListInfo::GetText(char *) const`

- Address: `0x00363080`
- Size: `264` bytes
- Source: `src/gcValUIListInfo.cpp`

```cpp
// gcValUIListInfo::GetText(char *) const  @ 0x00363080, 264B
// -----------------------------------------------------------------------------
void gcValUIListInfo::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->GetText(local);
    cStrAppend(buf, (const char *)0x36DF34, local, (const char *)0x36DAF0);

    int flag = 0;
    int kind = mField;
    if ((kind == 4) || (kind == 5)) {
        flag = 1;
    }
    int doExtra = flag & 0xFF;
    if (doExtra != 0) {
        char valueText[256];
        valueText[0] = '\0';
        __asm__ volatile("" ::: "memory");
        int val = pad18;
        register char *valueBuf __asm__("$17") = valueText;
        int tagged = 0;
        if (val & 1) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val + slot->offset, valueBuf);
        } else {
            cStrCat(valueBuf, (const char *)0x36DB24);
        }
        cStrAppend(buf, (const char *)0x36DE40, valueBuf);
    }
}
```

### Exemplar 4: `gcValLobbyFriendInfo::GetText(char *) const`

- Address: `0x00348654`
- Size: `252` bytes
- Source: `src/gcValLobbyFriendInfo.cpp`

```cpp
void gcValLobbyFriendInfo::GetText(char *buf) const {
    const char *empty = (const char *)0x36DAF0;

    if (field_10 == 0x10) {
        cStrAppend(buf, (const char *)0x36F4C4);
        cStrAppend(buf, (const char *)0x36DCB8, empty);
    } else {
        cStrAppend(buf, (const char *)0x36F4D0);

        int val = field_C._value;
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

        cStrAppend(buf, (const char *)0x36E060, empty);
    }
}
```


## Prior Failure Notes

### Note 1

- session `36ed7da4`; src `src/gcValHasPartialController.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/36ed7da4/00346654__agent_self_reported_failure__src_gcValHasPartialController.cpp`

Closest attempt compiles and keeps all previously matched sibling methods intact, but gcValHasPartialController::GetText remains at 23/312 bytes different. I matched the opening virtual desired-object GetText call, the partial-controller string selection, the first tagged-pointer validity test, and the final suffix; the remaining disassembly differences are localized to two areas: the helper GetText(false) call emits addiu a0,this,0x3c before li a2,0 while the original emits li a2,0 first, and the second tagged-pointer dispatch reload allocates the value in s1 instead of keeping it in a0/a1/a2 like the original.

Tried high-level if/else, goto-shaped bool conversion matching gcValObjectIsValid, separate and reused val variables, explicit a0/a1/a2 register bindings, keeping self live through inline asm, memory barriers, and a 300s permuter run (8,952 generated / 4,158 compiled candidates) with no improvement over the 23-byte baseline. Root cause appears to be SNC register/scheduler drift around the second tagged pointer dispatch; source-level leverage was exhausted without converting the function to assembly.


## Disassembly

```asm
346654:	27bdfff0 	addiu	sp,sp,-16
346658:	afb10004 	sw	s1,4(sp)
34665c:	00808825 	move	s1,a0
346660:	8e24000c 	lw	a0,12(s1)
346664:	afb00000 	sw	s0,0(sp)
346668:	00a08025 	move	s0,a1
34666c:	24840078 	addiu	a0,a0,120
346670:	84850000 	lh	a1,0(a0)
346674:	26260008 	addiu	a2,s1,8
346678:	8c870004 	lw	a3,4(a0)
34667c:	00c52021 	addu	a0,a2,a1
346680:	afbf0008 	sw	ra,8(sp)
346684:	00e0f809 	jalr	a3
346688:	02002825 	move	a1,s0
34668c:	9224004c 	lbu	a0,76(s1)
346690:	10800004 	beqz	a0,0x3466a4
346694:	3c050037 	lui	a1,0x37
346698:	3c050037 	lui	a1,0x37
34669c:	10000002 	b	0x3466a8
3466a0:	24a5f45c 	addiu	a1,a1,-2980
3466a4:	24a5f47c 	addiu	a1,a1,-2948
3466a8:	0c0004da 	jal	0x1368
3466ac:	02002025 	move	a0,s0
3466b0:	34060000 	li	a2,0x0
3466b4:	2624003c 	addiu	a0,s1,60
3466b8:	0c043a56 	jal	0x10e958
3466bc:	02002825 	move	a1,s0
3466c0:	8e240048 	lw	a0,72(s1)
3466c4:	34050000 	li	a1,0x0
3466c8:	30860001 	andi	a2,a0,0x1
3466cc:	54c00001 	bnezl	a2,0x3466d4
3466d0:	34050001 	li	a1,0x1
3466d4:	10a00003 	beqz	a1,0x3466e4
3466d8:	0004202b 	sltu	a0,zero,a0
3466dc:	10000003 	b	0x3466ec
3466e0:	34040000 	li	a0,0x0
3466e4:	308400ff 	andi	a0,a0,0xff
3466e8:	0004202b 	sltu	a0,zero,a0
3466ec:	1080001f 	beqz	a0,0x34676c
3466f0:	3c050037 	lui	a1,0x37
3466f4:	3c050037 	lui	a1,0x37
3466f8:	02002025 	move	a0,s0
3466fc:	0c000470 	jal	0x11c0
346700:	24a5dcbc 	addiu	a1,a1,-9028
346704:	8e240048 	lw	a0,72(s1)
346708:	34050000 	li	a1,0x0
34670c:	30860001 	andi	a2,a0,0x1
346710:	54c00001 	bnezl	a2,0x346718
346714:	34050001 	li	a1,0x1
346718:	10a00003 	beqz	a1,0x346728
34671c:	00000000 	nop
346720:	10000001 	b	0x346728
346724:	34040000 	li	a0,0x0
346728:	00802825 	move	a1,a0
34672c:	50a0000b 	beqzl	a1,0x34675c
346730:	3c050037 	lui	a1,0x37
346734:	8ca60004 	lw	a2,4(a1)
346738:	02002825 	move	a1,s0
34673c:	24c600d0 	addiu	a2,a2,208
346740:	84c70000 	lh	a3,0(a2)
346744:	8cc60004 	lw	a2,4(a2)
346748:	00c0f809 	jalr	a2
34674c:	00872021 	addu	a0,a0,a3
346750:	10000006 	b	0x34676c
346754:	3c050037 	lui	a1,0x37
346758:	3c050037 	lui	a1,0x37
34675c:	02002025 	move	a0,s0
346760:	0c0004da 	jal	0x1368
346764:	24a5db24 	addiu	a1,a1,-9436
346768:	3c050037 	lui	a1,0x37
34676c:	02002025 	move	a0,s0
346770:	0c0004da 	jal	0x1368
346774:	24a5e2e8 	addiu	a1,a1,-7448
346778:	8fb00000 	lw	s0,0(sp)
34677c:	8fb10004 	lw	s1,4(sp)
346780:	8fbf0008 	lw	ra,8(sp)
346784:	03e00008 	jr	ra
346788:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcValHasPartialController::GetText(char *) const
// Address: 0x00346654, Size: 312B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, s32);   /* extern */
? cStrCat_charptr_constcharptr(?, s32, s32);        /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, ?, ?); /* extern */

void gcValHasPartialController__GetText_charptrconst(void *arg0, ? arg1) {
    ? (*temp_a2_5)(s32, ?, ?, s16);
    ? (*temp_a3)(void *, ?, void *, ?);
    s16 temp_a3_2;
    s32 temp_a0_2;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    void *temp_a0;
    void *temp_a2;
    void *temp_a2_4;

    temp_a0 = arg0->unkC;
    temp_a2 = arg0 + 8;
    temp_a3 = (temp_a0 + 0x78)->unk4;
    temp_a3(temp_a2 + temp_a0->unk78, arg1, temp_a2, temp_a3);
    if (arg0->unk4C != 0) {
        var_a1 = 0x36F45C;
    } else {
        var_a1 = 0x36F47C;
    }
    cStrCat_charptr_constcharptr(arg1, var_a1);
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x3C, arg1, 0);
    temp_a0_2 = arg0->unk48;
    var_a1_2 = 0;
    temp_a2_2 = temp_a0_2 & 1;
    if (temp_a2_2 != 0) {
        var_a1_2 = 1;
    }
    if (var_a1_2 != 0) {
        var_a0 = 0;
    } else {
        var_a0 = ((temp_a0_2 != 0) & 0xFF) != 0;
    }
    if (var_a0 != 0) {
        cStrAppend_charptr_constcharptr_...(arg1, 0x36DCBC, temp_a2_2);
        var_a0_2 = arg0->unk48;
        var_a1_3 = 0;
        temp_a2_3 = var_a0_2 & 1;
        if (temp_a2_3 != 0) {
            var_a1_3 = 1;
        }
        if (var_a1_3 != 0) {
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            temp_a2_4 = var_a0_2->unk4;
            temp_a3_2 = temp_a2_4->unkD0;
            temp_a2_5 = (temp_a2_4 + 0xD0)->unk4;
            temp_a2_5(var_a0_2 + temp_a3_2, arg1, temp_a2_5, temp_a3_2);
        } else {
            cStrCat_charptr_constcharptr(arg1, 0x36DB24, temp_a2_3);
        }
    }
    cStrCat_charptr_constcharptr(arg1, 0x36E2E8);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
