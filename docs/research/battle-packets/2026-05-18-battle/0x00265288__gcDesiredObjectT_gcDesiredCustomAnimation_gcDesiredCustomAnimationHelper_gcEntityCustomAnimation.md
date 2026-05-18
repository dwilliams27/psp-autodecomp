# Battle Packet: `gcDesiredObjectT<gcDesiredCustomAnimation, gcDesiredCustomAnimationHelper, gcEntityCustomAnimation>::GetText(char *) const`

## Target

- Address: `0x00265288`
- Size: `264` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDesiredObjectT<gcDesiredCustomAnimation, gcDesiredCustomAnimationHelper, gcEntityCustomAnimation>`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `189`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`
- `0x001251c8` `failed` 12B `gcDesiredObjectHelper::GetPrimaryText(gcDesiredObjectHelper::gcPrimary) static`

## Placement

- Canonical source: `src/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.cpp`
- Header(s): `include/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.h`
- Allowed source(s): `src/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.cpp`
- Split-TU prefix: `src/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation_*.cpp`

## Class Header: `include/gcDesiredObjectT<gcDesiredCustomAnimation, gcDesiredCustomAnimationHelper, gcEntityCustomAnimation>.h`

Not found.

## Matched Same-Class Neighbors

- `0x00265160` 296B `gcDesiredObjectT<gcDesiredCustomAnimation, gcDesiredCustomAnimationHelper, gcEntityCustomAnimation>::GetDesiredType(void) const` — `src/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.cpp`

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

- session `7d3a7c7f`; src `src/gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.cpp`; snapshot `logs/failure_snapshots/match_20260505_123104/7d3a7c7f/00265288__agent_self_reported_failure__src_gcDesiredObjectTgcDesiredCustomAnimationgcDesiredCustomAnimationHelpergcEntityCustomAnimation.cpp`

GetText reaches the correct 264-byte size but 69/264 bytes differ from a register-allocation cascade rooted in the prologue scheduling. Original schedules `lw a2,8(a0)` at offset 4 BEFORE `sw s0,0(sp)`, so val lives in a2 and flag in a1 throughout. My emit schedules `sw s0` first, then `move s0,a1`, then `lw a1,8(a0)` — so val lives in a1 and flag in a2. Every downstream register choice cascades from this initial scheduling decision (e.g. original later does `lw a0,16(a0)` overwriting `this` while mine does `lw a1,16(a0)` then masks with `andi a0,a1,0xffff`). Source restructurings tested: (1) the verbose bool-collapse pattern from gcDesiredString::GetText (`int raw = (v8 != 0); hasReal = ((unsigned char)raw) != 0;`) — got the 9-instruction bnezl→beqz→sltu→b→li→andi→sltu→bnez sequence at offsets 0x1c-0x3c that the original uses (was 8-byte short before), and got `sltiu` (not `slti`) by casting `(unsigned int)(int)v < 1u`; (2) swapping `if (isOne != 0) /*0x36DAB8*/ else /*entry+8*/` so the inner branch emits beqz instead of bnezl; (3) #pragma control sched=1 (made it worse: 76 bytes diff) and sched=3 (rejected, range 0..2); (4) declaring explicit struct fields mNext/mType/mHandleId in the class and using member access — same emit; (5) changing `unsigned int v8` to `int v8` — no change. The remaining diff is purely the register-allocation cascade caused by the prologue scheduling: I cannot find a source pattern that makes SNC emit `lw` before `sw s0` here. Sister function gcDesiredEnumeration::GetText @ 0x001286f4 in the same gcAll_psp.obj has the identical issue and is also still in_progress (status verified via func_db). Suggested next steps: (a) run permuter for ≥5min — the bool-collapse fix was already non-trivial so permuter has runway; (b) try forcing buf into a stack-spilled local before any `this` access; (c) explore if `__asm__ volatile("" ::: "memory")` between specific statements influences scheduling; (d) compare against the actually-matched gcDesiredEntity-family GetText (e.g. gcDesiredEntityTemplate::GetText if it exists) to see what source shape gets the early-lw scheduling. Closest source kept in the file in the canonical form modeled after gcDesiredEnumeration::GetText so future agents can iterate from a clean baseline.


## Disassembly

```asm
265288:	27bdfff0 	addiu	sp,sp,-16
26528c:	8c860008 	lw	a2,8(a0)
265290:	afb00000 	sw	s0,0(sp)
265294:	00a08025 	move	s0,a1
265298:	34050000 	li	a1,0x0
26529c:	30c70001 	andi	a3,a2,0x1
2652a0:	afbf0004 	sw	ra,4(sp)
2652a4:	54e00001 	bnezl	a3,0x2652ac
2652a8:	34050001 	li	a1,0x1
2652ac:	10a00003 	beqz	a1,0x2652bc
2652b0:	0006282b 	sltu	a1,zero,a2
2652b4:	10000003 	b	0x2652c4
2652b8:	34060000 	li	a2,0x0
2652bc:	30a600ff 	andi	a2,a1,0xff
2652c0:	0006302b 	sltu	a2,zero,a2
2652c4:	14c0002e 	bnez	a2,0x265380
2652c8:	00000000 	nop
2652cc:	8c85000c 	lw	a1,12(a0)
2652d0:	34060007 	li	a2,0x7
2652d4:	14a60025 	bne	a1,a2,0x26536c
2652d8:	00000000 	nop
2652dc:	8c840010 	lw	a0,16(a0)
2652e0:	14800003 	bnez	a0,0x2652f0
2652e4:	3085ffff 	andi	a1,a0,0xffff
2652e8:	1000000c 	b	0x26531c
2652ec:	34050000 	li	a1,0x0
2652f0:	3c060004 	lui	a2,0x4
2652f4:	00052880 	sll	a1,a1,0x2
2652f8:	24c68890 	addiu	a2,a2,-30576
2652fc:	00a62821 	addu	a1,a1,a2
265300:	8ca60000 	lw	a2,0(a1)
265304:	10c00005 	beqz	a2,0x26531c
265308:	34050000 	li	a1,0x0
26530c:	8cc70030 	lw	a3,48(a2)
265310:	14e40002 	bne	a3,a0,0x26531c
265314:	00000000 	nop
265318:	00c02825 	move	a1,a2
26531c:	10a0000b 	beqz	a1,0x26534c
265320:	00000000 	nop
265324:	84a4001c 	lh	a0,28(a1)
265328:	2c840001 	sltiu	a0,a0,1
26532c:	308400ff 	andi	a0,a0,0xff
265330:	10800004 	beqz	a0,0x265344
265334:	24a50008 	addiu	a1,a1,8
265338:	3c050037 	lui	a1,0x37
26533c:	10000001 	b	0x265344
265340:	24a5dab8 	addiu	a1,a1,-9544
265344:	10000007 	b	0x265364
265348:	00000000 	nop
26534c:	10800004 	beqz	a0,0x265360
265350:	3c050037 	lui	a1,0x37
265354:	3c050037 	lui	a1,0x37
265358:	10000002 	b	0x265364
26535c:	24a5dac4 	addiu	a1,a1,-9532
265360:	24a5dacc 	addiu	a1,a1,-9524
265364:	10000004 	b	0x265378
265368:	00000000 	nop
26536c:	0c049472 	jal	0x1251c8
265370:	00a02025 	move	a0,a1
265374:	00402825 	move	a1,v0
265378:	0c0004da 	jal	0x1368
26537c:	02002025 	move	a0,s0
265380:	8fb00000 	lw	s0,0(sp)
265384:	8fbf0004 	lw	ra,4(sp)
265388:	03e00008 	jr	ra
26538c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcDesiredObjectT<gcDesiredCustomAnimation, gcDesiredCustomAnimationHelper, gcEntityCustomAnimation>::GetText(char *) const
// Address: 0x00265288, Size: 264B
// Obj: gcAll_psp.obj

? cStrCat_charptr_constcharptr(?, void *);          /* extern */
void *gcDesiredObjectHelper__GetPrimaryText_gcDesiredObjectHelper__gcPrimarystatic(s32, s32, ?, s32); /* extern */

void voidgcDesiredObjectT_gcDesiredCustomAnimation_gcDesiredCustomAnimationHelper_gcEntityCustomAnimation___GetText_charptrconst(void *arg0, ? arg1) {
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 var_a1;
    s32 var_a2;
    void *temp_a2_2;
    void *var_a1_2;
    void *var_a1_3;

    temp_a2 = arg0->unk8;
    var_a1 = 0;
    temp_a3 = temp_a2 & 1;
    if (temp_a3 != 0) {
        var_a1 = 1;
    }
    if (var_a1 != 0) {
        var_a2 = 0;
    } else {
        var_a2 = ((temp_a2 != 0) & 0xFF) != 0;
    }
    if (var_a2 == 0) {
        temp_a1 = arg0->unkC;
        if (temp_a1 == 7) {
            temp_a0 = arg0->unk10;
            if (temp_a0 == 0) {
                var_a1_2 = NULL;
            } else {
                temp_a2_2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
                var_a1_2 = NULL;
                if ((temp_a2_2 != NULL) && (temp_a2_2->unk30 == temp_a0)) {
                    var_a1_2 = temp_a2_2;
                }
            }
            if (var_a1_2 != NULL) {
                var_a1_3 = var_a1_2 + 8;
                if ((var_a1_2->unk1C == 0) & 0xFF) {
                    var_a1_3 = (void *)0x36DAB8;
                }
            } else if (temp_a0 != 0) {
                var_a1_3 = (void *)0x36DAC4;
            } else {
                var_a1_3 = (void *)0x36DACC;
            }
        } else {
            var_a1_3 = gcDesiredObjectHelper__GetPrimaryText_gcDesiredObjectHelper__gcPrimarystatic(temp_a1, temp_a1, 7, temp_a3);
        }
        cStrCat_charptr_constcharptr(arg1, var_a1_3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
