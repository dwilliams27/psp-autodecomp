# Battle Packet: `gcValEntityHasConstants::GetText(char *) const`

## Target

- Address: `0x0032fd74`
- Size: `232` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValEntityHasConstants`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `78`
- priority: `22`
- reason: `has failure_notes; rich notes; near miss 47B; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `47`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `47`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`

## Placement

- Canonical source: `src/gcValEntityHasConstants.cpp`
- Header(s): `include/gcValEntityHasConstants.h`
- Allowed source(s): `src/gcValEntityHasConstants.cpp`
- Split-TU prefix: `src/gcValEntityHasConstants_*.cpp`

## Class Header: `include/gcValEntityHasConstants.h`

Not found.

## Matched Same-Class Neighbors

- `0x0032f92c` 80B `gcValEntityHasConstants::AssignCopy(const cBase *)` — `src/gcValEntityHasConstants.cpp`
- `0x0032f97c` 260B `gcValEntityHasConstants::New(cMemPool *, cBase *) static` — `src/gcValEntityHasConstants.cpp`
- `0x0032fa80` 280B `gcValEntityHasConstants::GetType(void) const` — `src/gcValEntityHasConstants.cpp`
- `0x0032fb98` 120B `gcValEntityHasConstants::Write(cFile &) const` — `src/gcValEntityHasConstants.cpp`
- `0x0032fd28` 76B `gcValEntityHasConstants::Evaluate(void) const` — `src/gcValEntityHasConstants.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValEntityHasVariables::GetText(char *) const`

- Address: `0x00331b20`
- Size: `232` bytes
- Source: `src/gcValEntityHasVariables.cpp`

```cpp
// ── gcValEntityHasVariables::GetText(char *) const @ 0x00331b20 ──
void gcValEntityHasVariables::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);

    register const char *fmt __asm__("$4") = (const char *)0x36F2C8;
    int h = *(const int *)((const char *)this + 52);
    HandleObject *obj;
    if (h == 0) {
        obj = 0;
    } else {
        HandleObject *cand = (HandleObject *)D_00038890[h & 0xFFFF];
        obj = 0;
        if (cand != 0) {
            if (cand->handle == h) {
                obj = cand;
            }
        }
    }

    const char *text;
    if (obj != 0) {
        unsigned int empty = (obj->nameLen == 0);
        empty &= 0xFF;
        if (empty) {
            text = (const char *)0x36DAB8;
            goto object_done;
        }
        text = (const char *)obj + 8;
object_done:
        __asm__ volatile("" : : "r"(text));
        goto append;
    } else if (h != 0) {
        text = (const char *)0x36DAC4;
    } else {
        text = (const char *)0x36DACC;
    }
append:
    cStrAppend(buf, fmt, text);
}
```

### Exemplar 2: `gcValStatsTracking::GetText(char *) const`

- Address: `0x0035d21c`
- Size: `220` bytes
- Source: `src/gcValStatsTracking.cpp`

```cpp
void gcValStatsTracking::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36E2EC, (const char *)0x36DAF0);

    int val = *(const int *)((const char *)this + 0x10);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }

    if (flag != 0) {
        val = 0;
    } else {
        val = (val != 0);
        val &= 0xFF;
        val = (val != 0);
    }

    if (val != 0) {
        cStrCat(buf, (const char *)0x36E300);

        register int val2 __asm__("$4") = *(const int *)((const char *)this + 0x10);
        register int check __asm__("$5") = 0;
        register int bit2 __asm__("$6") = val2 & 1;
        if (bit2) {
            check = 1;
        }

        char *typeInfo;
        if (check != 0) {
            check = 0;
            typeInfo = *(char **)(check + 4);
        } else {
            check = val2;
            typeInfo = *(char **)(check + 4);
        }
        TextEntry *entry = (TextEntry *)(typeInfo + 0x40);
        short off = entry->offset;
        void (*fn)(void *, char *) = entry->fn;
        fn((char *)check + off, buf);

        cStrCat(buf, (const char *)0x36E2E8);
    }
}
```

### Exemplar 3: `gcValTableInfo::GetText(char *) const`

- Address: `0x003609b4`
- Size: `244` bytes
- Source: `src/gcValTableInfo.cpp`

```cpp
void gcValTableInfo::GetText(char *buf) const {
    char *typeInfo = *(char **)((const char *)this + 0x0C);
    DesiredTextSlot *textSlot = (DesiredTextSlot *)(typeInfo + 0x78);
    char *base = (char *)this + 0x08;
    textSlot->mFn(base + textSlot->mOffset, buf);

    cStrAppend(buf, (const char *)0x36DCB8, (const char *)0x36DAF0);

    if (*(int *)((const char *)this + 0x1C) == 2) {
        cStrCat(buf, (const char *)0x36E8D0);

        int val = *(int *)((const char *)this + 0x20);
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
            char *entryType = *(char **)(check + 4);
            DesiredTextSlot *entry = (DesiredTextSlot *)(entryType + 0xD0);
            entry->mFn((char *)check + entry->mOffset, buf);
        } else {
            cStrCat(buf, (const char *)0x36DB24);
        }

        cStrCat(buf, (const char *)0x36E90C);
    }
}
```

### Exemplar 4: `gcValUITexCoord::GetText(char *) const`

- Address: `0x00366f7c`
- Size: `244` bytes
- Source: `src/gcValUITexCoord.cpp`

```cpp
void gcValUITexCoord::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->GetText(local);
    cStrCat(buf, local);

    const char *sep = (const char *)0x36DADC;
    cStrCat(buf, sep);
    cStrCat(buf, gcUIControl::GetSpriteText((gcUIControl::gcUISprite)mField14));
    cStrCat(buf, sep);

    const char *text;
    switch (mField18) {
    case 0:
        text = (const char *)0x36F7D4;
        break;
    case 1:
        text = (const char *)0x36F7E0;
        break;
    case 2:
        text = (const char *)0x36F7EC;
        break;
    case 3:
        text = (const char *)0x36F7F8;
        break;
    default:
        text = (const char *)0x36DAF0;
        break;
    }
    cStrCat(buf, text);
}
```


## Prior Failure Notes

### Note 1

- session `da2e1d35`

Implemented a real C++ GetText body matching the reflective gcDesiredEntity text dispatch, handle-table lookup through D_00038890, resolved-object name selection, and unresolved/zero handle fallback strings. The best C shape compiles to 228B versus the 232B target. The remaining difference is localized to the resolved-object empty-name path: the target emits `lui a2,0x37; b object_done; addiu a2,a2,-9544` before the final branch to cStrAppend, while this compiler folds it to straight-line `lui; addiu` followed by the object_done branch. Everything around the lookup, boolean `sltiu/andi`, no-object fallback, and final cStrAppend argument setup matches in shape.

Tried structured if/else, inverted null checks, explicit candidate-validation nesting, a named D_00038890 global instead of raw 0x38890, object/no-object goto layout, barriers at the object join, explicit empty-name goto, ternary selection, and a sched=1 compile check. The ternary reaches 232B but changes the boolean branch into a beqzl/t0 sequence and causes a 47-byte mismatch. `tools/permuter.py` could not run on this target because it failed to locate the size-mismatched function in the compiled .o. I did not use inline MIPS assembly to force the branch-to-next because this repo's matching rules require real C/C++ rather than assembly patches.


## Disassembly

```asm
32fd74:	27bdfff0 	addiu	sp,sp,-16
32fd78:	afb10004 	sw	s1,4(sp)
32fd7c:	00808825 	move	s1,a0
32fd80:	8e24000c 	lw	a0,12(s1)
32fd84:	afb00000 	sw	s0,0(sp)
32fd88:	00a08025 	move	s0,a1
32fd8c:	24840078 	addiu	a0,a0,120
32fd90:	84850000 	lh	a1,0(a0)
32fd94:	26260008 	addiu	a2,s1,8
32fd98:	8c870004 	lw	a3,4(a0)
32fd9c:	00c52021 	addu	a0,a2,a1
32fda0:	afbf0008 	sw	ra,8(sp)
32fda4:	00e0f809 	jalr	a3
32fda8:	02002825 	move	a1,s0
32fdac:	8e250034 	lw	a1,52(s1)
32fdb0:	3c040037 	lui	a0,0x37
32fdb4:	14a00003 	bnez	a1,0x32fdc4
32fdb8:	2484f2a0 	addiu	a0,a0,-3424
32fdbc:	1000000d 	b	0x32fdf4
32fdc0:	34060000 	li	a2,0x0
32fdc4:	30a6ffff 	andi	a2,a1,0xffff
32fdc8:	3c070004 	lui	a3,0x4
32fdcc:	00063080 	sll	a2,a2,0x2
32fdd0:	24e78890 	addiu	a3,a3,-30576
32fdd4:	00c73021 	addu	a2,a2,a3
32fdd8:	8cc70000 	lw	a3,0(a2)
32fddc:	10e00005 	beqz	a3,0x32fdf4
32fde0:	34060000 	li	a2,0x0
32fde4:	8ce80030 	lw	t0,48(a3)
32fde8:	15050002 	bne	t0,a1,0x32fdf4
32fdec:	00000000 	nop
32fdf0:	00e03025 	move	a2,a3
32fdf4:	10c0000b 	beqz	a2,0x32fe24
32fdf8:	00000000 	nop
32fdfc:	84c5001c 	lh	a1,28(a2)
32fe00:	2ca50001 	sltiu	a1,a1,1
32fe04:	30a500ff 	andi	a1,a1,0xff
32fe08:	10a00004 	beqz	a1,0x32fe1c
32fe0c:	24c60008 	addiu	a2,a2,8
32fe10:	3c060037 	lui	a2,0x37
32fe14:	10000001 	b	0x32fe1c
32fe18:	24c6dab8 	addiu	a2,a2,-9544
32fe1c:	10000008 	b	0x32fe40
32fe20:	00802825 	move	a1,a0
32fe24:	10a00004 	beqz	a1,0x32fe38
32fe28:	3c060037 	lui	a2,0x37
32fe2c:	3c060037 	lui	a2,0x37
32fe30:	10000002 	b	0x32fe3c
32fe34:	24c6dac4 	addiu	a2,a2,-9532
32fe38:	24c6dacc 	addiu	a2,a2,-9524
32fe3c:	00802825 	move	a1,a0
32fe40:	0c000470 	jal	0x11c0
32fe44:	02002025 	move	a0,s0
32fe48:	8fb00000 	lw	s0,0(sp)
32fe4c:	8fb10004 	lw	s1,4(sp)
32fe50:	8fbf0008 	lw	ra,8(sp)
32fe54:	03e00008 	jr	ra
32fe58:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcValEntityHasConstants::GetText(char *) const
// Address: 0x0032fd74, Size: 232B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, void *); /* extern */

void gcValEntityHasConstants__GetText_charptrconst(void *arg0, ? arg1) {
    ? (*temp_a3)(void *, ?, void *, ?);
    s32 temp_a1;
    void *temp_a0;
    void *temp_a2;
    void *temp_a3_2;
    void *var_a2;
    void *var_a2_2;

    temp_a0 = arg0->unkC;
    temp_a2 = arg0 + 8;
    temp_a3 = (temp_a0 + 0x78)->unk4;
    temp_a3(temp_a2 + temp_a0->unk78, arg1, temp_a2, temp_a3);
    temp_a1 = arg0->unk34;
    if (temp_a1 == 0) {
        var_a2 = NULL;
    } else {
        temp_a3_2 = ((temp_a1 & 0xFFFF) * 4)->unk38890;
        var_a2 = NULL;
        if ((temp_a3_2 != NULL) && (temp_a3_2->unk30 == temp_a1)) {
            var_a2 = temp_a3_2;
        }
    }
    if (var_a2 != NULL) {
        var_a2_2 = var_a2 + 8;
        if ((var_a2->unk1C == 0) & 0xFF) {
            var_a2_2 = (void *)0x36DAB8;
        }
    } else if (temp_a1 != 0) {
        var_a2_2 = (void *)0x36DAC4;
    } else {
        var_a2_2 = (void *)0x36DACC;
    }
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F2A0, var_a2_2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
