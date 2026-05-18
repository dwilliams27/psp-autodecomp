# Battle Packet: `gcValEntityTimeSince::GetText(char *) const`

## Target

- Address: `0x0033f480`
- Size: `288` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValEntityTimeSince`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `tagged_pointer_shape`
- Near miss: `16`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Placement

- Canonical source: `src/gcValEntityTimeSince.cpp`
- Header(s): `include/gcValEntityTimeSince.h`
- Allowed source(s): `src/gcValEntityTimeSince.cpp`, `src/gcValEntityHasTimeSince.cpp`
- Split-TU prefix: `src/gcValEntityTimeSince_*.cpp`

## Class Header: `include/gcValEntityTimeSince.h`

Not found.

## Matched Same-Class Neighbors

- `0x0033ebf4` 292B `gcValEntityTimeSince::New(cMemPool *, cBase *) static` — `src/gcValEntityTimeSince.cpp`
- `0x0033ed18` 340B `gcValEntityTimeSince::GetType(void) const` — `src/gcValEntityTimeSince.cpp`
- `0x0033ee6c` 140B `gcValEntityTimeSince::Write(cFile &) const` — `src/gcSubGeomController.cpp`
- `0x0033fad0` 476B `gcValEntityTimeSince::~gcValEntityTimeSince(void)` — `src/gcValEntityTimeSince_dtor.cpp`

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

- session `aced64d7`; src `src/gcValEntityHasTimeSince.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/aced64d7/0033f480__agent_self_reported_failure__src_gcValEntityHasTimeSince.cpp`

Implemented gcValEntityTimeSince::GetText in the same source file as requested. It is identical to gcValEntityHasTimeSince::GetText except gcDesiredEnumerationEntryHelper::GetText(local, false) and final format string 0x36F3DC. The closest source compiles to the correct 288-byte size and is 16/288 bytes off after mirroring the best unsigned-check variant found by the permuter on the HasTimeSince method.

The byte diff is the same register-shape issue as 0x00330eac in the second tagged-pointer text block: original uses a0/a1/a2 and beqzl a1 for the null object fallback, while the compiled source routes the tag flag through s1/a1 and emits a non-likely beqz on a0 with a slightly different type-info load sequence. Manual restructurings that fixed the earlier duplicated sltu/lui scheduling did not move this remaining allocation into the original shape; the first method's 300s permuter run found only a partial improvement that applies here too.


## Disassembly

```asm
33f480:	27bdfef0 	addiu	sp,sp,-272
33f484:	afb10104 	sw	s1,260(sp)
33f488:	00808825 	move	s1,a0
33f48c:	8e24000c 	lw	a0,12(s1)
33f490:	afb00100 	sw	s0,256(sp)
33f494:	00a08025 	move	s0,a1
33f498:	24840078 	addiu	a0,a0,120
33f49c:	84850000 	lh	a1,0(a0)
33f4a0:	26260008 	addiu	a2,s1,8
33f4a4:	8c870004 	lw	a3,4(a0)
33f4a8:	00c52021 	addu	a0,a2,a1
33f4ac:	afbf0108 	sw	ra,264(sp)
33f4b0:	00e0f809 	jalr	a3
33f4b4:	02002825 	move	a1,s0
33f4b8:	a3a00000 	sb	zero,0(sp)
33f4bc:	a3a00000 	sb	zero,0(sp)
33f4c0:	34060000 	li	a2,0x0
33f4c4:	2624003c 	addiu	a0,s1,60
33f4c8:	0c043a56 	jal	0x10e958
33f4cc:	03a02825 	move	a1,sp
33f4d0:	8e240048 	lw	a0,72(s1)
33f4d4:	34050000 	li	a1,0x0
33f4d8:	30860001 	andi	a2,a0,0x1
33f4dc:	54c00001 	bnezl	a2,0x33f4e4
33f4e0:	34050001 	li	a1,0x1
33f4e4:	10a00003 	beqz	a1,0x33f4f4
33f4e8:	0004202b 	sltu	a0,zero,a0
33f4ec:	10000003 	b	0x33f4fc
33f4f0:	34040000 	li	a0,0x0
33f4f4:	308400ff 	andi	a0,a0,0xff
33f4f8:	0004202b 	sltu	a0,zero,a0
33f4fc:	1080001f 	beqz	a0,0x33f57c
33f500:	3c050037 	lui	a1,0x37
33f504:	3c050037 	lui	a1,0x37
33f508:	03a02025 	move	a0,sp
33f50c:	0c000470 	jal	0x11c0
33f510:	24a5dcbc 	addiu	a1,a1,-9028
33f514:	8e240048 	lw	a0,72(s1)
33f518:	34050000 	li	a1,0x0
33f51c:	30860001 	andi	a2,a0,0x1
33f520:	54c00001 	bnezl	a2,0x33f528
33f524:	34050001 	li	a1,0x1
33f528:	10a00003 	beqz	a1,0x33f538
33f52c:	00000000 	nop
33f530:	10000001 	b	0x33f538
33f534:	34040000 	li	a0,0x0
33f538:	00802825 	move	a1,a0
33f53c:	50a0000b 	beqzl	a1,0x33f56c
33f540:	3c050037 	lui	a1,0x37
33f544:	8ca60004 	lw	a2,4(a1)
33f548:	03a02825 	move	a1,sp
33f54c:	24c600d0 	addiu	a2,a2,208
33f550:	84c70000 	lh	a3,0(a2)
33f554:	8cc60004 	lw	a2,4(a2)
33f558:	00c0f809 	jalr	a2
33f55c:	00872021 	addu	a0,a0,a3
33f560:	10000006 	b	0x33f57c
33f564:	3c050037 	lui	a1,0x37
33f568:	3c050037 	lui	a1,0x37
33f56c:	03a02025 	move	a0,sp
33f570:	0c0004da 	jal	0x1368
33f574:	24a5db24 	addiu	a1,a1,-9436
33f578:	3c050037 	lui	a1,0x37
33f57c:	02002025 	move	a0,s0
33f580:	03a03025 	move	a2,sp
33f584:	0c000470 	jal	0x11c0
33f588:	24a5f3dc 	addiu	a1,a1,-3108
33f58c:	8fb00100 	lw	s0,256(sp)
33f590:	8fb10104 	lw	s1,260(sp)
33f594:	8fbf0108 	lw	ra,264(sp)
33f598:	03e00008 	jr	ra
33f59c:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcValEntityTimeSince::GetText(char *) const
// Address: 0x0033f480, Size: 288B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, s32); /* extern */
? cStrCat_charptr_constcharptr(void *, ?, s32);     /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcValEntityTimeSince__GetText_charptrconst(void *arg0, void *arg1) {
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
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F3DC, (s32) sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
