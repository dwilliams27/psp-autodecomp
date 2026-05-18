# Battle Packet: `gcValEntityHasTimeSince::GetText(char *) const`

## Target

- Address: `0x00330eac`
- Size: `288` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValEntityHasTimeSince`
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

- Canonical source: `src/gcValEntityHasTimeSince.cpp`
- Header(s): `include/gcValEntityHasTimeSince.h`
- Allowed source(s): `src/gcValEntityHasTimeSince.cpp`
- Split-TU prefix: `src/gcValEntityHasTimeSince_*.cpp`

## Class Header: `include/gcValEntityHasTimeSince.h`

Not found.

## Matched Same-Class Neighbors

- `0x003308f0` 292B `gcValEntityHasTimeSince::New(cMemPool *, cBase *) static` — `src/gcValEntityHasTimeSince.cpp`
- `0x00330a14` 280B `gcValEntityHasTimeSince::GetType(void) const` — `src/gcValEntityHasTimeSince.cpp`
- `0x00330b2c` 140B `gcValEntityHasTimeSince::Write(cFile &) const` — `src/gcValEntityHasTimeSince.cpp`
- `0x003314fc` 476B `gcValEntityHasTimeSince::~gcValEntityHasTimeSince(void)` — `src/gcValEntityHasTimeSince_dtor.cpp`

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

- session `aced64d7`; src `src/gcValEntityHasTimeSince.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/aced64d7/00330eac__agent_self_reported_failure__src_gcValEntityHasTimeSince.cpp`

Implemented gcValEntityHasTimeSince::GetText in C++ using the matched GetText family pattern: entity text virtual slot at type+0x78, local 256-byte buffer initialized with the duplicated zero store, gcDesiredEnumerationEntryHelper::GetText(local, true), tagged-pointer validity test for field +0x48, optional append of 0x36DCBC plus target object's type+0xD0 text slot, fallback cStrCat(local, 0x36DB24), and final cStrAppend(buf, 0x36F2B4, local). The first 0x94 bytes and final formatting path can be made structurally exact; the closest saved source is 16/288 bytes off.

The remaining diff is isolated to the second tagged-pointer block after cStrAppend(local, 0x36DCBC). The original keeps the loaded pointer in a0, uses a1 for the tag flag, a2 for the tag bit and later the type slot, emits move a1,a0 followed by beqzl a1 for the null fallback, then loads the type with lw a2,4(a1). The closest C source keeps the pointer in a0 but SNC allocates the flag through s1/a1 and emits beqz a0 plus an addiu/lw sequence for *(check+4). Tried direct if/else, goto form, bool/register bindings for a0/a1/a2, lifetime barriers, one-instruction inline load experiment, and a 300s permuter run (6936 generated, 3485 compiled), which improved 20 bytes to 16 but did not match. This appears to be local register-allocation/branch-likely drift in this block rather than a control-flow misunderstanding.


## Disassembly

```asm
330eac:	27bdfef0 	addiu	sp,sp,-272
330eb0:	afb10104 	sw	s1,260(sp)
330eb4:	00808825 	move	s1,a0
330eb8:	8e24000c 	lw	a0,12(s1)
330ebc:	afb00100 	sw	s0,256(sp)
330ec0:	00a08025 	move	s0,a1
330ec4:	24840078 	addiu	a0,a0,120
330ec8:	84850000 	lh	a1,0(a0)
330ecc:	26260008 	addiu	a2,s1,8
330ed0:	8c870004 	lw	a3,4(a0)
330ed4:	00c52021 	addu	a0,a2,a1
330ed8:	afbf0108 	sw	ra,264(sp)
330edc:	00e0f809 	jalr	a3
330ee0:	02002825 	move	a1,s0
330ee4:	a3a00000 	sb	zero,0(sp)
330ee8:	a3a00000 	sb	zero,0(sp)
330eec:	34060001 	li	a2,0x1
330ef0:	2624003c 	addiu	a0,s1,60
330ef4:	0c043a56 	jal	0x10e958
330ef8:	03a02825 	move	a1,sp
330efc:	8e240048 	lw	a0,72(s1)
330f00:	34050000 	li	a1,0x0
330f04:	30860001 	andi	a2,a0,0x1
330f08:	54c00001 	bnezl	a2,0x330f10
330f0c:	34050001 	li	a1,0x1
330f10:	10a00003 	beqz	a1,0x330f20
330f14:	0004202b 	sltu	a0,zero,a0
330f18:	10000003 	b	0x330f28
330f1c:	34040000 	li	a0,0x0
330f20:	308400ff 	andi	a0,a0,0xff
330f24:	0004202b 	sltu	a0,zero,a0
330f28:	1080001f 	beqz	a0,0x330fa8
330f2c:	3c050037 	lui	a1,0x37
330f30:	3c050037 	lui	a1,0x37
330f34:	03a02025 	move	a0,sp
330f38:	0c000470 	jal	0x11c0
330f3c:	24a5dcbc 	addiu	a1,a1,-9028
330f40:	8e240048 	lw	a0,72(s1)
330f44:	34050000 	li	a1,0x0
330f48:	30860001 	andi	a2,a0,0x1
330f4c:	54c00001 	bnezl	a2,0x330f54
330f50:	34050001 	li	a1,0x1
330f54:	10a00003 	beqz	a1,0x330f64
330f58:	00000000 	nop
330f5c:	10000001 	b	0x330f64
330f60:	34040000 	li	a0,0x0
330f64:	00802825 	move	a1,a0
330f68:	50a0000b 	beqzl	a1,0x330f98
330f6c:	3c050037 	lui	a1,0x37
330f70:	8ca60004 	lw	a2,4(a1)
330f74:	03a02825 	move	a1,sp
330f78:	24c600d0 	addiu	a2,a2,208
330f7c:	84c70000 	lh	a3,0(a2)
330f80:	8cc60004 	lw	a2,4(a2)
330f84:	00c0f809 	jalr	a2
330f88:	00872021 	addu	a0,a0,a3
330f8c:	10000006 	b	0x330fa8
330f90:	3c050037 	lui	a1,0x37
330f94:	3c050037 	lui	a1,0x37
330f98:	03a02025 	move	a0,sp
330f9c:	0c0004da 	jal	0x1368
330fa0:	24a5db24 	addiu	a1,a1,-9436
330fa4:	3c050037 	lui	a1,0x37
330fa8:	02002025 	move	a0,s0
330fac:	03a03025 	move	a2,sp
330fb0:	0c000470 	jal	0x11c0
330fb4:	24a5f2b4 	addiu	a1,a1,-3404
330fb8:	8fb00100 	lw	s0,256(sp)
330fbc:	8fb10104 	lw	s1,260(sp)
330fc0:	8fbf0108 	lw	ra,264(sp)
330fc4:	03e00008 	jr	ra
330fc8:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcValEntityHasTimeSince::GetText(char *) const
// Address: 0x00330eac, Size: 288B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, s32); /* extern */
? cStrCat_charptr_constcharptr(void *, ?, s32);     /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcValEntityHasTimeSince__GetText_charptrconst(void *arg0, void *arg1) {
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
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x3C, sp, 1);
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
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F2B4, (s32) sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
