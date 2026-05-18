# Battle Packet: `gcValEntityVariable::GetText(char *) const`

## Target

- Address: `0x003407ec`
- Size: `288` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValEntityVariable`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `tagged_pointer_shape`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Placement

- Canonical source: `src/gcValEntityVariable.cpp`
- Header(s): `include/gcValEntityVariable.h`
- Allowed source(s): `src/gcValEntityVariable.cpp`, `src/gcValEntityVariable_GetText.cpp`
- Split-TU prefix: `src/gcValEntityVariable_*.cpp`

## Class Header: `include/gcValEntityVariable.h`

Not found.

## Matched Same-Class Neighbors

- `0x0033ffc0` 292B `gcValEntityVariable::New(cMemPool *, cBase *) static` — `src/gcValEntityVariable.cpp`
- `0x003400e4` 340B `gcValEntityVariable::GetType(void) const` — `src/gcValEntityVariable.cpp`
- `0x00340238` 140B `gcValEntityVariable::Write(cFile &) const` — `src/gcValEntityVariable.cpp`
- `0x00340e3c` 476B `gcValEntityVariable::~gcValEntityVariable(void)` — `src/gcValEntityVariable.cpp`

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

- session `237053a9`; src `src/gcValEntityVariable_GetText.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/237053a9/003407ec__agent_self_reported_failure__src_gcValEntityVariable_GetText.cpp`

Closest attempt is a split-TU canonical C++ method in src/gcValEntityVariable_GetText.cpp. It reconstructs the full control flow and, after forcing the gcDesiredEnumerationEntryHelper bool argument into a pre-call value, compares at 16/288 bytes different. Existing matched siblings in src/gcValEntityVariable.cpp still compare cleanly; the GetText attempt is isolated per the split-TU rule.

Tried direct in-class implementation, split TU, tagged-pointer variants with if/else, explicit gotos, bool/int flag shapes, separate objVal/objCheck variables, register-variable hints, empty asm barriers with and without memory clobbers, and a 300s permuter pass. The remaining diff is concentrated in the inner tagged-object rendering path: original uses a1/a2 for the tag flag and a move a1,a0 plus beqzl before loading typeInfo, while SNC keeps choosing s1 for the flag and emits beqz a0 with an addiu/lw typeInfo load sequence. This appears to be register-allocation / branch-likely shape drift rather than missing control flow.


## Disassembly

```asm
3407ec:	27bdfef0 	addiu	sp,sp,-272
3407f0:	afb10104 	sw	s1,260(sp)
3407f4:	00808825 	move	s1,a0
3407f8:	8e24000c 	lw	a0,12(s1)
3407fc:	afb00100 	sw	s0,256(sp)
340800:	00a08025 	move	s0,a1
340804:	24840078 	addiu	a0,a0,120
340808:	84850000 	lh	a1,0(a0)
34080c:	26260008 	addiu	a2,s1,8
340810:	8c870004 	lw	a3,4(a0)
340814:	00c52021 	addu	a0,a2,a1
340818:	afbf0108 	sw	ra,264(sp)
34081c:	00e0f809 	jalr	a3
340820:	02002825 	move	a1,s0
340824:	a3a00000 	sb	zero,0(sp)
340828:	a3a00000 	sb	zero,0(sp)
34082c:	34060000 	li	a2,0x0
340830:	2624003c 	addiu	a0,s1,60
340834:	0c043a56 	jal	0x10e958
340838:	03a02825 	move	a1,sp
34083c:	8e240048 	lw	a0,72(s1)
340840:	34050000 	li	a1,0x0
340844:	30860001 	andi	a2,a0,0x1
340848:	54c00001 	bnezl	a2,0x340850
34084c:	34050001 	li	a1,0x1
340850:	10a00003 	beqz	a1,0x340860
340854:	0004202b 	sltu	a0,zero,a0
340858:	10000003 	b	0x340868
34085c:	34040000 	li	a0,0x0
340860:	308400ff 	andi	a0,a0,0xff
340864:	0004202b 	sltu	a0,zero,a0
340868:	1080001f 	beqz	a0,0x3408e8
34086c:	3c050037 	lui	a1,0x37
340870:	3c050037 	lui	a1,0x37
340874:	03a02025 	move	a0,sp
340878:	0c000470 	jal	0x11c0
34087c:	24a5dcbc 	addiu	a1,a1,-9028
340880:	8e240048 	lw	a0,72(s1)
340884:	34050000 	li	a1,0x0
340888:	30860001 	andi	a2,a0,0x1
34088c:	54c00001 	bnezl	a2,0x340894
340890:	34050001 	li	a1,0x1
340894:	10a00003 	beqz	a1,0x3408a4
340898:	00000000 	nop
34089c:	10000001 	b	0x3408a4
3408a0:	34040000 	li	a0,0x0
3408a4:	00802825 	move	a1,a0
3408a8:	50a0000b 	beqzl	a1,0x3408d8
3408ac:	3c050037 	lui	a1,0x37
3408b0:	8ca60004 	lw	a2,4(a1)
3408b4:	03a02825 	move	a1,sp
3408b8:	24c600d0 	addiu	a2,a2,208
3408bc:	84c70000 	lh	a3,0(a2)
3408c0:	8cc60004 	lw	a2,4(a2)
3408c4:	00c0f809 	jalr	a2
3408c8:	00872021 	addu	a0,a0,a3
3408cc:	10000006 	b	0x3408e8
3408d0:	3c050037 	lui	a1,0x37
3408d4:	3c050037 	lui	a1,0x37
3408d8:	03a02025 	move	a0,sp
3408dc:	0c0004da 	jal	0x1368
3408e0:	24a5db24 	addiu	a1,a1,-9436
3408e4:	3c050037 	lui	a1,0x37
3408e8:	02002025 	move	a0,s0
3408ec:	03a03025 	move	a2,sp
3408f0:	0c000470 	jal	0x11c0
3408f4:	24a5f3ec 	addiu	a1,a1,-3092
3408f8:	8fb00100 	lw	s0,256(sp)
3408fc:	8fb10104 	lw	s1,260(sp)
340900:	8fbf0108 	lw	ra,264(sp)
340904:	03e00008 	jr	ra
340908:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcValEntityVariable::GetText(char *) const
// Address: 0x003407ec, Size: 288B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, s32); /* extern */
? cStrCat_charptr_constcharptr(void *, ?, s32);     /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcValEntityVariable__GetText_charptrconst(void *arg0, void *arg1) {
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

    temp_a0 = arg0->unkC;
    temp_a2 = arg0 + 8;
    temp_a3 = (temp_a0 + 0x78)->unk4;
    temp_a3(temp_a2 + temp_a0->unk78, arg1, temp_a2, temp_a3);
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x3C, sp, 0);
    temp_a0_2 = arg0->unk48;
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
        var_a0_2 = arg0->unk48;
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
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F3EC, (s32) sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
