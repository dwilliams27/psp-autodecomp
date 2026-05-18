# Battle Packet: `gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const`

## Target

- Address: `0x001101a8`
- Size: `112` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntityControllerTemplate`
- Method family: `FindAttackSet`
- Leaf: `True`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; has failure snapshot; near miss 4B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `4`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x00110218` `untried` 264B `gcEntityControllerTemplate::GetAttack(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const`

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

- session `b07f7676`; src `src/gcEntityControllerTemplate.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/b07f7676/001101a8__agent_self_reported_failure__src_gcEntityControllerTemplate.cpp`

FindAttackSet stuck at 4/112-byte diff: pure SNC scheduler decision swapping `lw a0,0(sp)` (h.mIndex spill load) and `li a1,0` (offset init) at offsets 0x10/0x14. Baseline emits `li v0,0; lw a0,0(sp); li a1,0; li a3,0` while ours emits `li v0,0; li a1,0; lw a0,0(sp); li a3,0`. Tried: `int hidx=h.mIndex` local (got 5/8 worse), reordered declarations, moved offset assignment around, ran permuter for 180s (5136 candidates, 0 improvements). Same 4-byte diff affects FindAnimationSet at 0x00110030 identically -- both are SNC -O2 -Xsched=2 scheduler artifacts that source restructuring cannot influence. Per CLAUDE.md does NOT qualify as unmatchable (rule requires bnel/beql diff, this is li/lw scheduling).


## Disassembly

```asm
1101a8:	27bdfff0 	addiu	sp,sp,-16
1101ac:	afa50000 	sw	a1,0(sp)
1101b0:	8c86001c 	lw	a2,28(a0)
1101b4:	34020000 	li	v0,0x0
1101b8:	8fa40000 	lw	a0,0(sp)
1101bc:	34050000 	li	a1,0x0
1101c0:	34070000 	li	a3,0x0
1101c4:	54c00001 	bnezl	a2,0x1101cc
1101c8:	8cc7fffc 	lw	a3,-4(a2)
1101cc:	0047382a 	slt	a3,v0,a3
1101d0:	10e0000e 	beqz	a3,0x11020c
1101d4:	00c53821 	addu	a3,a2,a1
1101d8:	8ce70000 	lw	a3,0(a3)
1101dc:	24e70008 	addiu	a3,a3,8
1101e0:	8ce70000 	lw	a3,0(a3)
1101e4:	00e43826 	xor	a3,a3,a0
1101e8:	2ce70001 	sltiu	a3,a3,1
1101ec:	30e700ff 	andi	a3,a3,0xff
1101f0:	14e00004 	bnez	a3,0x110204
1101f4:	00000000 	nop
1101f8:	24420001 	addiu	v0,v0,1
1101fc:	1000fff0 	b	0x1101c0
110200:	24a50004 	addiu	a1,a1,4
110204:	03e00008 	jr	ra
110208:	27bd0010 	addiu	sp,sp,16
11020c:	2402ffff 	li	v0,-1
110210:	03e00008 	jr	ra
110214:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const
// Address: 0x001101a8, Size: 112B
// Obj: gcAll_psp.obj

s32 gcEntityControllerTemplate__FindAttackSet_cHandleT_gcEnumeration_const(void *arg0, s32 arg1) {
    s32 sp0;
    s32 var_a1;
    s32 var_a3;
    s32 var_v0;
    void *temp_a2;

    sp0 = arg1;
    temp_a2 = arg0->unk1C;
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
