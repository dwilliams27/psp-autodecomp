# Battle Packet: `gcTableColumnShort::SetSize(int)`

## Target

- Address: `0x0027169c`
- Size: `396` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcTableColumnShort`
- Method family: `SetSize`
- Leaf: `False`

## Queue Metadata

- score: `183`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 2B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `2`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

## Placement

- Canonical source: `src/gcTableColumnShort.cpp`
- Header(s): `include/gcTableColumnShort.h`
- Allowed source(s): `src/gcTableColumnShort.cpp`
- Split-TU prefix: `src/gcTableColumnShort_*.cpp`

## Class Header: `include/gcTableColumnShort.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012ad9c` 152B `gcTableColumnShort::Write(cFile &) const` — `src/gcTableColumnShort.cpp`
- `0x0012ae34` 200B `gcTableColumnShort::Read(cFile &, cMemPool *)` — `src/gcTableColumnShort.cpp`
- `0x00271504` 52B `gcTableColumnShort::AssignCopy(const cBase *)` — `src/gcTableColumnShort.cpp`
- `0x00271538` 136B `gcTableColumnShort::New(cMemPool *, cBase *) static` — `src/gcTableColumnShort.cpp`
- `0x002715c0` 220B `gcTableColumnShort::GetType(void) const` — `src/gcTableColumnShort.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEntityAnimationConfigSet::SetSize(int)`

- Address: `0x0025aa80`
- Size: `28` bytes
- Source: `src/gcEntityAnimationConfigSet.cpp`

```cpp
//   0x0025aa80 gcEntityAnimationConfigSet::SetSize(int)
//   0x0025a8c4 gcEntityAnimationConfigSet::AssignCopy(const cBase *)
//   0x0010e270 gcEntityAnimationConfigSet::Write(cFile &) const
//   0x0025ad30 gcEntityAnimationConfigSet::~gcEntityAnimationConfigSet(void)
//   0x0025a908 gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```

### Exemplar 2: `gcEntityAttackSet::SetSize(int)`

- Address: `0x0025c864`
- Size: `28` bytes
- Source: `src/gcEntityAttackSet.cpp`

```cpp
//   0x0025c864 gcEntityAttackSet::SetSize(int)
//   0x0025c6a8 gcEntityAttackSet::AssignCopy(const cBase *)
//   0x0010fa78 gcEntityAttackSet::Write(cFile &) const
//   0x0025cb14 gcEntityAttackSet::~gcEntityAttackSet(void)
//   0x0025c6ec gcEntityAttackSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```


## Prior Failure Notes

### Note 1

- session `e4c4e6e1`; src `src/gcTableColumnShort.cpp`; snapshot `logs/failure_snapshots/match_20260517_210450/e4c4e6e1/0027169c__agent_self_reported_failure__src_gcTableColumnShort.cpp`

Restored gcTableColumnShort::SetSize(int) into canonical src/gcTableColumnShort.cpp from the prior failure snapshot (it had been dropped from the class). Best build is 396B with the same 2/396-byte diff documented in prior session 35213a14: target emits `addu a2,s3,a1` for the initial copy-loop dst = newData + offset where the offset (a1) has just been set to 0 in the beqz delay slot, while SNC folds our `addu(s3, 0)` to a plain `move a2, s3` (encoding 02653021 vs 02603025). All other 394 bytes match — prologue, alloc path, both `li a0,0` duplicates from the bnez/bgtz join, copy-loop body (move a3,a2 + slt + lh + sh + addiu a0/a1/a2), free-count empty loop, and free tail all align exactly.

New approaches tried this session beyond prior agents': (1) `__asm__ volatile("" : "+r"(offset))` no-op barrier on offset before `dst = newData + offset` — SNC still constant-folds the add (2-byte diff unchanged); (2) `__asm__ volatile("ori %0,$0,0" : "=r"(offset))` to materialize 0 through an opaque init — broke scheduling badly (37B diff, scheduler can't see the constant and emits a real `ori a1,$0,0` outside the delay slot); (3) inline-asm-only addu (`__asm__("or %0,$0,$0\n\taddu %1,%2,%0" : "=&r"(offset),"=r"(dst) : "r"(newData))`) — much worse (182B diff, hostile to surrounding scheduling) and against the asm policy anyway; (4) `#pragma control sched=1` for just this method (with sched=2 restored before sibling methods) — 37B diff, lost the entire delay-slot scheduling. Re-ran tools/permuter.py for 300s/2 workers from the 2B baseline: 2776 candidates, 1065 compiled, zero improvements (matches the prior agent's 4488/2230/0).

Root cause is an SNC peephole/constant-propagation choice on `addu(reg, 0_reg)`: the original SNC keeps the addu (probably because its scheduler shoved `li a1,0` into the beqz delay slot BEFORE constant-propagation could fold it), while our SNC propagates the 0 through and rewrites to `or a2,s3,$0` (move). The source has no leverage — any C/C++ form that yields offset==0 at the dst init is foldable; any form that hides the 0 (volatile, asm, function call) inserts instructions or load/stores not in the target and blows up the diff. Diff is 2 bytes but on different opcodes (move vs addu), NOT bnel/beql, so per docs/decisions/006 this stays `failed` rather than `unmatchable_bnel`. Closest matching source is the current src/gcTableColumnShort.cpp; the offending instruction is at offset 0x228 in the compiled .o (target byte 02653021, our byte 02603025).

### Note 2

- session `35213a14`; src `src/gcTableColumnShort.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/35213a14/0027169c__agent_self_reported_failure__src_gcTableColumnShort.cpp`

Implemented gcTableColumnShort::SetSize(int) as canonical C++ in src/gcTableColumnShort.cpp. The current closest build is 396B and differs by only 2 bytes: target has `addu a2,s3,a1` at the copy-loop setup with a1 known zero, while SNC emits the equivalent `move a2,s3`. Allocation setup, null/size guards, copy loop body, free-count empty loop, and free tail otherwise line up with the target. Verified with make build/src/gcTableColumnShort.cpp.o and compare_func.py; unrelated pre-existing overload mismatches remain in this TU, but SetSize is the only target worked here.

Manual variants tried included size <= 0 vs size > 0 allocation branch shape, explicit gotos, splitting bytes calculation to preserve allocation registers, pointer-as-short* vs char* copy-loop setup, declaring offset inside vs outside the loop, recomputing dst from newData+offset each iteration, keeping separate store/dst locals, memory and register asm barriers on offset, unsigned offset, forced register-a1 offset, and sched=1 pragma. The only near-match forms either emit `move a2,s3` (2-byte diff) or get the desired `addu a2,s3,a1` but collapse the loop pointer/value registers and become much worse. Ran `python3 tools/permuter.py src/gcTableColumnShort.cpp 0x0027169c --time 300 --save-best`: 4488 candidates generated, 2230 compiled, no improvement from the 2-byte baseline.

### Note 3

- session `a3db9f02`

Implemented `gcTableColumnShort::SetSize(int)` in canonical C++ with the same pool-alloc / copy / free structure as the target: masked old count from `mValues.mData[-1]`, allocated via `cMemPool::GetPoolFromPtr((char*)this + 8)` and alloc-table slot `+0x28`, copied old shorts or zero-filled new slots, ran the empty old-count loop the original emits, then freed the old header through alloc-table slot `+0x30`. The best build is now extremely close to the target: the alloc path matches exactly, the null/size guards match, and the free tail matches exactly. The remaining `objdump` diff is in the copy-loop setup and symbol padding: current code emits `move a2,s3` where target has `addu a2,s3,a1` (with `a1` known-zero), and the symbol ends at 392B instead of 396B because the target keeps one extra 4-byte instruction / trailing padding in this path.

Tried several manual restructurings around the copy loop: pointer-as-`short*` vs `char*`, recomputing `dst` from `newData + offset`, keeping separate `slot` and `dst` locals, splitting `size*2+4` into `bytes = size + size`, duplicating the zero-init path to reproduce the extra `li a0,0`, and adding/removing a zero-cost asm barrier on `offset` to stop constant-folding. One restructuring made the function substantially worse and was reverted. I also tried the project permuter (`python3 tools/permuter.py src/gcTableColumnShort.cpp 0x0027169c --time 60 --save-best`), but it failed immediately because it could not isolate the target symbol from this multi-function TU (`Could not locate target function in compiled .o. Found 1828B .text but expected 396B function`). Root cause looks like last-mile SNC codegen around pointer materialization in the copy loop, not a misunderstanding of the function logic.


## Disassembly

```asm
27169c:	27bdffe0 	addiu	sp,sp,-32
2716a0:	afb00000 	sw	s0,0(sp)
2716a4:	00808025 	move	s0,a0
2716a8:	8e040008 	lw	a0,8(s0)
2716ac:	afb20008 	sw	s2,8(sp)
2716b0:	34120000 	li	s2,0x0
2716b4:	afb10004 	sw	s1,4(sp)
2716b8:	afb3000c 	sw	s3,12(sp)
2716bc:	afbf0010 	sw	ra,16(sp)
2716c0:	10800005 	beqz	a0,0x2716d8
2716c4:	00a08825 	move	s1,a1
2716c8:	8c92fffc 	lw	s2,-4(a0)
2716cc:	3c044000 	lui	a0,0x4000
2716d0:	2484ffff 	addiu	a0,a0,-1
2716d4:	02449024 	and	s2,s2,a0
2716d8:	1251004c 	beq	s2,s1,0x27180c
2716dc:	00000000 	nop
2716e0:	1e200003 	bgtz	s1,0x2716f0
2716e4:	00000000 	nop
2716e8:	10000012 	b	0x271734
2716ec:	34130000 	li	s3,0x0
2716f0:	0c0011e8 	jal	0x47a0
2716f4:	26040008 	addiu	a0,s0,8
2716f8:	8c440024 	lw	a0,36(v0)
2716fc:	02312821 	addu	a1,s1,s1
271700:	8c86001c 	lw	a2,28(a0)
271704:	24a50004 	addiu	a1,a1,4
271708:	24c60028 	addiu	a2,a2,40
27170c:	84c70000 	lh	a3,0(a2)
271710:	8cc90004 	lw	t1,4(a2)
271714:	00872021 	addu	a0,a0,a3
271718:	3c070037 	lui	a3,0x37
27171c:	34060002 	li	a2,0x2
271720:	24e7da98 	addiu	a3,a3,-9576
271724:	0120f809 	jalr	t1
271728:	34080112 	li	t0,0x112
27172c:	ac510000 	sw	s1,0(v0)
271730:	24530004 	addiu	s3,v0,4
271734:	16600004 	bnez	s3,0x271748
271738:	34040000 	li	a0,0x0
27173c:	1e200033 	bgtz	s1,0x27180c
271740:	00000000 	nop
271744:	34040000 	li	a0,0x0
271748:	0091282a 	slt	a1,a0,s1
27174c:	10a00010 	beqz	a1,0x271790
271750:	34050000 	li	a1,0x0
271754:	02653021 	addu	a2,s3,a1
271758:	00c03825 	move	a3,a2
27175c:	10e00007 	beqz	a3,0x27177c
271760:	0092482a 	slt	t1,a0,s2
271764:	11200004 	beqz	t1,0x271778
271768:	34080000 	li	t0,0x0
27176c:	8e080008 	lw	t0,8(s0)
271770:	01054021 	addu	t0,t0,a1
271774:	85080000 	lh	t0,0(t0)
271778:	a4e80000 	sh	t0,0(a3)
27177c:	24840001 	addiu	a0,a0,1
271780:	24a50002 	addiu	a1,a1,2
271784:	0091382a 	slt	a3,a0,s1
271788:	14e0fff3 	bnez	a3,0x271758
27178c:	24c60002 	addiu	a2,a2,2
271790:	8e110008 	lw	s1,8(s0)
271794:	12200005 	beqz	s1,0x2717ac
271798:	34040000 	li	a0,0x0
27179c:	8e24fffc 	lw	a0,-4(s1)
2717a0:	3c054000 	lui	a1,0x4000
2717a4:	24a5ffff 	addiu	a1,a1,-1
2717a8:	00852024 	and	a0,a0,a1
2717ac:	34050000 	li	a1,0x0
2717b0:	00a4302a 	slt	a2,a1,a0
2717b4:	10c00005 	beqz	a2,0x2717cc
2717b8:	00000000 	nop
2717bc:	24a50001 	addiu	a1,a1,1
2717c0:	00a4302a 	slt	a2,a1,a0
2717c4:	14c0fffe 	bnez	a2,0x2717c0
2717c8:	24a50001 	addiu	a1,a1,1
2717cc:	1220000e 	beqz	s1,0x271808
2717d0:	2631fffc 	addiu	s1,s1,-4
2717d4:	5220000c 	beqzl	s1,0x271808
2717d8:	ae000008 	sw	zero,8(s0)
2717dc:	0c0011e8 	jal	0x47a0
2717e0:	02202025 	move	a0,s1
2717e4:	8c440024 	lw	a0,36(v0)
2717e8:	02202825 	move	a1,s1
2717ec:	8c86001c 	lw	a2,28(a0)
2717f0:	24c60030 	addiu	a2,a2,48
2717f4:	84c70000 	lh	a3,0(a2)
2717f8:	8cc60004 	lw	a2,4(a2)
2717fc:	00c0f809 	jalr	a2
271800:	00872021 	addu	a0,a0,a3
271804:	ae000008 	sw	zero,8(s0)
271808:	ae130008 	sw	s3,8(s0)
27180c:	8fb00000 	lw	s0,0(sp)
271810:	8fb10004 	lw	s1,4(sp)
271814:	8fb20008 	lw	s2,8(sp)
271818:	8fb3000c 	lw	s3,12(sp)
27181c:	8fbf0010 	lw	ra,16(sp)
271820:	03e00008 	jr	ra
271824:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcTableColumnShort::SetSize(int)
// Address: 0x0027169c, Size: 396B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */

void gcTableColumnShort__SetSize_int(void *arg0, s32 arg1) {
    ? (*temp_a2_3)(void *, void *, ?, s16);
    s16 *temp_a0;
    s16 *temp_s1;
    s16 *var_a2;
    s16 *var_s3;
    s16 temp_a3;
    s16 var_t0;
    s32 *temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2_2;
    s32 var_s2;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_2;
    void *temp_s1_2;

    temp_a0 = arg0->unk8;
    var_s2 = 0;
    if (temp_a0 != NULL) {
        var_s2 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    if (var_s2 != arg1) {
        if (arg1 <= 0) {
            var_s3 = NULL;
        } else {
            temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0 + 8)->unk24;
            temp_a2 = temp_a0_2->unk1C;
            temp_v0 = (temp_a2 + 0x28)->unk4(temp_a0_2 + temp_a2->unk28, (arg1 * 2) + 4, 2, 0x36DA98);
            *temp_v0 = arg1;
            var_s3 = temp_v0 + 4;
        }
        var_a0 = 0;
        if (var_s3 == NULL) {
            if (arg1 <= 0) {
                var_a0 = 0;
                goto block_9;
            }
        } else {
block_9:
            var_a1 = 0;
            if (arg1 > 0) {
                var_a2 = var_s3;
                do {
                    if (var_a2 != NULL) {
                        var_t0 = 0;
                        if (var_a0 < var_s2) {
                            var_t0 = *(arg0->unk8 + var_a1);
                        }
                        *var_a2 = var_t0;
                    }
                    var_a0 += 1;
                    var_a1 += 2;
                    var_a2 += 2;
                } while (var_a0 < arg1);
            }
            temp_s1 = arg0->unk8;
            var_a0_2 = 0;
            if (temp_s1 != NULL) {
                var_a0_2 = temp_s1->unk-4 & 0x3FFFFFFF;
            }
            var_a1_2 = 0;
            var_a2_2 = var_a0_2 > 0;
            if (var_a2_2 != 0) {
                var_a1_2 = 1;
                do {
                    var_a2_2 = var_a1_2 < var_a0_2;
                    var_a1_2 += 1;
                } while (var_a2_2 != 0);
            }
            temp_s1_2 = temp_s1 - 4;
            if (temp_s1 != NULL) {
                if (temp_s1_2 != NULL) {
                    temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s1_2, var_a1_2, var_a2_2)->unk24;
                    temp_a2_2 = temp_a0_3->unk1C;
                    temp_a3 = temp_a2_2->unk30;
                    temp_a2_3 = (temp_a2_2 + 0x30)->unk4;
                    temp_a2_3(temp_a0_3 + temp_a3, temp_s1_2, temp_a2_3, temp_a3);
                }
                arg0->unk8 = NULL;
            }
            arg0->unk8 = var_s3;
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
