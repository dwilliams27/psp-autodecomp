# Battle Packet: `gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const`

## Target

- Address: `0x00110030`
- Size: `112` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntityControllerTemplate`
- Method family: `FindAnimationSet`
- Leaf: `True`

## Queue Metadata

- score: `163`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 4B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `4`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x001100a0` `untried` 264B `gcEntityControllerTemplate::GetAnimation(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const`
- `0x0032ed7c` `untried` 900B `gcValEntityHasAnimation::Evaluate(void) const`

## Placement

- Canonical source: `src/gcEntityControllerTemplate.cpp`
- Header(s): `include/gcEntityControllerTemplate.h`
- Allowed source(s): `src/gcEntityControllerTemplate.cpp`
- Split-TU prefix: `src/gcEntityControllerTemplate_*.cpp`

## Class Header: `include/gcEntityControllerTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x0010fc58` 180B `gcEntityControllerTemplate::Write(cFile &) const` — `src/gcEntityControllerTemplate.cpp`
- `0x0010fed8` 64B `gcEntityControllerTemplate::Reset(cMemPool *, bool)` — `src/gcEntityControllerTemplate.cpp`
- `0x0025cbac` 152B `gcEntityControllerTemplate::GetType(void) const` — `src/gcEntityControllerTemplate.cpp`
- `0x0025d4c8` 204B `gcEntityControllerTemplate::~gcEntityControllerTemplate(void)` — `src/gcEntityControllerTemplate.cpp`

## Prior Failure Notes

### Note 1

- session `dcf89d82`

Closest result is a 4-byte mismatch in the loop setup only. Expected has `lw a0,0(sp)` at offset 0x10 followed by `li a1,0` at 0x14; generated code has those two independent instructions swapped. The loop body, branch-likely size load, XOR/sltiu/andi boolean normalization, returns, and stack frame all match byte-for-byte. I left the source using an explicit `((eh->mIndex ^ h.mIndex) == 0) & 0xFF` expression, which preserves the expected instruction sequence and avoids emitting the unused cHandleT::operator== linkonce helper.

Tried direct integer comparison (shrinks to 100B and loses bool normalization), explicit target local (changes register allocation to target in a1 and offset in a0), memory/input/clobber empty asm barriers (worsen prologue ordering or store/load ordering), sched=1 around the function (no change), and a 300s permuter run with --save-best. The permuter generated 8880 candidates, compiled 3507, and found zero improvements from the 4-byte baseline. This still does not qualify as unmatchable_bnel because the diff is not a branch-likely divergence; it appears to be the same independent `lw` vs `li` scheduler-order divergence seen in the sibling FindAnimationSet/FindAttackSet functions.

### Note 2

- session `4a242600`

gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const. 4-byte diff that resists every permutation tried. Function body matches perfectly except for instructions at offset 0x10/0x14: expected has `lw a0,0(sp)` at 0x10 and `li a1,0x0` at 0x14; mine has them swapped (`li a1,0x0` at 0x10, `lw a0,0(sp)` at 0x14). These are two independent setup instructions before the loop — the SNC scheduler picks a different order. Body and loop are byte-identical.

Approaches tried: (1) original while-loop structure (4-byte diff). (2) Introducing local copy `cHandleT hh = h;` to force re-spill ordering — got worse (5-byte diff). (3) Combined `for (int i=0, offset=0; ; i++, offset+=4)` loop init — got much worse (10-byte diff). (4) Goto-based loop matching m2c output — also 10 bytes. (5) Permuter ran 300s @ 19 cand/sec across 5641 candidates with --sched=2: zero improvement. (6) Permuter --sched=1: zero improvement.

The sibling functions in the same family (gcPartialBodyControllerTemplate::FindAnimationSet @ 0x0013ff5c, FindAttackSet @ 0x001400d4) both have this IDENTICAL 4-byte scheduler divergence and are both also marked Failed in the DB — strongly suggests this is a class of compiler-version scheduling divergence, not a source-restructuring problem. Diff is NOT bnel-related so it does not qualify as `unmatchable_bnel` per the strict criteria. Root cause likely: SNC scheduler heuristic difference between our SNC build and the original game's SNC for ordering an early `lw` (parameter spill reload) vs an `li 0` immediate when both are independent. Probably needs either a different compiler flag combination not yet identified, or matching the originals requires a source pattern that differs in some non-obvious way (e.g. data is loaded from a different expression, or the loop entry is structured to put the lw at a specific position). Leaving the C source in place — the matched part is real decompiled C++.


## Disassembly

```asm
110030:	27bdfff0 	addiu	sp,sp,-16
110034:	afa50000 	sw	a1,0(sp)
110038:	8c860008 	lw	a2,8(a0)
11003c:	34020000 	li	v0,0x0
110040:	8fa40000 	lw	a0,0(sp)
110044:	34050000 	li	a1,0x0
110048:	34070000 	li	a3,0x0
11004c:	54c00001 	bnezl	a2,0x110054
110050:	8cc7fffc 	lw	a3,-4(a2)
110054:	0047382a 	slt	a3,v0,a3
110058:	10e0000e 	beqz	a3,0x110094
11005c:	00c53821 	addu	a3,a2,a1
110060:	8ce70000 	lw	a3,0(a3)
110064:	24e70008 	addiu	a3,a3,8
110068:	8ce70000 	lw	a3,0(a3)
11006c:	00e43826 	xor	a3,a3,a0
110070:	2ce70001 	sltiu	a3,a3,1
110074:	30e700ff 	andi	a3,a3,0xff
110078:	14e00004 	bnez	a3,0x11008c
11007c:	00000000 	nop
110080:	24420001 	addiu	v0,v0,1
110084:	1000fff0 	b	0x110048
110088:	24a50004 	addiu	a1,a1,4
11008c:	03e00008 	jr	ra
110090:	27bd0010 	addiu	sp,sp,16
110094:	2402ffff 	li	v0,-1
110098:	03e00008 	jr	ra
11009c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const
// Address: 0x00110030, Size: 112B
// Obj: gcAll_psp.obj

s32 gcEntityControllerTemplate__FindAnimationSet_cHandleT_gcEnumeration_const(void *arg0, s32 arg1) {
    s32 sp0;
    s32 var_a1;
    s32 var_a3;
    s32 var_v0;
    void *temp_a2;

    sp0 = arg1;
    temp_a2 = arg0->unk8;
    var_v0 = 0;
    var_a1 = 0;
loop_1:
    var_a3 = 0;
    if (temp_a2 != NULL) {
        var_a3 = temp_a2->unk-4;
    }
    if (var_v0 < var_a3) {
        if (!(((*(temp_a2 + var_a1))->unk8 == sp0) & 0xFF)) {
            var_v0 += 1;
            var_a1 += 4;
            goto loop_1;
        }
        return var_v0;
    }
    return -1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
