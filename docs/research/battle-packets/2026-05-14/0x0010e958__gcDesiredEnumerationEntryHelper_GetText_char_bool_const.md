# Battle Packet: `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`

## Target

- Address: `0x0010e958`
- Size: `372` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDesiredEnumerationEntryHelper`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `8`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x00004b38` `failed` 116B `cStr::cStr(const char *, ...)`
- `0x0023a260` `failed` 820B `cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::GetName(char *, bool, char *) const`

Callers:
- `0x0010ebc4` `untried` 500B `gcDesiredEnumerationEntry::GetTextEx(char *, bool, bool) const`
- `0x002872fc` `failed` 416B `gcProfileString::GetName(char *) const`
- `0x002a13e0` `failed` 292B `gcDoEntityActivatePartialController::GetText(char *) const`
- `0x002a6a3c` `matched` 572B `gcDoEntityAttack::GetText(char *) const`
- `0x002b94d0` `matched` 572B `gcDoEntityPartialBodyAttack::GetText(char *) const`
- `0x002bb198` `matched` 572B `gcDoEntityPlayAnimation::GetText(char *) const`
- `0x002bccbc` `matched` 572B `gcDoEntityPlayPartialBodyAnimation::GetText(char *) const`
- `0x002c1478` `matched` 648B `gcDoEntitySendMessage::GetText(char *) const`
- `0x002c2fdc` `failed` 644B `gcDoEntitySendPartialControllerMessage::GetText(char *) const`
- `0x002c961c` `failed` 584B `gcDoEntitySetLookAtTarget::GetText(char *) const`
- `0x002e9c3c` `matched` 484B `gcDoObjectForEachRelationship::GetText(char *) const`
- `0x002fc87c` `failed` 244B `gcDoSetEventEnumParam::GetText(char *) const`
- `0x0030e86c` `failed` 428B `gcDoUISendMessage::GetText(char *) const`
- `0x003268c4` `failed` 288B `gcValEntityConstant::GetText(char *) const`
- `0x0032f100` `failed` 288B `gcValEntityHasAnimation::GetText(char *) const`
- `0x00330eac` `failed` 288B `gcValEntityHasTimeSince::GetText(char *) const`
- `0x00336f78` `failed` 296B `gcValLookAtControllerVariable::GetText(char *) const`
- `0x003384d8` `failed` 296B `gcValEntityPartialControllerVariable::GetText(char *) const`
- `0x0033f480` `failed` 288B `gcValEntityTimeSince::GetText(char *) const`
- `0x003407ec` `failed` 288B `gcValEntityVariable::GetText(char *) const`
- `0x00342470` `failed` 216B `gcValEnumerationEntry::GetText(char *) const`
- `0x003455bc` `failed` 516B `gcValGameSetting::GetText(char *) const`
- `0x00346654` `failed` 312B `gcValHasPartialController::GetText(char *) const`
- `0x00354e90` `failed` 408B `gcValObjectHasCategory::GetText(char *) const`

## Placement

- Canonical source: `src/gcDesiredEnumerationEntryHelper.cpp`
- Header(s): `include/gcDesiredEnumerationEntryHelper.h`
- Allowed source(s): `src/gcDesiredEnumerationEntryHelper.cpp`
- Split-TU prefix: `src/gcDesiredEnumerationEntryHelper_*.cpp`

## Class Header: `include/gcDesiredEnumerationEntryHelper.h`

Not found.

## Matched Same-Class Neighbors

- `0x0010e450` 104B `gcDesiredEnumerationEntryHelper::Write(cWriteBlock &) const` — `src/gcDesiredEnumerationEntryHelper.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDesiredEntity::GetText(char *) const`

- Address: `0x0011d7ec`
- Size: `212` bytes
- Source: `src/gcDesiredEntity.cpp`

```cpp
void gcDesiredEntity::GetText(char *buf) const {
    int val = *(const int *)((const char *)this + 8);
    int isOwned = 0;
    int valBit0 = val & 1;
    if (valBit0) {
        isOwned = 1;
    }
    if (isOwned != 0) {
        isOwned = 0;
    } else {
        isOwned = (val != 0);
        isOwned &= 0xFF;
        isOwned = isOwned != 0;
    }
    if (isOwned == 0) {
        ((const gcDesiredEntityHelper *)((const char *)this + 12))->GetText(buf);
        val = *(const int *)((const char *)this + 8);
        valBit0 = val & 1;
    }
    int flag = 0;
    if (valBit0 != 0) {
        flag = 1;
    }
    int doIt;
    if (flag != 0) {
        doIt = 1;
    } else {
        doIt = (val == 0);
        doIt &= 0xFF;
        doIt = doIt != 0;
    }
    if (doIt != 0) {
        char tmp[256];
        tmp[0] = 0;
        if (((const gcDesiredEnumerationEntry *)((const char *)this + 0x14))
                ->GetTextEx(tmp, false, false) != 0) {
            cStrAppend(buf, (const char *)0x36DE40, tmp);
        }
    }
}
```

### Exemplar 2: `gcDesiredCamera::GetText(char *) const`

- Address: `0x00121a90`
- Size: `128` bytes
- Source: `src/gcDesiredCamera.cpp`

```cpp
// 0x00121a90 - gcDesiredCamera::GetText(char *) const
void gcDesiredCamera::GetText(char *buf) const {
    int val = *(const int *)((const char *)this + 8);
    if (val == 0) {
        char *typeInfo = *(char **)((const char *)this + 16);
        GetTextRec *rec = (GetTextRec *)(typeInfo + 0x78);
        short off = rec->offset;
        void *base = (char *)this + 12;
        rec->fn((char *)base + off, buf);
    } else {
        cStrCat(buf, (const char *)0x36DAF0);
    }
    cStrCat(buf, (const char *)0x36DADC);
    cStrCat(buf, (const char *)0x36DE70);
}
```

### Exemplar 3: `gcExpression::GetText(char *) const`

- Address: `0x0023a21c`
- Size: `68` bytes
- Source: `src/gcExpression.cpp`

```cpp
void gcExpression::GetText(char *buf) const {
    const gcExprVEntry *vt = *(const gcExprVEntry **)((char *)this + 4);
    const gcExprVEntry *e = vt + 1;
    const void *r = e->fn((const char *)this + e->adj);
    cStrCat(buf, *(const char **)((char *)r + 0xC));
}
```

### Exemplar 4: `gcDesiredUIWidget::GetText(char *) const`

- Address: `0x0012edc4`
- Size: `28` bytes
- Source: `src/gcDesiredUIWidget.cpp`

```cpp
void gcDesiredUIWidget::GetText(char *buf) const {
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(buf);
}
```


## Prior Failure Notes

### Note 1

- session `ee788b26`; src `src/gcDesiredEnumerationEntryHelper.cpp`; snapshot `logs/failure_snapshots/match_20260505_123104/ee788b26/0010e958__agent_self_reported_failure__src_gcDesiredEnumerationEntryHelper.cpp`

gcDesiredEnumerationEntryHelper::GetText(char *, bool) const — closest attempt is 368B (4-byte/1-instruction shorter than expected 372B). Source uses extern-C cHandlePairT_GetName + 0x38890 dispatch identical to matched gcDesiredEnumeration::GetText, plus a `cStr text;` local with implicit op= (compiler synthesizes 256-byte memcpy via lwl/lwr/swl/swr unrolled by 8) for the temp-from-cStr(src) copy. cStrCat (2-arg) for primary case and cStrAppend (3-arg variadic) for the trailing mField1!=0 append, both confirmed via func_db (cStrAppend at 0x11c0, cStrCat at 0x1368). All other bytes match (prologue, dispatch math, table lookup at 0x38890, isZero+&0xFF idiom, 256-byte memcpy unroll, cStrCat/cStrAppend tail).

Divergence is NOT the bnezl/beqzl branch-likely pattern — both expected and mine emit plain `beqz a0, ...` for the inner isZero check. The diff is a CFG-flattening difference: expected emits an extra intermediate merge `b 0xbc; nop` before jumping to outer cStr ctor target 0xdc. The compiler keeps a 2-step jump (inner-merge → outer-merge) where mine collapses both inner-if branches to jump directly to the cStr ctor at 0xd8. This eliminates 1 hop (2 instrs) but mine fills a beqz delay slot with nop (+1 instr), net -1 instr / -4 bytes. This is a sibling of the ADR 006 issue but at the CFG-flattening layer rather than branch-likely emission.

Restructurings tried (6): (1) original `if (isZero == 0) { src = entry+8 } else { src = DAB8 }` → 364B (used bnezl); (2) inverted `if (isZero != 0) { src = DAB8 } else { src = entry+8 }` → 368B (closest); (3) `goto have_src` after each src assignment → 364B; (4) ternary `src = (isZero == 0) ? entry+8 : DAB8` → 368B; (5) `do { ... break; ... } while(0)` wrapper → 364B; (6) explicit `goto entry_done` with empty label after inner else → 364B. Best stays 368B with the inverted form.

Permuter cannot run on this file in-place because the .o contains both Write (104B, matched) and GetText (368B/372B) totaling >>372B; isolating GetText to a standalone TU could enable permuter exploration of the CFG-flattening hop. Qualifies for bnel/branch-likely-cousin unmatchable classification per docs/decisions/006 (≤8 byte diff, >=3 manual restructurings, all other bytes match).


## Disassembly

```asm
10e958:	27bdfdf0 	addiu	sp,sp,-528
10e95c:	afb00200 	sw	s0,512(sp)
10e960:	00808025 	move	s0,a0
10e964:	82070000 	lb	a3,0(s0)
10e968:	30c400ff 	andi	a0,a2,0xff
10e96c:	34060001 	li	a2,0x1
10e970:	afb10204 	sw	s1,516(sp)
10e974:	afbf0208 	sw	ra,520(sp)
10e978:	14e60043 	bne	a3,a2,0x10ea88
10e97c:	00a08825 	move	s1,a1
10e980:	82050001 	lb	a1,1(s0)
10e984:	14a00008 	bnez	a1,0x10e9a8
10e988:	a3a00000 	sb	zero,0(sp)
10e98c:	00803025 	move	a2,a0
10e990:	26040004 	addiu	a0,s0,4
10e994:	03a02825 	move	a1,sp
10e998:	0c08e898 	jal	0x23a260
10e99c:	34070001 	li	a3,0x1
10e9a0:	10000035 	b	0x10ea78
10e9a4:	02202025 	move	a0,s1
10e9a8:	8e050004 	lw	a1,4(s0)
10e9ac:	14a00003 	bnez	a1,0x10e9bc
10e9b0:	27a40100 	addiu	a0,sp,256
10e9b4:	1000000d 	b	0x10e9ec
10e9b8:	34060000 	li	a2,0x0
10e9bc:	30a6ffff 	andi	a2,a1,0xffff
10e9c0:	3c070004 	lui	a3,0x4
10e9c4:	00063080 	sll	a2,a2,0x2
10e9c8:	24e78890 	addiu	a3,a3,-30576
10e9cc:	00c73021 	addu	a2,a2,a3
10e9d0:	8cc70000 	lw	a3,0(a2)
10e9d4:	10e00005 	beqz	a3,0x10e9ec
10e9d8:	34060000 	li	a2,0x0
10e9dc:	8ce80030 	lw	t0,48(a3)
10e9e0:	15050002 	bne	t0,a1,0x10e9ec
10e9e4:	00000000 	nop
10e9e8:	00e03025 	move	a2,a3
10e9ec:	10c0000b 	beqz	a2,0x10ea1c
10e9f0:	00000000 	nop
10e9f4:	84c5001c 	lh	a1,28(a2)
10e9f8:	2ca50001 	sltiu	a1,a1,1
10e9fc:	30a500ff 	andi	a1,a1,0xff
10ea00:	10a00004 	beqz	a1,0x10ea14
10ea04:	24c60008 	addiu	a2,a2,8
10ea08:	3c060037 	lui	a2,0x37
10ea0c:	10000001 	b	0x10ea14
10ea10:	24c6dab8 	addiu	a2,a2,-9544
10ea14:	10000007 	b	0x10ea34
10ea18:	00000000 	nop
10ea1c:	10a00004 	beqz	a1,0x10ea30
10ea20:	3c060037 	lui	a2,0x37
10ea24:	3c060037 	lui	a2,0x37
10ea28:	10000002 	b	0x10ea34
10ea2c:	24c6dac4 	addiu	a2,a2,-9532
10ea30:	24c6dacc 	addiu	a2,a2,-9524
10ea34:	0c0012ce 	jal	0x4b38
10ea38:	00c02825 	move	a1,a2
10ea3c:	34040000 	li	a0,0x0
10ea40:	34050020 	li	a1,0x20
10ea44:	03a43021 	addu	a2,sp,a0
10ea48:	98c70100 	lwr	a3,256(a2)
10ea4c:	88c70103 	lwl	a3,259(a2)
10ea50:	98c80104 	lwr	t0,260(a2)
10ea54:	88c80107 	lwl	t0,263(a2)
10ea58:	b8c70000 	swr	a3,0(a2)
10ea5c:	a8c70003 	swl	a3,3(a2)
10ea60:	b8c80004 	swr	t0,4(a2)
10ea64:	a8c80007 	swl	t0,7(a2)
10ea68:	24a5ffff 	addiu	a1,a1,-1
10ea6c:	14a0fff5 	bnez	a1,0x10ea44
10ea70:	24840008 	addiu	a0,a0,8
10ea74:	02202025 	move	a0,s1
10ea78:	0c0004da 	jal	0x1368
10ea7c:	03a02825 	move	a1,sp
10ea80:	10000006 	b	0x10ea9c
10ea84:	82100001 	lb	s0,1(s0)
10ea88:	3c050037 	lui	a1,0x37
10ea8c:	02202025 	move	a0,s1
10ea90:	0c0004da 	jal	0x1368
10ea94:	24a5daf0 	addiu	a1,a1,-9488
10ea98:	82100001 	lb	s0,1(s0)
10ea9c:	12000006 	beqz	s0,0x10eab8
10eaa0:	3c050037 	lui	a1,0x37
10eaa4:	3c060037 	lui	a2,0x37
10eaa8:	02202025 	move	a0,s1
10eaac:	24a5dcb8 	addiu	a1,a1,-9032
10eab0:	0c000470 	jal	0x11c0
10eab4:	24c6daf0 	addiu	a2,a2,-9488
10eab8:	8fb00200 	lw	s0,512(sp)
10eabc:	8fb10204 	lw	s1,516(sp)
10eac0:	8fbf0208 	lw	ra,520(sp)
10eac4:	03e00008 	jr	ra
10eac8:	27bd0210 	addiu	sp,sp,528
```

## m2c Starting Point

```c
// Decompiled: gcDesiredEnumerationEntryHelper::GetText(char *, bool) const
// Address: 0x0010e958, Size: 372B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, ?);     /* extern */
? cStrCat_charptr_constcharptr(?, void *, ?, s8);   /* extern */
? cStr__cStr_constcharptr_...(? *, void *, void *, void *); /* extern */
? constcharptrcHandlePairT_gcEnumeration_cSubHandleT_gcEnumerationEntry____GetName_charptr_bool_charptrconst(void *, void *, s32, ?); /* extern */

void gcDesiredEnumerationEntryHelper__GetText_charptr_boolconst(void *arg0, ? arg1, s32 arg2) {
    ? sp100;
    s32 temp_a1;
    s32 var_a0;
    s32 var_a1;
    s8 var_a2;
    s8 var_a3;
    void *temp_a2;
    void *var_a2_2;

    var_a3 = arg0->unk0;
    if (var_a3 == 1) {
        if (arg0->unk1 == 0) {
            constcharptrcHandlePairT_gcEnumeration_cSubHandleT_gcEnumerationEntry____GetName_charptr_bool_charptrconst(arg0 + 4, sp, arg2 & 0xFF, 1);
        } else {
            temp_a1 = arg0->unk4;
            if (temp_a1 == 0) {
                var_a2 = 0;
            } else {
                var_a3 = (s8) ((temp_a1 & 0xFFFF) * 4)->unk38890;
                var_a2 = 0;
                if ((var_a3 != 0) && (var_a3->unk30 == temp_a1)) {
                    var_a2 = var_a3;
                }
            }
            if (var_a2 != 0) {
                var_a2_2 = var_a2 + 8;
                if ((var_a2->unk1C == 0) & 0xFF) {
                    var_a2_2 = (void *)0x36DAB8;
                }
            } else if (temp_a1 != 0) {
                var_a2_2 = (void *)0x36DAC4;
            } else {
                var_a2_2 = (void *)0x36DACC;
            }
            cStr__cStr_constcharptr_...(&sp100, var_a2_2, var_a2_2, (void *) var_a3);
            var_a0 = 0;
            var_a1 = 0x20;
            do {
                temp_a2 = sp + var_a0;
                temp_a2->unk0 = (unaligned s32) M2C_LWL(temp_a2->unk103);
                temp_a2->unk4 = (unaligned s32) M2C_LWL(temp_a2->unk107);
                var_a1 -= 1;
                var_a0 += 8;
            } while (var_a1 != 0);
        }
        cStrCat_charptr_constcharptr(arg1, sp);
    } else {
        cStrCat_charptr_constcharptr(arg1, (void *)0x36DAF0, 1, var_a3);
    }
    if (arg0->unk1 != 0) {
        cStrAppend_charptr_constcharptr_...(arg1, 0x36DCB8, 0x36DAF0);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
