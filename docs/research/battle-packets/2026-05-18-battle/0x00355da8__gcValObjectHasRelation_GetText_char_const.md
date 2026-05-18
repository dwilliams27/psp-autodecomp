# Battle Packet: `gcValObjectHasRelation::GetText(char *) const`

## Target

- Address: `0x00355da8`
- Size: `432` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValObjectHasRelation`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 15B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `15`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `15`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x00001428` `untried` 80B `cStrCopy(char *, const char *)`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Placement

- Canonical source: `src/gcValObjectHasRelation.cpp`
- Header(s): `include/gcValObjectHasRelation.h`
- Allowed source(s): `src/gcValObjectHasRelation.cpp`, `src/gcValObjectHasRelation_GetText.cpp`
- Split-TU prefix: `src/gcValObjectHasRelation_*.cpp`

## Class Header: `include/gcValObjectHasRelation.h`

Not found.

## Matched Same-Class Neighbors

- `0x003555c4` 420B `gcValObjectHasRelation::AssignCopy(const cBase *)` — `src/gcValObjectHasRelation.cpp`
- `0x00355768` 188B `gcValObjectHasRelation::New(cMemPool *, cBase *) static` — `src/gcValObjectHasRelation.cpp`
- `0x00355824` 280B `gcValObjectHasRelation::GetType(void) const` — `src/gcValObjectHasRelation.cpp`
- `0x0035593c` 168B `gcValObjectHasRelation::Write(cFile &) const` — `src/gcValObjectHasRelation.cpp`
- `0x003559e4` 416B `gcValObjectHasRelation::Read(cFile &, cMemPool *)` — `src/gcValObjectHasRelation.cpp`

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

### Exemplar 2: `gcValCinematicInfo::GetText(char *) const`

- Address: `0x003241c4`
- Size: `504` bytes
- Source: `src/gcValCinematicInfo.cpp`

```cpp
void gcValCinematicInfo::GetText(char *buf) const {
    register const gcValCinematicInfo *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;

    cStrAppend(out, (const char *)0x36DCE4, (const char *)0x36DAF0);

    switch (*(int *)((const char *)self + 8)) {
    case 1:
    case 2:
    case 3: {
        int val = *(int *)((const char *)self + 0x0C);
        int tagged = 0;
        int tag = val & 1;
        register const char *suffix __asm__("$18") = (const char *)0x36E440;
        if (tag != 0) {
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
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, (const char *)0x36DAD8);

        val = *(int *)((const char *)self + 0x10);
        tagged = 0;
        if (val & 1) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, suffix);
        break;
    }
    case 7:
    case 8:
    case 9: {
        int val = *(int *)((const char *)self + 0x0C);
        int tagged = 0;
        int tag = val & 1;
        register const char *suffix __asm__("$16") = (const char *)0x36E440;
        if (tag != 0) {
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
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, suffix);
        break;
    }
    case 4:
    case 5:
    case 6:
    default:
        break;
    }

    cStrAppend(out, (const char *)0x36EBE4);
}
```

### Exemplar 3: `gcValLobbyScoreboardRowInfo::GetText(char *) const`

- Address: `0x0034d9cc`
- Size: `560` bytes
- Source: `src/gcValLobbyScoreboardRowInfo.cpp`

```cpp
// 0x0034d9cc -- gcValLobbyScoreboardRowInfo::GetText(char *) const
void gcValLobbyScoreboardRowInfo::GetText(char *buf) const {
    register const gcValLobbyScoreboardRowInfo *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char local[256];

    cStrAppend(out, (const char *)0x36F530);
    cStrAppend(out, (const char *)0x36DCB8, (const char *)0x36DAF0);

    register const char *open __asm__("$18") = (const char *)0x36E300;
    cStrAppend(out, open);

    int tagged = 0;
    int val = *(int *)((const char *)self + 0x10);
    int tag = val & 1;
    register const char *close __asm__("$19") = (const char *)0x36E2E8;
    if (tag != 0) {
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
        GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
        ((void (*)(void *, char *))slot->fn)((char *)val + slot->offset, out);
    } else {
        cStrCat(out, (const char *)0x36DB24);
    }

    cStrAppend(out, close);

    int isTwo = *(int *)((const char *)self + 8) == 2;
    isTwo &= 0xFF;
    if (isTwo != 0) {
        cStrAppend(out, open);

        if (*(int *)((const char *)self + 0x0C) == 0) {
            int obj = *(int *)((const char *)self + 0x14);
            int owned = 0;
            if (obj & 1) {
                owned = 1;
            }
            if (owned != 0) {
                obj = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int objCheck = obj;
            if (objCheck != 0) {
                char *type = *(char **)(objCheck + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)obj + slot->offset, out);
            } else {
                cStrCat(out, (const char *)0x36DB24);
            }
        } else {
            int obj = *(int *)((const char *)self + 0x18);
            int owned = 0;
            int objTag = obj & 1;
            if (objTag != 0) {
                owned = 1;
            }

            int useFallback;
            if (owned == 0) {
                goto object_not_owned;
            }
            useFallback = 1;
            goto object_fallback_done;
        object_not_owned:
            int zero = obj == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        object_fallback_done:

            if (useFallback != 0) {
                cStrAppend(out, (const char *)0x36DACC);
            } else {
                local[0] = '\0';

                int ownedAgain = 0;
                if (objTag != 0) {
                    ownedAgain = 1;
                }

                char *type;
                int base = obj;
                if (ownedAgain != 0) {
                    base = 0;
                    type = *(char **)(base + 4);
                } else {
                    type = *(char **)(base + 4);
                }

                GetTextSlot *slot = (GetTextSlot *)(type + 0x40);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)base + slot->offset, local);
                cStrAppend(out, local);
            }
        }
        cStrAppend(out, close);
    }
}
```

### Exemplar 4: `gcValEntitySoundFrequency::GetText(char *) const`

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


## Prior Failure Notes

### Note 1

- session `1d6df5ab`; src `src/gcValObjectHasRelation_GetText.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/1d6df5ab/00355da8__agent_self_reported_failure__src_gcValObjectHasRelation_GetText.cpp`

Closest attempt is a split-TU C++ implementation in src/gcValObjectHasRelation_GetText.cpp. I first tried adding the method to the existing class TU, but that produced a 293-byte mismatch and a 436-byte emitted symbol; moving it to a split translation unit produced the correct 432-byte size and a 106-byte baseline. A 300s tools/permuter.py --save-best run improved the split source to 15/432 bytes different. The original gcValObjectHasRelation.cpp is restored to its prior matched methods only.

The remaining 15-byte diff is very localized. The best source matches the dispatch/control-flow structure, but two instruction-order/register-allocation spots remain: near the top the compiler emits li a0,0 before text[0]=0 while the target stores text[0] first, and the gcDesiredEnumerationEntryHelper::GetText call sets up the helper bool in the jal delay slot rather than before the addiu a0,self,0x14. Manual tweaks to declaration/assignment order and a register-bound helper flag made the function regress to ~90-byte mismatches, so the permuter-saved 15-byte attempt is preserved.


## Disassembly

```asm
355da8:	27bdfef0 	addiu	sp,sp,-272
355dac:	afb00100 	sw	s0,256(sp)
355db0:	afb10104 	sw	s1,260(sp)
355db4:	00808025 	move	s0,a0
355db8:	00a08825 	move	s1,a1
355dbc:	8e050008 	lw	a1,8(s0)
355dc0:	a3a00000 	sb	zero,0(sp)
355dc4:	34040000 	li	a0,0x0
355dc8:	30a60001 	andi	a2,a1,0x1
355dcc:	afbf0108 	sw	ra,264(sp)
355dd0:	54c00001 	bnezl	a2,0x355dd8
355dd4:	34040001 	li	a0,0x1
355dd8:	10800003 	beqz	a0,0x355de8
355ddc:	0005202b 	sltu	a0,zero,a1
355de0:	10000003 	b	0x355df0
355de4:	34040000 	li	a0,0x0
355de8:	308400ff 	andi	a0,a0,0xff
355dec:	0004202b 	sltu	a0,zero,a0
355df0:	10800011 	beqz	a0,0x355e38
355df4:	34040000 	li	a0,0x0
355df8:	54c00001 	bnezl	a2,0x355e00
355dfc:	34040001 	li	a0,0x1
355e00:	50800005 	beqzl	a0,0x355e18
355e04:	8ca40004 	lw	a0,4(a1)
355e08:	34050000 	li	a1,0x0
355e0c:	8ca40004 	lw	a0,4(a1)
355e10:	10000002 	b	0x355e1c
355e14:	24840078 	addiu	a0,a0,120
355e18:	24840078 	addiu	a0,a0,120
355e1c:	84860000 	lh	a2,0(a0)
355e20:	8c870004 	lw	a3,4(a0)
355e24:	00a62021 	addu	a0,a1,a2
355e28:	00e0f809 	jalr	a3
355e2c:	03a02825 	move	a1,sp
355e30:	10000006 	b	0x355e4c
355e34:	92050024 	lbu	a1,36(s0)
355e38:	3c050037 	lui	a1,0x37
355e3c:	03a02025 	move	a0,sp
355e40:	0c00050a 	jal	0x1428
355e44:	24a5dacc 	addiu	a1,a1,-9524
355e48:	92050024 	lbu	a1,36(s0)
355e4c:	10a00004 	beqz	a1,0x355e60
355e50:	3c040037 	lui	a0,0x37
355e54:	3c040037 	lui	a0,0x37
355e58:	10000002 	b	0x355e64
355e5c:	2484f698 	addiu	a0,a0,-2408
355e60:	2484f6b8 	addiu	a0,a0,-2376
355e64:	00802825 	move	a1,a0
355e68:	02202025 	move	a0,s1
355e6c:	0c000470 	jal	0x11c0
355e70:	03a03025 	move	a2,sp
355e74:	a3a00000 	sb	zero,0(sp)
355e78:	34060001 	li	a2,0x1
355e7c:	26040014 	addiu	a0,s0,20
355e80:	0c043a56 	jal	0x10e958
355e84:	03a02825 	move	a1,sp
355e88:	8e050020 	lw	a1,32(s0)
355e8c:	34040000 	li	a0,0x0
355e90:	30a60001 	andi	a2,a1,0x1
355e94:	54c00001 	bnezl	a2,0x355e9c
355e98:	34040001 	li	a0,0x1
355e9c:	10800003 	beqz	a0,0x355eac
355ea0:	0005202b 	sltu	a0,zero,a1
355ea4:	10000003 	b	0x355eb4
355ea8:	34050000 	li	a1,0x0
355eac:	308500ff 	andi	a1,a0,0xff
355eb0:	0005282b 	sltu	a1,zero,a1
355eb4:	10a0001f 	beqz	a1,0x355f34
355eb8:	3c050037 	lui	a1,0x37
355ebc:	3c050037 	lui	a1,0x37
355ec0:	03a02025 	move	a0,sp
355ec4:	0c000470 	jal	0x11c0
355ec8:	24a5dcbc 	addiu	a1,a1,-9028
355ecc:	8e100020 	lw	s0,32(s0)
355ed0:	34040000 	li	a0,0x0
355ed4:	32050001 	andi	a1,s0,0x1
355ed8:	54a00001 	bnezl	a1,0x355ee0
355edc:	34040001 	li	a0,0x1
355ee0:	10800003 	beqz	a0,0x355ef0
355ee4:	00000000 	nop
355ee8:	10000001 	b	0x355ef0
355eec:	34100000 	li	s0,0x0
355ef0:	02002025 	move	a0,s0
355ef4:	1080000b 	beqz	a0,0x355f24
355ef8:	3c050037 	lui	a1,0x37
355efc:	8c840004 	lw	a0,4(a0)
355f00:	03a02825 	move	a1,sp
355f04:	248400d0 	addiu	a0,a0,208
355f08:	84860000 	lh	a2,0(a0)
355f0c:	8c870004 	lw	a3,4(a0)
355f10:	00e0f809 	jalr	a3
355f14:	02062021 	addu	a0,s0,a2
355f18:	10000006 	b	0x355f34
355f1c:	3c050037 	lui	a1,0x37
355f20:	3c050037 	lui	a1,0x37
355f24:	03a02025 	move	a0,sp
355f28:	0c0004da 	jal	0x1368
355f2c:	24a5db24 	addiu	a1,a1,-9436
355f30:	3c050037 	lui	a1,0x37
355f34:	02202025 	move	a0,s1
355f38:	03a03025 	move	a2,sp
355f3c:	0c000470 	jal	0x11c0
355f40:	24a5ef10 	addiu	a1,a1,-4336
355f44:	8fb00100 	lw	s0,256(sp)
355f48:	8fb10104 	lw	s1,260(sp)
355f4c:	8fbf0108 	lw	ra,264(sp)
355f50:	03e00008 	jr	ra
355f54:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: gcValObjectHasRelation::GetText(char *) const
// Address: 0x00355da8, Size: 432B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(void *, s32, void *); /* extern */
? cStrCat_charptr_constcharptr(void *, ?);          /* extern */
? cStrCopy_charptr_constcharptr(void *, ?, s32);    /* extern */
? gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *, void *, ?); /* extern */

void gcValObjectHasRelation__GetText_charptrconst(void *arg0, void *arg1) {
    ? (*temp_a3)(s32, void *, s16, ?);
    ? (*temp_a3_2)(s32, void *, s16, ?);
    s16 temp_a2_2;
    s16 temp_a2_4;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a2_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_a0_7;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_s0;
    void *temp_a0;
    void *var_a0_4;

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
        temp_a2_2 = var_a0_4->unk78;
        temp_a3 = (var_a0_4 + 0x78)->unk4;
        temp_a3(var_a1 + temp_a2_2, sp, temp_a2_2, temp_a3);
    } else {
        cStrCopy_charptr_constcharptr(sp, 0x36DACC, temp_a2);
    }
    if (arg0->unk24 != 0) {
        var_a0_5 = 0x36F698;
    } else {
        var_a0_5 = 0x36F6B8;
    }
    cStrAppend_charptr_constcharptr_...(arg1, var_a0_5, sp);
    gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(arg0 + 0x14, sp, 1);
    temp_a1 = arg0->unk20;
    var_a0_6 = 0;
    temp_a2_3 = temp_a1 & 1;
    if (temp_a2_3 != 0) {
        var_a0_6 = 1;
    }
    if (var_a0_6 != 0) {
        var_a1_2 = 0;
    } else {
        var_a1_2 = ((temp_a1 != 0) & 0xFF) != 0;
    }
    if (var_a1_2 != 0) {
        cStrAppend_charptr_constcharptr_...(sp, 0x36DCBC, (void *) temp_a2_3);
        var_s0 = arg0->unk20;
        var_a0_7 = 0;
        if (var_s0 & 1) {
            var_a0_7 = 1;
        }
        if (var_a0_7 != 0) {
            var_s0 = 0;
        }
        if (var_s0 != 0) {
            temp_a0 = var_s0->unk4;
            temp_a2_4 = temp_a0->unkD0;
            temp_a3_2 = (temp_a0 + 0xD0)->unk4;
            temp_a3_2(var_s0 + temp_a2_4, sp, temp_a2_4, temp_a3_2);
        } else {
            cStrCat_charptr_constcharptr(sp, 0x36DB24);
        }
    }
    cStrAppend_charptr_constcharptr_...(arg1, 0x36EF10, sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
