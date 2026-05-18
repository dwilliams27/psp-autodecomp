# Battle Packet: `gcValEntityConstant::GetText(char *) const`

## Target

- Address: `0x003268c4`
- Size: `288` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValEntityConstant`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `168`
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

- Canonical source: `src/gcValEntityConstant.cpp`
- Header(s): `include/gcValEntityConstant.h`
- Allowed source(s): `src/gcValEntityConstant.cpp`
- Split-TU prefix: `src/gcValEntityConstant_*.cpp`

## Class Header: `include/gcValEntityConstant.h`

Not found.

## Matched Same-Class Neighbors

- `0x00326318` 292B `gcValEntityConstant::New(cMemPool *, cBase *) static` — `src/gcValEntityConstant.cpp`
- `0x0032643c` 280B `gcValEntityConstant::GetType(void) const` — `src/gcValEntityConstant.cpp`
- `0x00326554` 140B `gcValEntityConstant::Write(cFile &) const` — `src/gcDoSwitchCase.cpp`
- `0x00326f14` 476B `gcValEntityConstant::~gcValEntityConstant(void)` — `src/gcValEntityConstant_dtor.cpp`

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

- session `ef146dc2`; src `src/gcValEntityConstant.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/ef146dc2/003268c4__agent_self_reported_failure__src_gcValEntityConstant.cpp`

Implemented gcValEntityConstant::GetText in canonical C++ method form using the matched GetText family pattern: entity virtual GetText at type+0x78, 256-byte local buffer, gcDesiredEnumerationEntryHelper::GetText(local,false), tagged-pointer validity checks, optional 0x36DCBC append plus type+0xD0 virtual GetText dispatch, and final cStrAppend(buf,0x36F1E8,local). Fixed the initial size mismatches by using local[0] = *local = '\0', binding the helper bool as a2, and using the matched bool conversion idiom for the first tagged-pointer validity test. Existing GetType/New/Write siblings remain matched.

Closest attempt is 16/288 bytes different after a 300s permuter run improved the baseline from 20 to 16 bytes by changing the second dispatch check to unsigned. Remaining mismatch is confined to the second tagged-pointer normalization and null dispatch block after cStrAppend(local,0x36DCBC): target keeps obj in a0, uses a1/a2 for flag/tag, then does move a1,a0 and beqzl a1 with the null-case lui in the delay slot. Current SNC output keeps obj in a0 but allocates the flag to s1 and emits beqz a0 plus addiu/lw through a1. Tried register bindings, owner liveness, sched=1, direct slot fn locals, memory barriers, and a narrow inline-asm normalization; register hints had no effect and inline asm disturbed scheduling/prologue too much. Root cause appears to be local register allocation/branch-likely selection in this small block.


## Disassembly

```asm
3268c4:	27bdfef0 	addiu	sp,sp,-272
3268c8:	afb10104 	sw	s1,260(sp)
3268cc:	00808825 	move	s1,a0
3268d0:	8e24000c 	lw	a0,12(s1)
3268d4:	afb00100 	sw	s0,256(sp)
3268d8:	00a08025 	move	s0,a1
3268dc:	24840078 	addiu	a0,a0,120
3268e0:	84850000 	lh	a1,0(a0)
3268e4:	26260008 	addiu	a2,s1,8
3268e8:	8c870004 	lw	a3,4(a0)
3268ec:	00c52021 	addu	a0,a2,a1
3268f0:	afbf0108 	sw	ra,264(sp)
3268f4:	00e0f809 	jalr	a3
3268f8:	02002825 	move	a1,s0
3268fc:	a3a00000 	sb	zero,0(sp)
326900:	a3a00000 	sb	zero,0(sp)
326904:	34060000 	li	a2,0x0
326908:	2624003c 	addiu	a0,s1,60
32690c:	0c043a56 	jal	0x10e958
326910:	03a02825 	move	a1,sp
326914:	8e240048 	lw	a0,72(s1)
326918:	34050000 	li	a1,0x0
32691c:	30860001 	andi	a2,a0,0x1
326920:	54c00001 	bnezl	a2,0x326928
326924:	34050001 	li	a1,0x1
326928:	10a00003 	beqz	a1,0x326938
32692c:	0004202b 	sltu	a0,zero,a0
326930:	10000003 	b	0x326940
326934:	34040000 	li	a0,0x0
326938:	308400ff 	andi	a0,a0,0xff
32693c:	0004202b 	sltu	a0,zero,a0
326940:	1080001f 	beqz	a0,0x3269c0
326944:	3c050037 	lui	a1,0x37
326948:	3c050037 	lui	a1,0x37
32694c:	03a02025 	move	a0,sp
326950:	0c000470 	jal	0x11c0
326954:	24a5dcbc 	addiu	a1,a1,-9028
326958:	8e240048 	lw	a0,72(s1)
32695c:	34050000 	li	a1,0x0
326960:	30860001 	andi	a2,a0,0x1
326964:	54c00001 	bnezl	a2,0x32696c
326968:	34050001 	li	a1,0x1
32696c:	10a00003 	beqz	a1,0x32697c
326970:	00000000 	nop
326974:	10000001 	b	0x32697c
326978:	34040000 	li	a0,0x0
32697c:	00802825 	move	a1,a0
326980:	50a0000b 	beqzl	a1,0x3269b0
326984:	3c050037 	lui	a1,0x37
326988:	8ca60004 	lw	a2,4(a1)
32698c:	03a02825 	move	a1,sp
326990:	24c600d0 	addiu	a2,a2,208
326994:	84c70000 	lh	a3,0(a2)
326998:	8cc60004 	lw	a2,4(a2)
32699c:	00c0f809 	jalr	a2
3269a0:	00872021 	addu	a0,a0,a3
3269a4:	10000006 	b	0x3269c0
3269a8:	3c050037 	lui	a1,0x37
3269ac:	3c050037 	lui	a1,0x37
3269b0:	03a02025 	move	a0,sp
3269b4:	0c0004da 	jal	0x1368
3269b8:	24a5db24 	addiu	a1,a1,-9436
3269bc:	3c050037 	lui	a1,0x37
3269c0:	02002025 	move	a0,s0
3269c4:	03a03025 	move	a2,sp
3269c8:	0c000470 	jal	0x11c0
3269cc:	24a5f1e8 	addiu	a1,a1,-3608
3269d0:	8fb00100 	lw	s0,256(sp)
3269d4:	8fb10104 	lw	s1,260(sp)
3269d8:	8fbf0108 	lw	ra,264(sp)
3269dc:	03e00008 	jr	ra
3269e0:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcValEntityConstant::GetText(char *) const
// Address: 0x003268c4, Size: 288B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, s32); /* extern */
? cStrCat_charptr_constcharptr(void *, ?, s32);     /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcValEntityConstant__GetText_charptrconst(void *arg0, void *arg1) {
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
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F1E8, (s32) sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
