# Battle Packet: `gcValStringIndex::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)`

## Target

- Address: `0x0035ec68`
- Size: `500` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValStringIndex`
- Method family: `VisitReferences`
- Leaf: `False`

## Queue Metadata

- score: `127`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 13B; codegen-specific diagnosis; VisitReferences hard family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x00270834` `untried` 1348B `gcDesiredString::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)`

## Placement

- Canonical source: `src/gcValStringIndex.cpp`
- Header(s): `include/gcValStringIndex.h`
- Allowed source(s): `src/gcValStringIndex.cpp`
- Split-TU prefix: `src/gcValStringIndex_*.cpp`

## Compiler Guidance

VisitReferences template: preserve callback null checks, direct self callback order, then handle/list/member VisitReferences calls in exact field order.

## Class Header: `include/gcValStringIndex.h`

Not found.

## Matched Same-Class Neighbors

- `0x0035e880` 52B `gcValStringIndex::AssignCopy(const cBase *)` — `src/gcValEntityIsLocallyControlled.cpp`
- `0x0035e8b4` 192B `gcValStringIndex::New(cMemPool *, cBase *) static` — `src/gcValStringIndex.cpp`
- `0x0035e974` 280B `gcValStringIndex::GetType(void) const` — `src/gcValStringIndex.cpp`
- `0x0035ea8c` 108B `gcValStringIndex::Write(cFile &) const` — `src/gcValStringIndex.cpp`
- `0x0035eaf8` 212B `gcValStringIndex::Read(cFile &, cMemPool *)` — `src/gcValStringIndex.cpp`

## Prior Failure Notes

### Note 1

- session `d8000fb8`

Added a C++ VisitReferences body close to the target and iterated through high-level, explicit-label, callback-alias, register-variable, barrier, and tail-sharing forms. The best source is saved in src/gcValStringIndex.cpp and compiles to the correct 500B size with the target ancestry-loop shape, including the bnez/bnel structure and the shared recursive/direct callback tail. compare_func.py reports 13/500 bytes differing.

The remaining blocker is saved-register allocation: the target keeps the desired string in s4, callback in s5, and matched desired object in s6, while the best compiler output keeps desired in s4 but swaps callback/matched between s6/s5. A 300s permuter run generated 10,440 candidates, compiled 4,341, improved the baseline from 21 to 13 differing bytes, and saved that best result, but did not find the final s5/s6 swap. Attempts to force named register variables or callback aliases either left the same swap or disturbed the prologue/type-init block badly.

### Note 2

- session `fbe59501`

Added a compiling C++ VisitReferences body and iterated through several shapes: a high-level gcValEnumerationEntry/gcValCameraIsValid-style version, a lower-level m2c-inspired version with explicit temp variables and gotos, a shared-tail callback/recursive-call variant, and a do-while ancestry walk. I also tried both direct DispatchEntry pointer access and the raw +8/+12 field loads, plus boolean-vs-unsigned-char direct-flag tests to chase the lhu/andi/sltu/andi sequence. The best intermediate state got down to 99/500 differing bytes; the current state compiles to 488B, so it is not close enough to permute yet.

The stable match blocker is codegen shape, not semantics. The target wants the exact prologue/register layout `s4=this+8`, `s5=cb`, `s6=matched`, the dispatch load pattern `lw a0,12(s3); addiu a0,8; lh/lw`, and the original ancestry walk / tail-call structure from the provided disassembly. My current source keeps drifting between two bad families: one gets the register allocation closer but duplicates the recursive helper call and misses the bool-mask sequence; the other gets the shared recursive tail and `andi 0xff` but moves callback/desired into the wrong saved registers and shortens the loop. Next attempt should probably start from the best 500B-ish version, force the target register ownership/prologue first, then tune the ancestry loop specifically toward the `bnel`/`lw parent` pattern shown in the user-provided disassembly.


## Disassembly

```asm
35ec68:	27bdffd0 	addiu	sp,sp,-48
35ec6c:	afb30014 	sw	s3,20(sp)
35ec70:	afb40018 	sw	s4,24(sp)
35ec74:	24940008 	addiu	s4,a0,8
35ec78:	00809825 	move	s3,a0
35ec7c:	afb5001c 	sw	s5,28(sp)
35ec80:	afbe0028 	sw	s8,40(sp)
35ec84:	afa50000 	sw	a1,0(sp)
35ec88:	00c02025 	move	a0,a2
35ec8c:	00e0a825 	move	s5,a3
35ec90:	0100f025 	move	s8,t0
35ec94:	afb00008 	sw	s0,8(sp)
35ec98:	afb1000c 	sw	s1,12(sp)
35ec9c:	afb20010 	sw	s2,16(sp)
35eca0:	afb60020 	sw	s6,32(sp)
35eca4:	afb70024 	sw	s7,36(sp)
35eca8:	afbf002c 	sw	ra,44(sp)
35ecac:	10e00004 	beqz	a3,0x35ecc0
35ecb0:	afa90004 	sw	t1,4(sp)
35ecb4:	02602825 	move	a1,s3
35ecb8:	02a0f809 	jalr	s5
35ecbc:	03c03025 	move	a2,s8
35ecc0:	1280005a 	beqz	s4,0x35ee2c
35ecc4:	0260b825 	move	s7,s3
35ecc8:	3c120004 	lui	s2,0x4
35eccc:	8e4485e4 	lw	a0,-31260(s2)
35ecd0:	14800029 	bnez	a0,0x35ed78
35ecd4:	34160000 	li	s6,0x0
35ecd8:	3c110004 	lui	s1,0x4
35ecdc:	8e2485e0 	lw	a0,-31264(s1)
35ece0:	1480001b 	bnez	a0,0x35ed50
35ece4:	3c100004 	lui	s0,0x4
35ece8:	8e0485dc 	lw	a0,-31268(s0)
35ecec:	1480000e 	bnez	a0,0x35ed28
35ecf0:	3c08001c 	lui	t0,0x1c
35ecf4:	3c040037 	lui	a0,0x37
35ecf8:	3c050037 	lui	a1,0x37
35ecfc:	34060001 	li	a2,0x1
35ed00:	00003825 	move	a3,zero
35ed04:	00004025 	move	t0,zero
35ed08:	00004825 	move	t1,zero
35ed0c:	00005025 	move	t2,zero
35ed10:	00005825 	move	t3,zero
35ed14:	2484d894 	addiu	a0,a0,-10092
35ed18:	0c001d4d 	jal	0x7534
35ed1c:	24a5d89c 	addiu	a1,a1,-10084
35ed20:	ae0285dc 	sw	v0,-31268(s0)
35ed24:	3c08001c 	lui	t0,0x1c
35ed28:	8e0785dc 	lw	a3,-31268(s0)
35ed2c:	00002025 	move	a0,zero
35ed30:	00002825 	move	a1,zero
35ed34:	34060002 	li	a2,0x2
35ed38:	00004825 	move	t1,zero
35ed3c:	00005025 	move	t2,zero
35ed40:	00005825 	move	t3,zero
35ed44:	0c001d4d 	jal	0x7534
35ed48:	25083c58 	addiu	t0,t0,15448
35ed4c:	ae2285e0 	sw	v0,-31264(s1)
35ed50:	8e2785e0 	lw	a3,-31264(s1)
35ed54:	00002025 	move	a0,zero
35ed58:	00002825 	move	a1,zero
35ed5c:	34060003 	li	a2,0x3
35ed60:	00004025 	move	t0,zero
35ed64:	00004825 	move	t1,zero
35ed68:	00005025 	move	t2,zero
35ed6c:	0c001d4d 	jal	0x7534
35ed70:	00005825 	move	t3,zero
35ed74:	ae4285e4 	sw	v0,-31260(s2)
35ed78:	8e64000c 	lw	a0,12(s3)
35ed7c:	8e5085e4 	lw	s0,-31260(s2)
35ed80:	24840008 	addiu	a0,a0,8
35ed84:	84850000 	lh	a1,0(a0)
35ed88:	8c860004 	lw	a2,4(a0)
35ed8c:	00c0f809 	jalr	a2
35ed90:	02852021 	addu	a0,s4,a1
35ed94:	00409825 	move	s3,v0
35ed98:	16000003 	bnez	s0,0x35eda8
35ed9c:	8fa50000 	lw	a1,0(sp)
35eda0:	1000000a 	b	0x35edcc
35eda4:	34040000 	li	a0,0x0
35eda8:	12600008 	beqz	s3,0x35edcc
35edac:	34040000 	li	a0,0x0
35edb0:	56700003 	bnel	s3,s0,0x35edc0
35edb4:	8e73001c 	lw	s3,28(s3)
35edb8:	10000004 	b	0x35edcc
35edbc:	34040001 	li	a0,0x1
35edc0:	1660fffb 	bnez	s3,0x35edb0
35edc4:	00000000 	nop
35edc8:	34040000 	li	a0,0x0
35edcc:	54800001 	bnezl	a0,0x35edd4
35edd0:	0280b025 	move	s6,s4
35edd4:	12c00009 	beqz	s6,0x35edfc
35edd8:	02802025 	move	a0,s4
35eddc:	96c40028 	lhu	a0,40(s6)
35ede0:	30a6fe00 	andi	a2,a1,0xfe00
35ede4:	00c42024 	and	a0,a2,a0
35ede8:	0004202b 	sltu	a0,zero,a0
35edec:	308400ff 	andi	a0,a0,0xff
35edf0:	14800009 	bnez	a0,0x35ee18
35edf4:	00000000 	nop
35edf8:	02802025 	move	a0,s4
35edfc:	8fa90004 	lw	t1,4(sp)
35ee00:	02e03025 	move	a2,s7
35ee04:	02a03825 	move	a3,s5
35ee08:	0c09c20d 	jal	0x270834
35ee0c:	03c04025 	move	t0,s8
35ee10:	10000006 	b	0x35ee2c
35ee14:	00000000 	nop
35ee18:	12a00004 	beqz	s5,0x35ee2c
35ee1c:	02e02025 	move	a0,s7
35ee20:	02802825 	move	a1,s4
35ee24:	02a0f809 	jalr	s5
35ee28:	03c03025 	move	a2,s8
35ee2c:	8fb00008 	lw	s0,8(sp)
35ee30:	8fb1000c 	lw	s1,12(sp)
35ee34:	8fb20010 	lw	s2,16(sp)
35ee38:	8fb30014 	lw	s3,20(sp)
35ee3c:	8fb40018 	lw	s4,24(sp)
35ee40:	8fb5001c 	lw	s5,28(sp)
35ee44:	8fb60020 	lw	s6,32(sp)
35ee48:	8fb70024 	lw	s7,36(sp)
35ee4c:	8fbe0028 	lw	s8,40(sp)
35ee50:	8fbf002c 	lw	ra,44(sp)
35ee54:	03e00008 	jr	ra
35ee58:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: gcValStringIndex::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)
// Address: 0x0035ec68, Size: 500B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcDesiredString__VisitReferences_unsignedint_cBaseptr_void_ptr_cBaseptr_unsignedint_voidptr_voidptr_unsignedint(void *, s32, void *, ? (*)(void *, void *, ?)); /* extern */

void gcValStringIndex__VisitReferences_unsignedint_cBaseptr_void_ptr_cBaseptr_unsignedint_voidptr_voidptr_unsignedint(void *arg0, s32 arg1, void *arg2, ? (*arg3)(void *, void *, ?)) {
    ? temp_s8;
    s16 temp_a1;
    s32 temp_s0;
    s32 var_a0;
    void *(*temp_a2)(s32, s16, ?);
    void *temp_a0;
    void *temp_s4;
    void *var_s3;
    void *var_s6;

    temp_s4 = arg0 + 8;
    temp_s8 = M2C_ERROR(/* Read from unset register $t0 */);
    if (arg3 != NULL) {
        arg3(arg2, arg0, temp_s8);
    }
    if (temp_s4 != NULL) {
        var_s6 = NULL;
        if (*(s32 *)0x385E4 == 0) {
            if (*(s32 *)0x385E0 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                }
                *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
            }
            *(s32 *)0x385E4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 3, *(s32 *)0x385E0);
        }
        temp_a0 = arg0->unkC;
        temp_s0 = *(s32 *)0x385E4;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_s3 = temp_a2(temp_s4 + temp_a1, temp_a1, temp_a2);
        if (temp_s0 == 0) {
            goto block_17;
        }
        var_a0 = 0;
        if (var_s3 != NULL) {
loop_12:
            if (var_s3 != temp_s0) {
                var_s3 = var_s3->unk1C;
                if (var_s3 == NULL) {
block_17:
                    var_a0 = 0;
                } else {
                    goto loop_12;
                }
            } else {
                var_a0 = 1;
            }
        }
        if (var_a0 != 0) {
            var_s6 = temp_s4;
        }
        if (var_s6 != NULL) {
            if (!(subroutine_arg0 & 0xFE00 & var_s6->unk28 & 0xFF)) {
                goto block_23;
            }
            if (arg3 != NULL) {
                arg3(arg0, temp_s4, temp_s8);
            }
        } else {
block_23:
            gcDesiredString__VisitReferences_unsignedint_cBaseptr_void_ptr_cBaseptr_unsignedint_voidptr_voidptr_unsignedint(temp_s4, subroutine_arg0, arg0, arg3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
