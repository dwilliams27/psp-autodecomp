# Battle Packet: `gcValStringCompare::GetText(char *) const`

## Target

- Address: `0x0035e0cc`
- Size: `384` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValStringCompare`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `13`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001428` `untried` 80B `cStrCopy(char *, const char *)`

## Placement

- Canonical source: `src/gcValStringCompare.cpp`
- Header(s): `include/gcValStringCompare.h`
- Allowed source(s): `src/gcValStringCompare.cpp`
- Split-TU prefix: `src/gcValStringCompare_*.cpp`

## Class Header: `include/gcValStringCompare.h`

Not found.

## Matched Same-Class Neighbors

- `0x0035d92c` 420B `gcValStringCompare::AssignCopy(const cBase *)` — `src/gcValStringCompare.cpp`
- `0x0035dad0` 156B `gcValStringCompare::New(cMemPool *, cBase *) static` — `src/gcValStringCompare.cpp`
- `0x0035db6c` 280B `gcValStringCompare::GetType(void) const` — `src/gcValStringCompare.cpp`
- `0x0035dc84` 196B `gcValStringCompare::Write(cFile &) const` — `src/gcValStringCompare.cpp`
- `0x0035dd48` 532B `gcValStringCompare::Read(cFile &, cMemPool *)` — `src/gcValStringCompare.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValConsoleInfo::GetText(char *) const`

- Address: `0x00158e84`
- Size: `408` bytes
- Source: `src/gcValConsoleInfo_GetText.cpp`

```cpp
void gcValConsoleInfo::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36F1AC, (const char *)0x36DAF0);

    int validKind = 0;
    int kind = *(int *)((const char *)this + 8);
    if ((kind == 0x15) || (kind == 0x16) || (kind == 0x17)) {
        validKind = 1;
    }
    validKind &= 0xFF;

    if (validKind != 0) {
        if (*(int *)((const char *)this + 0xC) == 0) {
            int val = *(int *)((const char *)this + 0x10);
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
                char *type = *(char **)(check + 4);
                cTypeMethod *slot = (cTypeMethod *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)val + slot->offset, buf);
            } else {
                cStrCat(buf, (const char *)0x36DB24);
            }
        } else {
            int val = *(int *)((const char *)this + 0x14);
            int tagged = 0;
            int tag = val & 1;
            if (tag != 0) {
                tagged = 1;
            }

            int useFallback;
            if (tagged == 0) {
                goto text_not_tagged;
            }
            useFallback = 1;
            goto text_fallback_done;
        text_not_tagged:
            int zero = val == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        text_fallback_done:

            if (useFallback != 0) {
                cStrAppend(buf, (const char *)0x36DACC);
            } else {
                char local[256];
                local[0] = '\0';

                int taggedAgain = 0;
                if (tag != 0) {
                    taggedAgain = 1;
                }

                char *type;
                int base = val;
                if (taggedAgain != 0) {
                    base = 0;
                    type = *(char **)(base + 4);
                } else {
                    type = *(char **)(base + 4);
                }

                cTypeMethod *slot = (cTypeMethod *)(type + 0x40);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)base + slot->offset, local);
                cStrAppend(buf, local);
            }
        }
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}
```

### Exemplar 2: `gcValEntitySoundFrequency::GetText(char *) const`

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

### Exemplar 3: `gcValObjectIsValid::GetText(char *) const`

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

### Exemplar 4: `gcValUIListInfo::GetText(char *) const`

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


## Prior Failure Notes

### Note 1

- session `b8c9eae9`; src `src/gcValStringCompare.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/b8c9eae9/0035e0cc__agent_self_reported_failure__src_gcValStringCompare.cpp`

Closest attempt is the permuter-saved variant at 13/384 bytes differing. The high-level body is reconstructed as two 256-byte stack buffers, each filled by desired-object text via vtable slot +0x40 or cStrCopy(0x36DACC), followed by the mExact-selected final cStrAppend format. Manual attempts included initial m2c-shaped control flow, declaration-order changes to force flag/value into a0/a1, duplicating the second operand load/tag computation across both predecessors, explicit right-buffer register use, a sched=1 test, and a 300s permuter run over 8448 candidates; the permuter improved 16 differing bytes to 13 but did not match.

The remaining diff is not a bnel/beql compiler-divergence case. It appears to be SNC scheduling/register-placement drift in the prologue and first call setup: small reorderings of saves, stack byte stores, s1/s2 materialization, and the early cStrCopy call setup move/lui placement. Existing matched siblings in this file remain matched with the closest attempt.


## Disassembly

```asm
35e0cc:	27bdfdf0 	addiu	sp,sp,-528
35e0d0:	a3a00000 	sb	zero,0(sp)
35e0d4:	afb00200 	sw	s0,512(sp)
35e0d8:	afb10204 	sw	s1,516(sp)
35e0dc:	00808025 	move	s0,a0
35e0e0:	00a08825 	move	s1,a1
35e0e4:	a3a00100 	sb	zero,256(sp)
35e0e8:	8e050008 	lw	a1,8(s0)
35e0ec:	afb20208 	sw	s2,520(sp)
35e0f0:	27b20100 	addiu	s2,sp,256
35e0f4:	34040000 	li	a0,0x0
35e0f8:	30a60001 	andi	a2,a1,0x1
35e0fc:	afbf020c 	sw	ra,524(sp)
35e100:	54c00001 	bnezl	a2,0x35e108
35e104:	34040001 	li	a0,0x1
35e108:	10800003 	beqz	a0,0x35e118
35e10c:	0005202b 	sltu	a0,zero,a1
35e110:	10000003 	b	0x35e120
35e114:	34040000 	li	a0,0x0
35e118:	308400ff 	andi	a0,a0,0xff
35e11c:	0004202b 	sltu	a0,zero,a0
35e120:	10800012 	beqz	a0,0x35e16c
35e124:	34040000 	li	a0,0x0
35e128:	54c00001 	bnezl	a2,0x35e130
35e12c:	34040001 	li	a0,0x1
35e130:	50800005 	beqzl	a0,0x35e148
35e134:	8ca40004 	lw	a0,4(a1)
35e138:	34050000 	li	a1,0x0
35e13c:	8ca40004 	lw	a0,4(a1)
35e140:	10000002 	b	0x35e14c
35e144:	24840040 	addiu	a0,a0,64
35e148:	24840040 	addiu	a0,a0,64
35e14c:	84860000 	lh	a2,0(a0)
35e150:	8c870004 	lw	a3,4(a0)
35e154:	00a62021 	addu	a0,a1,a2
35e158:	00e0f809 	jalr	a3
35e15c:	03a02825 	move	a1,sp
35e160:	8e05000c 	lw	a1,12(s0)
35e164:	10000007 	b	0x35e184
35e168:	30a40001 	andi	a0,a1,0x1
35e16c:	3c050037 	lui	a1,0x37
35e170:	03a02025 	move	a0,sp
35e174:	0c00050a 	jal	0x1428
35e178:	24a5dacc 	addiu	a1,a1,-9524
35e17c:	8e05000c 	lw	a1,12(s0)
35e180:	30a40001 	andi	a0,a1,0x1
35e184:	34060000 	li	a2,0x0
35e188:	54800001 	bnezl	a0,0x35e190
35e18c:	34060001 	li	a2,0x1
35e190:	10c00003 	beqz	a2,0x35e1a0
35e194:	0005302b 	sltu	a2,zero,a1
35e198:	10000003 	b	0x35e1a8
35e19c:	34060000 	li	a2,0x0
35e1a0:	30c600ff 	andi	a2,a2,0xff
35e1a4:	0006302b 	sltu	a2,zero,a2
35e1a8:	10c00012 	beqz	a2,0x35e1f4
35e1ac:	00803025 	move	a2,a0
35e1b0:	34040000 	li	a0,0x0
35e1b4:	54c00001 	bnezl	a2,0x35e1bc
35e1b8:	34040001 	li	a0,0x1
35e1bc:	50800005 	beqzl	a0,0x35e1d4
35e1c0:	8ca40004 	lw	a0,4(a1)
35e1c4:	34050000 	li	a1,0x0
35e1c8:	8ca40004 	lw	a0,4(a1)
35e1cc:	10000002 	b	0x35e1d8
35e1d0:	24840040 	addiu	a0,a0,64
35e1d4:	24840040 	addiu	a0,a0,64
35e1d8:	84860000 	lh	a2,0(a0)
35e1dc:	8c870004 	lw	a3,4(a0)
35e1e0:	00a62021 	addu	a0,a1,a2
35e1e4:	00e0f809 	jalr	a3
35e1e8:	02402825 	move	a1,s2
35e1ec:	10000006 	b	0x35e208
35e1f0:	92100011 	lbu	s0,17(s0)
35e1f4:	3c050037 	lui	a1,0x37
35e1f8:	02402025 	move	a0,s2
35e1fc:	0c00050a 	jal	0x1428
35e200:	24a5dacc 	addiu	a1,a1,-9524
35e204:	92100011 	lbu	s0,17(s0)
35e208:	12000004 	beqz	s0,0x35e21c
35e20c:	3c040037 	lui	a0,0x37
35e210:	3c040037 	lui	a0,0x37
35e214:	10000002 	b	0x35e220
35e218:	2484f778 	addiu	a0,a0,-2184
35e21c:	2484f784 	addiu	a0,a0,-2172
35e220:	00802825 	move	a1,a0
35e224:	02202025 	move	a0,s1
35e228:	03a03025 	move	a2,sp
35e22c:	0c000470 	jal	0x11c0
35e230:	02403825 	move	a3,s2
35e234:	8fb00200 	lw	s0,512(sp)
35e238:	8fb10204 	lw	s1,516(sp)
35e23c:	8fb20208 	lw	s2,520(sp)
35e240:	8fbf020c 	lw	ra,524(sp)
35e244:	03e00008 	jr	ra
35e248:	27bd0210 	addiu	sp,sp,528
```

## m2c Starting Point

```c
// Decompiled: gcValStringCompare::GetText(char *) const
// Address: 0x0035e0cc, Size: 384B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, s8 *, s8 *); /* extern */
? cStrCopy_charptr_constcharptr(s8 *, ?, s32);      /* extern */

void gcValStringCompare__GetText_charptrconst(void *arg0, ? arg1) {
    s8 sp100;
    ? (*temp_a3)(s32, s8 *, s16, ?);
    ? (*temp_a3_2)(s32, s8 *, s16, ?);
    ? var_a0_7;
    s16 temp_a2_2;
    s16 temp_a2_3;
    s32 temp_a0;
    s32 temp_a2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_5;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a2_2;
    void *var_a0_4;
    void *var_a0_6;

    sp100 = 0;
    var_a1 = arg0->unk8;
    var_a0 = 0;
    temp_a2 = var_a1 & 1;
    if (temp_a2 != 0) {
        var_a0 = 1;
    }
    if (var_a0 != 0) {
        var_a0_2 = 0;
    } else {
        var_a0_2 = ((var_a1 != 0) & 0xFF) != 0;
    }
    var_a0_3 = 0;
    if (var_a0_2 != 0) {
        if (temp_a2 != 0) {
            var_a0_3 = 1;
        }
        if (var_a0_3 == 0) {
            var_a0_4 = var_a1->unk4;
        } else {
            var_a1 = 0;
            var_a0_4 = *4;
        }
        temp_a2_2 = var_a0_4->unk40;
        temp_a3 = (var_a0_4 + 0x40)->unk4;
        temp_a3(var_a1 + temp_a2_2, sp, temp_a2_2, temp_a3);
        var_a1_2 = arg0->unkC;
    } else {
        cStrCopy_charptr_constcharptr(sp, 0x36DACC, temp_a2);
        var_a1_2 = arg0->unkC;
    }
    temp_a0 = var_a1_2 & 1;
    var_a2 = 0;
    if (temp_a0 != 0) {
        var_a2 = 1;
    }
    if (var_a2 != 0) {
        var_a2_2 = 0;
    } else {
        var_a2_2 = ((var_a1_2 != 0) & 0xFF) != 0;
    }
    if (var_a2_2 != 0) {
        var_a0_5 = 0;
        if (temp_a0 != 0) {
            var_a0_5 = 1;
        }
        if (var_a0_5 == 0) {
            var_a0_6 = var_a1_2->unk4;
        } else {
            var_a1_2 = 0;
            var_a0_6 = *4;
        }
        temp_a2_3 = var_a0_6->unk40;
        temp_a3_2 = (var_a0_6 + 0x40)->unk4;
        temp_a3_2(var_a1_2 + temp_a2_3, &sp100, temp_a2_3, temp_a3_2);
    } else {
        cStrCopy_charptr_constcharptr(&sp100, 0x36DACC, temp_a0);
    }
    if (arg0->unk11 != 0) {
        var_a0_7 = 0x36F778;
    } else {
        var_a0_7 = 0x36F784;
    }
    cStrAppend_charptr_constcharptr_...(arg1, var_a0_7, sp, &sp100);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
