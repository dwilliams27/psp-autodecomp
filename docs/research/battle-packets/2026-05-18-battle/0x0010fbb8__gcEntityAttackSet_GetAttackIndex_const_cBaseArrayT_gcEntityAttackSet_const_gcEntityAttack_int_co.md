# Battle Packet: `gcEntityAttackSet::GetAttackIndex(const cBaseArrayT<gcEntityAttackSet *> &, const gcEntityAttack *, int *, const gcEntityAttack *) static`

## Target

- Address: `0x0010fbb8`
- Size: `160` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntityAttackSet`
- Method family: `GetAttackIndex`
- Leaf: `True`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 24B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `24`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `24`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x0010f2f0` `untried` 888B `gcEntityAttackState::WriteAttack(cOutStream &, const cBaseArrayT<gcEntityAttackSet *> &, const gcEntityAttack *) const`

## Placement

- Canonical source: `src/gcEntityAttackSet.cpp`
- Header(s): `include/gcEntityAttackSet.h`
- Allowed source(s): `src/gcEntityAttackSet.cpp`
- Split-TU prefix: `src/gcEntityAttackSet_*.cpp`

## Class Header: `include/gcEntityAttackSet.h`

Not found.

## Matched Same-Class Neighbors

- `0x0010fa78` 88B `gcEntityAttackSet::Write(cFile &) const` — `src/gcEntityAttackSet.cpp`
- `0x0010fad0` 232B `gcEntityAttackSet::Read(cFile &, cMemPool *)` — `src/gcEntityAttackSet.cpp`
- `0x0025c6a8` 68B `gcEntityAttackSet::AssignCopy(const cBase *)` — `src/gcEntityAttackSet.cpp`
- `0x0025c6ec` 156B `gcEntityAttackSet::New(cMemPool *, cBase *) static` — `src/gcEntityAttackSet.cpp`
- `0x0025c788` 220B `gcEntityAttackSet::GetType(void) const` — `src/gcEntityAttackSet.cpp`

## Prior Failure Notes

### Note 1

- session `b3248322`

Best source is a fully decompiled nested cBaseArray search and compiles to the correct 160-byte size with the right outer-loop setup and branch-likely control flow. The remaining mismatch is 24/160 bytes, isolated to inner-loop register allocation/scheduling: expected keeps the inner array pointer in t5, inner index in t4, inner byte offset in t3, and inner size/load scratch in v1; SNC rotates these roles in the current compile, using v1 for the inner pointer, t5 for the index, t4 for the byte offset, and t3 for size/load scratch. Manual attempts included the original direct CSE-friendly form, volatile reloads of sets.mData, splitting the outer reload into a separate temporary, flipping the inner bounds test to produce the expected beqzl form, explicit zero materialization for outerIndex to get the target initial ori into t2, fixed-register local experiments, and flag probes with -Xsched=1, -Xmopt=0, and -Xxopt=0. The current source is the best clean shape found.

Permuter was run for 300 seconds from the 24-byte baseline: 4,488 candidates generated, 2,036 compiled, zero improvements, best stayed at 24 bytes. This is not a bnel divergence: branch-likely forms now match structurally, and the diff is a register-allocation/scheduling issue in the inner loop rather than beq/bne vs beql/bnel.

### Note 2

- session `42f34560`

Implemented the full nested cBaseArray search in proper C++ syntax, modeled after gcEntityAnimationConfigSet::GetAnimationIndex. The closest source compiles to the correct logic but a 156-byte function instead of the 160-byte target: SNC keeps the outer array data pointer in a register, fills the outer bounds-check branch delay slot with the first indexed address calculation, and avoids the target's extra reload/nop shape. I also tried forcing volatile loads and an empty memory barrier around the outer array test; that made the body much worse (113/160 bytes differed), so I reverted to the closer C++ source.

I attempted to run the permuter for this 4-byte size gap (`tools/permuter.py src/gcEntityAttackSet.cpp 0x0010fbb8 --time 300 --save-best`), but the tool failed before searching because it could not locate the target function in the multi-function compiled object. Root cause appears to be a register-allocation/scheduling difference for the outer loop, not a branch-likely-only divergence.


## Disassembly

```asm
10fbb8:	2408ffff 	li	t0,-1
10fbbc:	ace80000 	sw	t0,0(a3)
10fbc0:	acc80000 	sw	t0,0(a2)
10fbc4:	340a0000 	li	t2,0x0
10fbc8:	8c890000 	lw	t1,0(a0)
10fbcc:	34080000 	li	t0,0x0
10fbd0:	340b0000 	li	t3,0x0
10fbd4:	55200001 	bnezl	t1,0x10fbdc
10fbd8:	8d2bfffc 	lw	t3,-4(t1)
10fbdc:	014b582a 	slt	t3,t2,t3
10fbe0:	1160001b 	beqz	t3,0x10fc50
10fbe4:	00000000 	nop
10fbe8:	8c8b0000 	lw	t3,0(a0)
10fbec:	340c0000 	li	t4,0x0
10fbf0:	01685821 	addu	t3,t3,t0
10fbf4:	8d6d0000 	lw	t5,0(t3)
10fbf8:	340b0000 	li	t3,0x0
10fbfc:	25ad000c 	addiu	t5,t5,12
10fc00:	8da20000 	lw	v0,0(t5)
10fc04:	34030000 	li	v1,0x0
10fc08:	54400001 	bnezl	v0,0x10fc10
10fc0c:	8c43fffc 	lw	v1,-4(v0)
10fc10:	0183182a 	slt	v1,t4,v1
10fc14:	5060000c 	beqzl	v1,0x10fc48
10fc18:	254a0001 	addiu	t2,t2,1
10fc1c:	8da30000 	lw	v1,0(t5)
10fc20:	006b1821 	addu	v1,v1,t3
10fc24:	8c630000 	lw	v1,0(v1)
10fc28:	50650004 	beql	v1,a1,0x10fc3c
10fc2c:	acca0000 	sw	t2,0(a2)
10fc30:	258c0001 	addiu	t4,t4,1
10fc34:	1000fff3 	b	0x10fc04
10fc38:	256b0004 	addiu	t3,t3,4
10fc3c:	acec0000 	sw	t4,0(a3)
10fc40:	03e00008 	jr	ra
10fc44:	34020001 	li	v0,0x1
10fc48:	1000ffe1 	b	0x10fbd0
10fc4c:	25080004 	addiu	t0,t0,4
10fc50:	03e00008 	jr	ra
10fc54:	00001025 	move	v0,zero
```

## m2c Starting Point

```c
// Decompiled: gcEntityAttackSet::GetAttackIndex(const cBaseArrayT<gcEntityAttackSet *> &, const gcEntityAttack *, int *, const gcEntityAttack *) static
// Address: 0x0010fbb8, Size: 160B
// Obj: gcAll_psp.obj

s32 gcEntityAttackSet__GetAttackIndex_constcBaseArrayT_gcEntityAttackSetptr_ref_constgcEntityAttackptr_intptr_constgcEntityAttackptrstatic(void **arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    s32 var_t0;
    s32 var_t2;
    s32 var_t3;
    s32 var_t3_2;
    s32 var_t4;
    s32 var_v1;
    void *temp_t1;
    void *temp_t5;
    void *temp_v0;

    *arg3 = -1;
    *arg2 = -1;
    var_t2 = 0;
    temp_t1 = *arg0;
    var_t0 = 0;
loop_1:
    var_t3 = 0;
    if (temp_t1 != NULL) {
        var_t3 = temp_t1->unk-4;
    }
    if (var_t2 < var_t3) {
        var_t4 = 0;
        temp_t5 = *(*arg0 + var_t0);
        var_t3_2 = 0;
        temp_v0 = temp_t5->unkC;
loop_5:
        var_v1 = 0;
        if (temp_v0 != NULL) {
            var_v1 = temp_v0->unk-4;
        }
        if (var_t4 >= var_v1) {
            var_t2 += 1;
            var_t0 += 4;
            goto loop_1;
        }
        if (*(temp_t5->unkC + var_t3_2) == arg1) {
            *arg2 = var_t2;
            *arg3 = var_t4;
            return 1;
        }
        var_t4 += 1;
        var_t3_2 += 4;
        goto loop_5;
    }
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
