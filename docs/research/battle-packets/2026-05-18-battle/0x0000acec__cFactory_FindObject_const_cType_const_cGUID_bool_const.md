# Battle Packet: `cFactory::FindObject(const cType *, const cGUID &, bool) const`

## Target

- Address: `0x0000acec`
- Size: `648` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cFactory`
- Method family: `FindObject`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; correct function size; codegen-specific diagnosis`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `16`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0000acec` `failed` 648B `cFactory::FindObject(const cType *, const cGUID &, bool) const`
- `0x0000b2dc` `untried` 796B `cFactory::FindGroup(const cType *)`
- `0x0000c430` `untried` 116B `cGroup::FindObject(const cGUID &) const`

Callers:
- `0x000093ec` `untried` 828B `cHandle::Read(cReadBlock &, cMemPool *)`
- `0x0000acec` `failed` 648B `cFactory::FindObject(const cType *, const cGUID &, bool) const`
- `0x00169548` `untried` 1324B `gcBackgroundLoader::LoadObjects(int, gcMap::cObjectLoad *) static`

## Placement

- Canonical source: `src/cFactory.cpp`
- Header(s): `include/cFactory.h`
- Allowed source(s): `src/cFactory.cpp`
- Split-TU prefix: `src/cFactory_*.cpp`

## Class Header: `include/cFactory.h`

Not found.

## Matched Same-Class Neighbors

- `0x0000ab4c` 76B `cFactory::Write(cFile &) const` — `src/cFactory.cpp`
- `0x0000ab98` 188B `cFactory::Read(cFile &, cMemPool *)` — `src/cFactory.cpp`
- `0x0000ac54` 152B `cFactory::Load(void)` — `src/cFactory.cpp`
- `0x0000b114` 296B `cFactory::CreateObject(const cType *, const cGUID &, bool, unsigned int, bool)` — `src/cFactory.cpp`
- `0x0000b23c` 160B `cFactory::CopyObject(const cObject *, const cGUID &)` — `src/cFactory.cpp`

## Prior Failure Notes

### Note 1

- session `c5a1ae9a`; src `src/cFactory.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/c5a1ae9a/0000acec__agent_self_reported_failure__src_cFactory.cpp`

Started from the existing near-match in src/cFactory.cpp and verified the current baseline: 16/648 bytes differ, with all mismatches in the final type-check/parent-advance control-flow region. Tested multiple tail restructurings not preserved in the final file: direct fallthrough if (!isFactory), short-circuit && parent advance, positive if/else, sched=1 pragma, default-zero isFactory, while-loop type walk, pointer-valued isFactory, zero-instruction memory barrier in the true branch, and short-circuit OR forms. The OR shape could produce the desired beqzl+delay-slot parent load in one intermediate build, but it either shrank the function to 644B or introduced extra flag tests and regressed the byte diff; the stable best remained the original 16-byte baseline.

Final verified disassembly still differs at build/src/cFactory.cpp.o offsets 0x720-0x738: our SNC emits an extra branch-to-join from loop failure, then a bnez-first parent advance (bnez a0; nop; lw s1,0(s1)), while expected/build/func/0000acec.o has li a0,0 followed by beqzl a0 with lw s1,0(s1) in the delay slot and an unconditional branch to the recurse block. Ran python3 tools/permuter.py src/cFactory.cpp 0x0000acec --time 600 --save-best; it generated 17040 candidates, compiled 6146, and found zero improvement from 16 bytes. Not marking unmatchable_bnel because the diff is 16 bytes and is a broader branch-layout issue, not the narrow <=8 byte bnel/beq-likely divergence criterion.

### Note 2

- session `79ccb4ea`

Implemented a near-match C++ version in src/cFactory.cpp and got the function down to a stable 16-byte diff at the correct 648-byte size. The successful pieces are the prologue, static cFactory type initialization chain, virtual GetType dispatch, and the recursive parent-factory search setup. The remaining mismatch is in the final post-type-check control-flow region: our SNC output still emits a different branch layout than the target around the isFactory test and parent advance, specifically where the target uses the sequence at expected/build/func/0000acec.o offsets 0x220-0x238 (`beqzl` + delay-slot load, then `b` to the recurse block) and our build keeps collapsing that into a `bnez`-first shape.

Tried multiple manual restructurings focused on that branch: nested `if`/`else`, shared `goto recurse_parent`, explicit `advance_factory` labels, split `if (isFactory == 0)` tests, and a flattened parent-walk fallthrough. One alternate rewrite regressed the function to 636B and was reverted; the best retained version remains 16 bytes off. Ran `python3 tools/permuter.py src/cFactory.cpp 0x0000acec --time 300 --save-best`; it generated 9072 candidates, compiled 3200, and found zero improvement from the 16-byte baseline. The next agent should start from the current source in src/cFactory.cpp and focus only on reproducing the target block shape at the end of the type-check loop, using expected/build/func/0000acec.o as the reference.


## Disassembly

```asm
acec:	27bdffd0 	addiu	sp,sp,-48
acf0:	afb1000c 	sw	s1,12(sp)
acf4:	afb20010 	sw	s2,16(sp)
acf8:	00808825 	move	s1,a0
acfc:	00c09025 	move	s2,a2
ad00:	afa50004 	sw	a1,4(sp)
ad04:	afb00008 	sw	s0,8(sp)
ad08:	afb30014 	sw	s3,20(sp)
ad0c:	afb40018 	sw	s4,24(sp)
ad10:	afb5001c 	sw	s5,28(sp)
ad14:	afb60020 	sw	s6,32(sp)
ad18:	afb70024 	sw	s7,36(sp)
ad1c:	afbe0028 	sw	s8,40(sp)
ad20:	afbf002c 	sw	ra,44(sp)
ad24:	0c002cb7 	jal	0xb2dc
ad28:	30f000ff 	andi	s0,a3,0xff
ad2c:	02404025 	move	t0,s2
ad30:	00402025 	move	a0,v0
ad34:	10800005 	beqz	a0,0xad4c
ad38:	afa80000 	sw	t0,0(sp)
ad3c:	0c00310c 	jal	0xc430
ad40:	01002825 	move	a1,t0
ad44:	14400015 	bnez	v0,0xad9c
ad48:	00000000 	nop
ad4c:	12000006 	beqz	s0,0xad68
ad50:	00000000 	nop
ad54:	8e310000 	lw	s1,0(s1)
ad58:	1620001c 	bnez	s1,0xadcc
ad5c:	3c1e0037 	lui	s8,0x37
ad60:	10000071 	b	0xaf28
ad64:	34110000 	li	s1,0x0
ad68:	00001025 	move	v0,zero
ad6c:	8fb00008 	lw	s0,8(sp)
ad70:	8fb1000c 	lw	s1,12(sp)
ad74:	8fb20010 	lw	s2,16(sp)
ad78:	8fb30014 	lw	s3,20(sp)
ad7c:	8fb40018 	lw	s4,24(sp)
ad80:	8fb5001c 	lw	s5,28(sp)
ad84:	8fb60020 	lw	s6,32(sp)
ad88:	8fb70024 	lw	s7,36(sp)
ad8c:	8fbe0028 	lw	s8,40(sp)
ad90:	8fbf002c 	lw	ra,44(sp)
ad94:	03e00008 	jr	ra
ad98:	27bd0030 	addiu	sp,sp,48
ad9c:	8fb00008 	lw	s0,8(sp)
ada0:	8fb1000c 	lw	s1,12(sp)
ada4:	8fb20010 	lw	s2,16(sp)
ada8:	8fb30014 	lw	s3,20(sp)
adac:	8fb40018 	lw	s4,24(sp)
adb0:	8fb5001c 	lw	s5,28(sp)
adb4:	8fb60020 	lw	s6,32(sp)
adb8:	8fb70024 	lw	s7,36(sp)
adbc:	8fbe0028 	lw	s8,40(sp)
adc0:	8fbf002c 	lw	ra,44(sp)
adc4:	03e00008 	jr	ra
adc8:	27bd0030 	addiu	sp,sp,48
adcc:	3c170037 	lui	s7,0x37
add0:	3c16001c 	lui	s6,0x1c
add4:	27dec830 	addiu	s8,s8,-14288
add8:	26f7c838 	addiu	s7,s7,-14280
addc:	26d63c58 	addiu	s6,s6,15448
ade0:	3c150004 	lui	s5,0x4
ade4:	3c140004 	lui	s4,0x4
ade8:	3c130004 	lui	s3,0x4
adec:	3c120004 	lui	s2,0x4
adf0:	8ea40c90 	lw	a0,3216(s5)
adf4:	54800032 	bnezl	a0,0xaec0
adf8:	8e240004 	lw	a0,4(s1)
adfc:	8e8485e4 	lw	a0,-31260(s4)
ae00:	54800025 	bnezl	a0,0xae98
ae04:	8e8785e4 	lw	a3,-31260(s4)
ae08:	8e6485e0 	lw	a0,-31264(s3)
ae0c:	54800018 	bnezl	a0,0xae70
ae10:	8e6785e0 	lw	a3,-31264(s3)
ae14:	8e4485dc 	lw	a0,-31268(s2)
ae18:	1480000a 	bnez	a0,0xae44
ae1c:	03c02025 	move	a0,s8
ae20:	02e02825 	move	a1,s7
ae24:	34060001 	li	a2,0x1
ae28:	00003825 	move	a3,zero
ae2c:	00004025 	move	t0,zero
ae30:	00004825 	move	t1,zero
ae34:	00005025 	move	t2,zero
ae38:	0c001d4d 	jal	0x7534
ae3c:	00005825 	move	t3,zero
ae40:	ae4285dc 	sw	v0,-31268(s2)
ae44:	8e4785dc 	lw	a3,-31268(s2)
ae48:	00002025 	move	a0,zero
ae4c:	00002825 	move	a1,zero
ae50:	34060002 	li	a2,0x2
ae54:	02c04025 	move	t0,s6
ae58:	00004825 	move	t1,zero
ae5c:	00005025 	move	t2,zero
ae60:	0c001d4d 	jal	0x7534
ae64:	00005825 	move	t3,zero
ae68:	ae6285e0 	sw	v0,-31264(s3)
ae6c:	8e6785e0 	lw	a3,-31264(s3)
ae70:	00002025 	move	a0,zero
ae74:	00002825 	move	a1,zero
ae78:	34060003 	li	a2,0x3
ae7c:	00004025 	move	t0,zero
ae80:	00004825 	move	t1,zero
ae84:	00005025 	move	t2,zero
ae88:	0c001d4d 	jal	0x7534
ae8c:	00005825 	move	t3,zero
ae90:	ae8285e4 	sw	v0,-31260(s4)
ae94:	8e8785e4 	lw	a3,-31260(s4)
ae98:	00002025 	move	a0,zero
ae9c:	00002825 	move	a1,zero
aea0:	34060005 	li	a2,0x5
aea4:	00004025 	move	t0,zero
aea8:	00004825 	move	t1,zero
aeac:	00005025 	move	t2,zero
aeb0:	0c001d4d 	jal	0x7534
aeb4:	00005825 	move	t3,zero
aeb8:	aea20c90 	sw	v0,3216(s5)
aebc:	8e240004 	lw	a0,4(s1)
aec0:	8eb00c90 	lw	s0,3216(s5)
aec4:	24840008 	addiu	a0,a0,8
aec8:	84850000 	lh	a1,0(a0)
aecc:	8c860004 	lw	a2,4(a0)
aed0:	00c0f809 	jalr	a2
aed4:	02252021 	addu	a0,s1,a1
aed8:	16000003 	bnez	s0,0xaee8
aedc:	00402025 	move	a0,v0
aee0:	1000000a 	b	0xaf0c
aee4:	34040000 	li	a0,0x0
aee8:	50800008 	beqzl	a0,0xaf0c
aeec:	34040000 	li	a0,0x0
aef0:	54900003 	bnel	a0,s0,0xaf00
aef4:	8c84001c 	lw	a0,28(a0)
aef8:	10000004 	b	0xaf0c
aefc:	34040001 	li	a0,0x1
af00:	1480fffb 	bnez	a0,0xaef0
af04:	00000000 	nop
af08:	34040000 	li	a0,0x0
af0c:	50800003 	beqzl	a0,0xaf1c
af10:	8e310000 	lw	s1,0(s1)
af14:	10000004 	b	0xaf28
af18:	00000000 	nop
af1c:	5620ffb5 	bnezl	s1,0xadf4
af20:	8ea40c90 	lw	a0,3216(s5)
af24:	34110000 	li	s1,0x0
af28:	1220ff8f 	beqz	s1,0xad68
af2c:	00000000 	nop
af30:	8fa50004 	lw	a1,4(sp)
af34:	8fa60000 	lw	a2,0(sp)
af38:	02202025 	move	a0,s1
af3c:	0c002b3b 	jal	0xacec
af40:	34070001 	li	a3,0x1
af44:	8fb00008 	lw	s0,8(sp)
af48:	8fb1000c 	lw	s1,12(sp)
af4c:	8fb20010 	lw	s2,16(sp)
af50:	8fb30014 	lw	s3,20(sp)
af54:	8fb40018 	lw	s4,24(sp)
af58:	8fb5001c 	lw	s5,28(sp)
af5c:	8fb60020 	lw	s6,32(sp)
af60:	8fb70024 	lw	s7,36(sp)
af64:	8fbe0028 	lw	s8,40(sp)
af68:	8fbf002c 	lw	ra,44(sp)
af6c:	03e00008 	jr	ra
af70:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: cFactory::FindObject(const cType *, const cGUID &, bool) const
// Address: 0x0000acec, Size: 648B
// Obj: cAll_psp.obj

s32 cFactory__FindGroup_constcTypeptr();            /* extern */
s32 cGroup__FindObject_constcGUIDrefconst(s32, s32); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

s32 cFactory__FindObject_constcTypeptr_constcGUIDref_boolconst(? *arg0, s32 arg1, s32 arg2, s32 arg3) {
    ? *var_s1;
    s16 temp_a1;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;

    temp_v0_2 = cFactory__FindGroup_constcTypeptr();
    if ((temp_v0_2 == 0) || (temp_v0 = cGroup__FindObject_constcGUIDrefconst(temp_v0_2, arg2), (temp_v0 == 0))) {
        if (arg3 & 0xFF) {
            var_s1 = *arg0;
            if (var_s1 == NULL) {
                goto block_30;
            }
loop_8:
            if (*(s32 *)0x40C90 == 0) {
                if (*(s32 *)0x385E4 == 0) {
                    if (*(s32 *)0x385E0 == 0) {
                        if (*(s32 *)0x385DC == 0) {
                            *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36C830, 0x36C838, 1, 0);
                        }
                        *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
                    }
                    *(s32 *)0x385E4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 3, *(s32 *)0x385E0);
                }
                *(s32 *)0x40C90 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 5, *(s32 *)0x385E4);
            }
            temp_a0 = var_s1->unk4;
            temp_s0 = *(s32 *)0x40C90;
            temp_a1 = temp_a0->unk8;
            temp_a2 = (temp_a0 + 8)->unk4;
            var_a0 = temp_a2(var_s1 + temp_a1, temp_a1, temp_a2);
            if (temp_s0 == 0) {
                goto block_24;
            }
            if (var_a0 != NULL) {
loop_19:
                if (var_a0 != temp_s0) {
                    var_a0 = var_a0->unk1C;
                    if (var_a0 == NULL) {
                        goto block_24;
                    }
                    goto loop_19;
                }
                var_a0_2 = 1;
            } else {
block_24:
                var_a0_2 = 0;
            }
            if (var_a0_2 == 0) {
                var_s1 = var_s1->unk0;
                if (var_s1 == NULL) {
block_30:
                    var_s1 = NULL;
                } else {
                    goto loop_8;
                }
            }
            if (var_s1 != NULL) {
                return cFactory__FindObject_constcTypeptr_constcGUIDref_boolconst(var_s1, subroutine_arg1, subroutine_arg0, 1);
            }
            /* Duplicate return node #5. Try simplifying control flow for better match */
            return 0;
        }
        return 0;
    }
    return temp_v0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
