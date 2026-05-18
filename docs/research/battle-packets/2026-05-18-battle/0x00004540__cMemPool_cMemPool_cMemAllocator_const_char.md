# Battle Packet: `cMemPool::cMemPool(cMemAllocator *, const char *)`

## Target

- Address: `0x00004540`
- Size: `336` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cMemPool`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 4B; Constructor retry family; manageable size`
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

Callees:
- `0x000012d0` `untried` 68B `cStrLength(const char *)`
- `0x00001588` `untried` 68B `cStrCopy(char *, const char *, int)`
- `0x00004a38` `untried` 108B `cMemPool::GetLock(void) static`
- `0x001b9c64` `untried` 68B `__vec_new`
- `0x0036bbcc` `untried` 0B `sceKernelUnlockMutex`
- `0x0036bc24` `untried` 0B `sceKernelLockMutex`

Callers:
- `0x00004aa4` `untried` 148B `cMemPool::AllocGlobalPool(void) static`
- `0x0000ea80` `untried` 228B `cFilePackPlatform::PackAlloc(unsigned int)`
- `0x000bab28` `untried` 73920B `__sti__eAll_psp_cpp`
- `0x0016ccbc` `untried` 7036B `gcGame::gcGame(cBase *)`
- `0x001810a4` `untried` 123704B `__sti__gcAll_psp_cpp`
- `0x001a539c` `failed` 176B `nwNetwork::GetMemPool(void) static`
- `0x001c6164` `matched` 120B `cStaticMemPool::cStaticMemPool(const char *, unsigned int, unsigned int)`
- `0x001db65c` `matched` 136B `eDisplayList::eDisplayList(void)`

## Placement

- Canonical source: `src/cMemPool.cpp`
- Header(s): `include/cMemPool.h`
- Allowed source(s): `src/cMemPool.cpp`
- Split-TU prefix: `src/cMemPool_*.cpp`

## Compiler Guidance

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/cMemPool.h`

```cpp
#ifndef CMEMPOOL_H
#define CMEMPOOL_H

class cMemPool {
public:
    static float GetSizeScale(void);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00004690` 272B `cMemPool::~cMemPool(void)` — `src/cMemPool.cpp`
- `0x00004a2c` 12B `cMemPool::GetSizeScale(void) static` — `src/cMemPool_GetSizeScale.cpp`

## Prior Failure Notes

### Note 1

- session `a1e21f28`

Implemented a real C++ cMemPool constructor with the classdesc store, GUID __vec_new, name copy, CRC calculation, mutex lock/unlock, and circular pool-list insertion. The best source is 4 bytes off: every instruction after relocation masking matches except the CRC setup emits addiu a3,v0,-1 followed by move a1,a3, while expected emits addiu a1,v0,-1 followed by move a3,a1. The CRC loop body, list insertion, lock calls, prologue, and epilogue are otherwise byte-identical.

Tried direct constants vs symbol references for the classdesc, GUID ctor, CRC table, and pool-list head; typed base-pointer access at 0x380000 to get lw/sw -0x3fb0(s1); multiple CRC loop forms including one-counter, two-counter, swapped-counter, expression-only table lookup, explicit old/temp ordering, declaration-order changes, and zero-instruction barriers. The first permuter run improved 23 -> 8 -> 4 bytes over 300s and saved the current source; a second 300s permuter run from the 4-byte baseline compiled 5515 candidates with no improvement. This is not a bnel/unmatchable-branch case: the remaining diff is just register selection in two setup instructions.


## Disassembly

```asm
4540:	27bdfff0 	addiu	sp,sp,-16
4544:	afb00000 	sw	s0,0(sp)
4548:	00808025 	move	s0,a0
454c:	3c040038 	lui	a0,0x38
4550:	2484e698 	addiu	a0,a0,-6504
4554:	ae050024 	sw	a1,36(s0)
4558:	ae040098 	sw	a0,152(s0)
455c:	ae000028 	sw	zero,40(s0)
4560:	afb10004 	sw	s1,4(sp)
4564:	00c08825 	move	s1,a2
4568:	ae00002c 	sw	zero,44(s0)
456c:	3c07001c 	lui	a3,0x1c
4570:	ae000030 	sw	zero,48(s0)
4574:	26040034 	addiu	a0,s0,52
4578:	34050008 	li	a1,0x8
457c:	34060008 	li	a2,0x8
4580:	afb20008 	sw	s2,8(sp)
4584:	afbf000c 	sw	ra,12(sp)
4588:	0c06e719 	jal	0x1b9c64
458c:	24e7607c 	addiu	a3,a3,24700
4590:	26120004 	addiu	s2,s0,4
4594:	ae000094 	sw	zero,148(s0)
4598:	02402025 	move	a0,s2
459c:	02202825 	move	a1,s1
45a0:	0c000562 	jal	0x1588
45a4:	34060020 	li	a2,0x20
45a8:	a2000023 	sb	zero,35(s0)
45ac:	0c0004b4 	jal	0x12d0
45b0:	02402025 	move	a0,s2
45b4:	2445ffff 	addiu	a1,v0,-1
45b8:	2404ffff 	li	a0,-1
45bc:	02403025 	move	a2,s2
45c0:	00a03825 	move	a3,a1
45c4:	10400010 	beqz	v0,0x4608
45c8:	3c110038 	lui	s1,0x38
45cc:	3c080038 	lui	t0,0x38
45d0:	2508bc50 	addiu	t0,t0,-17328
45d4:	90c90000 	lbu	t1,0(a2)
45d8:	00a05025 	move	t2,a1
45dc:	00894826 	xor	t1,a0,t1
45e0:	312900ff 	andi	t1,t1,0xff
45e4:	00094880 	sll	t1,t1,0x2
45e8:	01284821 	addu	t1,t1,t0
45ec:	8d290000 	lw	t1,0(t1)
45f0:	00042202 	srl	a0,a0,0x8
45f4:	24e5ffff 	addiu	a1,a3,-1
45f8:	24c60001 	addiu	a2,a2,1
45fc:	01242026 	xor	a0,t1,a0
4600:	1540fff4 	bnez	t2,0x45d4
4604:	00a03825 	move	a3,a1
4608:	2405ffff 	li	a1,-1
460c:	00852026 	xor	a0,a0,a1
4610:	0c00128e 	jal	0x4a38
4614:	ae040000 	sw	a0,0(s0)
4618:	8c440000 	lw	a0,0(v0)
461c:	34050001 	li	a1,0x1
4620:	0c0daf09 	jal	0x36bc24
4624:	00003025 	move	a2,zero
4628:	8e24c050 	lw	a0,-16304(s1)
462c:	5080000a 	beqzl	a0,0x4658
4630:	ae30c050 	sw	s0,-16304(s1)
4634:	8c840030 	lw	a0,48(a0)
4638:	ae040030 	sw	a0,48(s0)
463c:	8e24c050 	lw	a0,-16304(s1)
4640:	8e050030 	lw	a1,48(s0)
4644:	ae04002c 	sw	a0,44(s0)
4648:	acb0002c 	sw	s0,44(a1)
464c:	8e04002c 	lw	a0,44(s0)
4650:	10000003 	b	0x4660
4654:	ac900030 	sw	s0,48(a0)
4658:	ae10002c 	sw	s0,44(s0)
465c:	ae100030 	sw	s0,48(s0)
4660:	0c00128e 	jal	0x4a38
4664:	00000000 	nop
4668:	8c440000 	lw	a0,0(v0)
466c:	0c0daef3 	jal	0x36bbcc
4670:	34050001 	li	a1,0x1
4674:	02001025 	move	v0,s0
4678:	8fb00000 	lw	s0,0(sp)
467c:	8fb10004 	lw	s1,4(sp)
4680:	8fb20008 	lw	s2,8(sp)
4684:	8fbf000c 	lw	ra,12(sp)
4688:	03e00008 	jr	ra
468c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: cMemPool::cMemPool(cMemAllocator *, const char *)
// Address: 0x00004540, Size: 336B
// Obj: cAll_psp.obj

? __vec_new(void *, ?, ?, ?);                       /* extern */
s32 *cMemPool__GetLock_voidstatic(s32, ?, u8 *, s32); /* extern */
? cStrCopy_charptr_constcharptr_int(u8 *, ?, ?);    /* extern */
s32 cStrLength_constcharptr(u8 *);                  /* extern */
? sceKernelLockMutex(s32, ?, ?);                    /* extern */
? sceKernelUnlockMutex(s32, ?);                     /* extern */

void *cMemPool__cMemPool_cMemAllocatorptr_constcharptr(void *arg0, s32 arg1, ? arg2) {
    s32 temp_a0;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_v0;
    s32 var_a1;
    s32 var_a3;
    u32 var_a0;
    u8 *temp_s2;
    u8 *var_a2;
    void *var_a0_2;

    arg0->unk24 = arg1;
    arg0->unk98 = 0x37E698;
    arg0->unk28 = 0;
    arg0->unk2C = NULL;
    arg0->unk30 = NULL;
    __vec_new(arg0 + 0x34, 8, 8, 0x1C607C);
    temp_s2 = arg0 + 4;
    arg0->unk94 = 0;
    cStrCopy_charptr_constcharptr_int(temp_s2, arg2, 0x20);
    arg0->unk23 = 0;
    temp_v0 = cStrLength_constcharptr(temp_s2);
    var_a1 = temp_v0 - 1;
    var_a0 = -1U;
    var_a2 = temp_s2;
    var_a3 = var_a1;
    if (temp_v0 != 0) {
        do {
            temp_t2 = var_a1;
            temp_t1 = (((var_a0 ^ *var_a2) & 0xFF) * 4)->unk37BC50;
            var_a1 = var_a3 - 1;
            var_a2 += 1;
            var_a0 = temp_t1 ^ (var_a0 >> 8);
            var_a3 = var_a1;
        } while (temp_t2 != 0);
    }
    temp_a0 = var_a0 ^ -1;
    arg0->unk0 = temp_a0;
    sceKernelLockMutex(*cMemPool__GetLock_voidstatic(temp_a0, -1, var_a2, var_a3), 1, 0);
    var_a0_2 = *(void **)0x37C050;
    if (var_a0_2 == NULL) {
        *(void **)0x37C050 = arg0;
        arg0->unk2C = arg0;
        arg0->unk30 = arg0;
    } else {
        arg0->unk30 = (void *) var_a0_2->unk30;
        arg0->unk2C = (void *) *(void **)0x37C050;
        arg0->unk30->unk2C = arg0;
        var_a0_2 = arg0->unk2C;
        var_a0_2->unk30 = arg0;
    }
    sceKernelUnlockMutex(*cMemPool__GetLock_voidstatic((s32) var_a0_2), 1);
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
