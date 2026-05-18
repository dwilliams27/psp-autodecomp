# Battle Packet: `gcDoSetEventEnumParam::GetText(char *) const`

## Target

- Address: `0x002fc87c`
- Size: `244` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoSetEventEnumParam`
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

- Canonical source: `src/gcDoSetEventEnumParam.cpp`
- Header(s): `include/gcDoSetEventEnumParam.h`
- Allowed source(s): `src/gcDoSetEventEnumParam.cpp`
- Split-TU prefix: `src/gcDoSetEventEnumParam_*.cpp`

## Class Header: `include/gcDoSetEventEnumParam.h`

Not found.

## Matched Same-Class Neighbors

- `0x002fc304` 196B `gcDoSetEventEnumParam::New(cMemPool *, cBase *) static` — `src/gcDoSetEventEnumParam.cpp`
- `0x002fc3c8` 280B `gcDoSetEventEnumParam::GetType(void) const` — `src/gcDoSetEventEnumParam.cpp`
- `0x002fc4e0` 120B `gcDoSetEventEnumParam::Write(cFile &) const` — `src/gcDoSetEventEnumParam.cpp`
- `0x002fc558` 300B `gcDoSetEventEnumParam::Read(cFile &, cMemPool *)` — `src/gcDoEntitySetScale.cpp`
- `0x002fcb64` 248B `gcDoSetEventEnumParam::~gcDoSetEventEnumParam(void)` — `src/gcDoSetEventEnumParam.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoEntityAssignPath::GetText(char *) const`

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

### Exemplar 2: `gcDoUISetTextSprite::GetText(char *) const`

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

### Exemplar 3: `gcDoStatsTrackingOp::GetText(char *) const`

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

### Exemplar 4: `gcDoEntitySetGeomCurveTarget::GetText(char *) const`

- Address: `0x002c7c10`
- Size: `232` bytes
- Source: `src/gcDoEntitySetGeomCurveTarget.cpp`

```cpp
void gcDoEntitySetGeomCurveTarget::GetText(char *buf) const {
    char text[256];

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    GetTextRec *rec0 = (GetTextRec *)(typeInfo0 + 0x78);
    char *base0 = (char *)this + 0x0C;
    rec0->fn(base0 + rec0->offset, buf);

    int which = *(int *)((const char *)this + 0x3C);
    text[0] = '\0';

    if (which <= 0) {
        if (which >= 0) {
            char *typeInfo2 = *(char **)((const char *)this + 0x58);
            GetTextRec *rec2 = (GetTextRec *)(typeInfo2 + 0x78);
            char *base2 = (char *)this + 0x54;
            rec2->fn(base2 + rec2->offset, text);
            cStrAppend(buf, (const char *)0x36E9E4,
                       *(int *)((const char *)this + 0x38), text,
                       (char *)this + 0x80);
        }
    } else if (which < 2) {
        char *typeInfo1 = *(char **)((const char *)this + 0x44);
        GetTextRec *rec1 = (GetTextRec *)(typeInfo1 + 0x78);
        char *base1 = (char *)this + 0x40;
        rec1->fn(base1 + rec1->offset, text);
        cStrAppend(buf, (const char *)0x36EA10,
                   *(int *)((const char *)this + 0x38), text);
    }
}
```


## Prior Failure Notes

### Note 1

- session `e8a49ca9`; src `src/gcDoSetEventEnumParam.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/e8a49ca9/002fc87c__agent_self_reported_failure__src_gcDoSetEventEnumParam.cpp`

Closest attempt is a 16/244 byte mismatch after manual restructuring and a 5-minute permuter run (baseline 20 bytes, best saved at 16). The first cStrAppend, helper GetText call scheduling, first tagged-pointer boolean check, size, and outer control flow match. The remaining mismatch is localized to the second tagged-pointer dispatch after appending 0x36DCBC: expected keeps this in s1, reloads the tagged value into a0, uses a1/a2 for flag/bit, then moves a0 to a1 and emits beqzl for the null check before the +0xD0 text dispatch. The closest source instead reloads the tagged value into s1 and uses a0/a1 around the flag and null check; all remaining differences are register encodings plus the null-check branch form in that block.

Tried normal m2c-style C, memory-barrier and non-clobber asm else branches, explicit bit temporaries, register bindings for a0/a1/a2, keeping self live through empty asm, inverting the inner null branch, and running tools/permuter.py for 300s with --save-best. The likely root cause is SNC register-allocation/context drift for this small second dispatch block; the structurally identical gcValTableInfo variant matches because the original also uses s1 for the tagged value there, but this function’s original does not.


## Disassembly

```asm
2fc87c:	27bdfff0 	addiu	sp,sp,-16
2fc880:	afb00000 	sw	s0,0(sp)
2fc884:	00a08025 	move	s0,a1
2fc888:	afb10004 	sw	s1,4(sp)
2fc88c:	00808825 	move	s1,a0
2fc890:	3c050037 	lui	a1,0x37
2fc894:	3c060037 	lui	a2,0x37
2fc898:	02002025 	move	a0,s0
2fc89c:	24a5e678 	addiu	a1,a1,-6536
2fc8a0:	afbf0008 	sw	ra,8(sp)
2fc8a4:	0c000470 	jal	0x11c0
2fc8a8:	24c6daf0 	addiu	a2,a2,-9488
2fc8ac:	34060000 	li	a2,0x0
2fc8b0:	26240014 	addiu	a0,s1,20
2fc8b4:	0c043a56 	jal	0x10e958
2fc8b8:	02002825 	move	a1,s0
2fc8bc:	8e240020 	lw	a0,32(s1)
2fc8c0:	34050000 	li	a1,0x0
2fc8c4:	30860001 	andi	a2,a0,0x1
2fc8c8:	54c00001 	bnezl	a2,0x2fc8d0
2fc8cc:	34050001 	li	a1,0x1
2fc8d0:	10a00003 	beqz	a1,0x2fc8e0
2fc8d4:	0004202b 	sltu	a0,zero,a0
2fc8d8:	10000003 	b	0x2fc8e8
2fc8dc:	34040000 	li	a0,0x0
2fc8e0:	308400ff 	andi	a0,a0,0xff
2fc8e4:	0004202b 	sltu	a0,zero,a0
2fc8e8:	1080001c 	beqz	a0,0x2fc95c
2fc8ec:	3c050037 	lui	a1,0x37
2fc8f0:	02002025 	move	a0,s0
2fc8f4:	0c000470 	jal	0x11c0
2fc8f8:	24a5dcbc 	addiu	a1,a1,-9028
2fc8fc:	8e240020 	lw	a0,32(s1)
2fc900:	34050000 	li	a1,0x0
2fc904:	30860001 	andi	a2,a0,0x1
2fc908:	54c00001 	bnezl	a2,0x2fc910
2fc90c:	34050001 	li	a1,0x1
2fc910:	10a00003 	beqz	a1,0x2fc920
2fc914:	00000000 	nop
2fc918:	10000001 	b	0x2fc920
2fc91c:	34040000 	li	a0,0x0
2fc920:	00802825 	move	a1,a0
2fc924:	50a0000a 	beqzl	a1,0x2fc950
2fc928:	3c050037 	lui	a1,0x37
2fc92c:	8ca60004 	lw	a2,4(a1)
2fc930:	02002825 	move	a1,s0
2fc934:	24c600d0 	addiu	a2,a2,208
2fc938:	84c70000 	lh	a3,0(a2)
2fc93c:	8cc60004 	lw	a2,4(a2)
2fc940:	00c0f809 	jalr	a2
2fc944:	00872021 	addu	a0,a0,a3
2fc948:	10000004 	b	0x2fc95c
2fc94c:	00000000 	nop
2fc950:	02002025 	move	a0,s0
2fc954:	0c0004da 	jal	0x1368
2fc958:	24a5db24 	addiu	a1,a1,-9436
2fc95c:	8fb00000 	lw	s0,0(sp)
2fc960:	8fb10004 	lw	s1,4(sp)
2fc964:	8fbf0008 	lw	ra,8(sp)
2fc968:	03e00008 	jr	ra
2fc96c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcDoSetEventEnumParam::GetText(char *) const
// Address: 0x002fc87c, Size: 244B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, s32);   /* extern */
? cStrCat_charptr_constcharptr(?, ?, s32);          /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(s32, ?, ?); /* extern */

void gcDoSetEventEnumParam__GetText_charptrconst(void *arg0, ? arg1) {
    ? (*temp_a2_4)(s32, ?, ?, s16);
    s16 temp_a3;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    void *temp_a2_3;

    cStrAppend_charptr_constcharptr_...(arg1, 0x36E678, 0x36DAF0);
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x14, arg1, 0);
    temp_a0 = arg0->unk20;
    var_a1 = 0;
    temp_a2 = temp_a0 & 1;
    if (temp_a2 != 0) {
        var_a1 = 1;
    }
    if (var_a1 != 0) {
        var_a0 = 0;
    } else {
        var_a0 = ((temp_a0 != 0) & 0xFF) != 0;
    }
    if (var_a0 != 0) {
        cStrAppend_charptr_constcharptr_...(arg1, 0x36DCBC, temp_a2);
        var_a0_2 = arg0->unk20;
        var_a1_2 = 0;
        temp_a2_2 = var_a0_2 & 1;
        if (temp_a2_2 != 0) {
            var_a1_2 = 1;
        }
        if (var_a1_2 != 0) {
            var_a0_2 = 0;
        }
        if (var_a0_2 == 0) {
            cStrCat_charptr_constcharptr(arg1, 0x36DB24, temp_a2_2);
        } else {
            temp_a2_3 = var_a0_2->unk4;
            temp_a3 = temp_a2_3->unkD0;
            temp_a2_4 = (temp_a2_3 + 0xD0)->unk4;
            temp_a2_4(var_a0_2 + temp_a3, arg1, temp_a2_4, temp_a3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
