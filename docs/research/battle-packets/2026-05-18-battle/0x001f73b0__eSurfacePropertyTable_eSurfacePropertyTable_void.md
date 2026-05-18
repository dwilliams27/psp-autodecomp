# Battle Packet: `eSurfacePropertyTable::~eSurfacePropertyTable(void)`

## Target

- Address: `0x001f73b0`
- Size: `480` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSurfacePropertyTable`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x000070bc` `untried` 240B `cBaseArray::RemoveAll(void)`
- `0x00009dc4` `failed` 432B `cObject::~cObject(void)`

## Placement

- Canonical source: `src/eSurfacePropertyTable.cpp`
- Header(s): `include/eSurfacePropertyTable.h`
- Allowed source(s): `src/eSurfacePropertyTable.cpp`, `src/eSurfacePropertyTable_dtor.cpp`
- Split-TU prefix: `src/eSurfacePropertyTable_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eSurfacePropertyTable.h`

Not found.

## Matched Same-Class Neighbors

- `0x00053f50` 492B `eSurfacePropertyTable::Read(cFile &, cMemPool *)` — `src/eSurfacePropertyTable.cpp`
- `0x001f6f28` 88B `eSurfacePropertyTable::AssignCopy(const cBase *)` — `src/eSurfacePropertyTable.cpp`
- `0x001f6f80` 148B `eSurfacePropertyTable::New(cMemPool *, cBase *) static` — `src/gcValLobbyMailInfo.cpp`
- `0x001f7014` 288B `eSurfacePropertyTable::GetType(void) const` — `src/eSurfacePropertyTable.cpp`

## Matched Method Exemplars

### Exemplar 1: `eOnePassModelMtl::~eOnePassModelMtl(void)`

- Address: `0x00081f74`
- Size: `480` bytes
- Source: `src/eOnePassModelMtl_dtor.cpp`

```cpp
eOnePassModelMtl::~eOnePassModelMtl() {
    *(void **)((char *)this + 4) = (void *)0x385430;

    char *p64 = (char *)this + 0x64;
    char *p60 = (char *)this + 0x60;
    void *m78 = *(void **)((char *)this + 0x78);
    if (m78 != 0) {
        char *type = *(char **)((char *)m78 + 4);
        DeleteRec *slot = (DeleteRec *)(type + 0x50);
        short off = slot->offset;
        char *base = (char *)m78 + off;
        __asm__ volatile("" : "+r"(base));
        void (*fn)(void *, void *) = slot->fn;
        fn(base, (void *)3);
        *(void **)((char *)this + 0x78) = 0;
    }

    PlatformFree();
    *(void **)((char *)this + 4) = (void *)0x380018;

    if (p64 != 0) {
        void *entries = *(void **)((char *)this + 0x64);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                __asm__ volatile("" : "+r"(base));
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x64) = 0;
        }
    }

    if (p60 != 0) {
        void *entries = *(void **)((char *)this + 0x60);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                __asm__ volatile("" : "+r"(base));
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x60) = 0;
        }
    }

    eMaterial_dtor(this, 0);
}
```

### Exemplar 2: `eStandardGeomTrailMtl::~eStandardGeomTrailMtl(void)`

- Address: `0x000879f4`
- Size: `480` bytes
- Source: `src/eStandardGeomTrailMtl_dtor.cpp`

```cpp
eStandardGeomTrailMtl::~eStandardGeomTrailMtl() {
    *(void **)((char *)this + 4) = (void *)0x385C08;

    char *p64 = (char *)this + 0x64;
    char *p60 = (char *)this + 0x60;
    void *m70 = *(void **)((char *)this + 0x70);
    if (m70 != 0) {
        char *type = *(char **)((char *)m70 + 4);
        DeleteRec *slot = (DeleteRec *)(type + 0x50);
        short off = slot->offset;
        __asm__ volatile("" ::: "memory");
        char *base = (char *)m70 + off;
        void (*fn)(void *, void *) = slot->fn;
        fn(base, (void *)3);
        *(void **)((char *)this + 0x70) = 0;
    }

    PlatformFree();
    *(void **)((char *)this + 4) = (void *)0x380018;

    if (p64 != 0) {
        void *entries = *(void **)((char *)this + 0x64);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x64) = 0;
        }
    }

    if (p60 != 0) {
        void *entries = *(void **)((char *)this + 0x60);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x60) = 0;
        }
    }

    eMaterial_dtor(this, 0);
}
```

### Exemplar 3: `eDecalSystem::~eDecalSystem(void)`

- Address: `0x000332a8`
- Size: `428` bytes
- Source: `src/eDecalSystem_dtor.cpp`

```cpp
eDecalSystem::~eDecalSystem() {
    *(void **)((char *)this + 4) = (void *)0x3805D8;

    void *arr94Addr = (char *)this + 0x94;
    if (arr94Addr != 0) {
        void *entries = *(void **)((char *)this + 0x94);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x94) = 0;
        }
    }

    void *arr90Addr = (char *)this + 0x90;
    if (arr90Addr != 0) {
        void *entries = *(void **)((char *)this + 0x90);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x90) = 0;
        }
    }

    eGeom_dtor(this, 0);
}
```

### Exemplar 4: `eSimulatedController::~eSimulatedController(void)`

- Address: `0x0006bf10`
- Size: `364` bytes
- Source: `src/eSimulatedController_dtor.cpp`

```cpp
eSimulatedController::~eSimulatedController() {
    *(void **)((char *)this + 4) = (void *)0x3834D8;
    cBaseArray *arr44 = (cBaseArray *)((char *)this + 0x44);
    cBaseArray *arr3C = (cBaseArray *)((char *)this + 0x3C);
    if (arr44 != 0) {
        arr44->RemoveAll();
    }
    if (arr3C != 0) {
        arr3C->RemoveAll();
    }
    void *bodyArr = (char *)this + 0x38;
    if (bodyArr != 0) {
        void *entries = *(void **)((char *)this + 0x38);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x38) = 0;
        }
    }
    ePhysicsController_dtor(this, 0);
}
```


## Prior Failure Notes

### Note 1

- session `fe0f4d81`; src `src/eSurfacePropertyTable_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/fe0f4d81/001f73b0__agent_self_reported_failure__src_eSurfacePropertyTable_dtor.cpp`

category=REG_ALLOC. Match converged to 8/480 byte diff after substantial restructuring (started 83 bytes, then 50, then 11, then 8 via mask-variable trick). Remaining 8 bytes is a pure s3<->s4 register swap on (loop-index i) and (loop-invariant mask 0x3FFFFFFF) — first differing instruction at 0x6c: 'li s4,0x0' (expected) vs 'li s3,0x0' (current). All structural instructions match: same control flow, same stack frame (-48), same call sequence, identical immediates, identical memory offsets. Permuter ran 240s post-convergence, 0 improvements. Tried both orderings of declaration: declaring 'int mask' before 'int i' shifts the mask init out of the beqz delay slot (~18 bytes drift); declaring 'int mask' inside 'if (i < outerCount)' AFTER int i gets the cleanest result (8 bytes) but allocator still picks i=s3 not s4. Also tried explicit asm-init for offset (made it 288 bytes worse), removing mask entirely (11 bytes with instruction reorder at 0x78), sched=2 (105 bytes worse). Source structure that got to 8 bytes: inlined (char*)this+0x4C/0x44 instead of stored locals (huge reduction from 83 to 50 bytes), reload outerEntries fresh inside the loop body each iter (50 -> 11 bytes), explicit 'int mask = 0x3FFFFFFF; __asm__ volatile("" : "+r"(mask));' inside the if-block (11 -> 8 bytes). The remaining s3/s4 swap matches the textbook REG_ALLOC drift criteria in docs/research/snc-register-allocation.md (≤8 bytes, only register-field differences, callee-save only, permuter zero-progress). Note that the sister file src/eOnePassModelMtl_dtor.cpp has a related 67-byte unmatched diff with the same structural pattern (m78 vtable destruct + two array deletions); fixing that would benefit from the same eSimulatedController-style barriers used here for function 1.


## Disassembly

```asm
1f73b0:	27bdffd0 	addiu	sp,sp,-48
1f73b4:	afb00000 	sw	s0,0(sp)
1f73b8:	afb10004 	sw	s1,4(sp)
1f73bc:	afb20008 	sw	s2,8(sp)
1f73c0:	afb3000c 	sw	s3,12(sp)
1f73c4:	afb40010 	sw	s4,16(sp)
1f73c8:	afb50014 	sw	s5,20(sp)
1f73cc:	afb60018 	sw	s6,24(sp)
1f73d0:	afb7001c 	sw	s7,28(sp)
1f73d4:	afbe0020 	sw	s8,32(sp)
1f73d8:	afbf0024 	sw	ra,36(sp)
1f73dc:	00808025 	move	s0,a0
1f73e0:	1080005d 	beqz	a0,0x1f7558
1f73e4:	00a08825 	move	s1,a1
1f73e8:	3c040038 	lui	a0,0x38
1f73ec:	24841dc0 	addiu	a0,a0,7616
1f73f0:	ae040004 	sw	a0,4(s0)
1f73f4:	2604004c 	addiu	a0,s0,76
1f73f8:	10800041 	beqz	a0,0x1f7500
1f73fc:	261e0044 	addiu	s8,s0,68
1f7400:	8e04004c 	lw	a0,76(s0)
1f7404:	10800005 	beqz	a0,0x1f741c
1f7408:	34120000 	li	s2,0x0
1f740c:	8c92fffc 	lw	s2,-4(a0)
1f7410:	3c054000 	lui	a1,0x4000
1f7414:	24a5ffff 	addiu	a1,a1,-1
1f7418:	02459024 	and	s2,s2,a1
1f741c:	34140000 	li	s4,0x0
1f7420:	0292282a 	slt	a1,s4,s2
1f7424:	10a00027 	beqz	a1,0x1f74c4
1f7428:	3c134000 	lui	s3,0x4000
1f742c:	2673ffff 	addiu	s3,s3,-1
1f7430:	34150000 	li	s5,0x0
1f7434:	0095b021 	addu	s6,a0,s5
1f7438:	52c0001f 	beqzl	s6,0x1f74b8
1f743c:	26940001 	addiu	s4,s4,1
1f7440:	8ed70000 	lw	s7,0(s6)
1f7444:	12e00003 	beqz	s7,0x1f7454
1f7448:	34050000 	li	a1,0x0
1f744c:	8ee5fffc 	lw	a1,-4(s7)
1f7450:	00b32824 	and	a1,a1,s3
1f7454:	34060000 	li	a2,0x0
1f7458:	00c5382a 	slt	a3,a2,a1
1f745c:	10e00005 	beqz	a3,0x1f7474
1f7460:	00000000 	nop
1f7464:	24c60001 	addiu	a2,a2,1
1f7468:	00c5382a 	slt	a3,a2,a1
1f746c:	14e0fffe 	bnez	a3,0x1f7468
1f7470:	24c60001 	addiu	a2,a2,1
1f7474:	12e0000f 	beqz	s7,0x1f74b4
1f7478:	26f7fffc 	addiu	s7,s7,-4
1f747c:	52e0000c 	beqzl	s7,0x1f74b0
1f7480:	aec00000 	sw	zero,0(s6)
1f7484:	0c0011e8 	jal	0x47a0
1f7488:	02e02025 	move	a0,s7
1f748c:	8c440024 	lw	a0,36(v0)
1f7490:	8c85001c 	lw	a1,28(a0)
1f7494:	24a50030 	addiu	a1,a1,48
1f7498:	84a60000 	lh	a2,0(a1)
1f749c:	00862021 	addu	a0,a0,a2
1f74a0:	8ca60004 	lw	a2,4(a1)
1f74a4:	00c0f809 	jalr	a2
1f74a8:	02e02825 	move	a1,s7
1f74ac:	aec00000 	sw	zero,0(s6)
1f74b0:	8e04004c 	lw	a0,76(s0)
1f74b4:	26940001 	addiu	s4,s4,1
1f74b8:	0292282a 	slt	a1,s4,s2
1f74bc:	14a0ffdd 	bnez	a1,0x1f7434
1f74c0:	26b50004 	addiu	s5,s5,4
1f74c4:	1080000e 	beqz	a0,0x1f7500
1f74c8:	2492fffc 	addiu	s2,a0,-4
1f74cc:	5240000c 	beqzl	s2,0x1f7500
1f74d0:	ae00004c 	sw	zero,76(s0)
1f74d4:	0c0011e8 	jal	0x47a0
1f74d8:	02402025 	move	a0,s2
1f74dc:	8c440024 	lw	a0,36(v0)
1f74e0:	8c85001c 	lw	a1,28(a0)
1f74e4:	24a50030 	addiu	a1,a1,48
1f74e8:	84a60000 	lh	a2,0(a1)
1f74ec:	00862021 	addu	a0,a0,a2
1f74f0:	8ca60004 	lw	a2,4(a1)
1f74f4:	00c0f809 	jalr	a2
1f74f8:	02402825 	move	a1,s2
1f74fc:	ae00004c 	sw	zero,76(s0)
1f7500:	13c00004 	beqz	s8,0x1f7514
1f7504:	02002025 	move	a0,s0
1f7508:	0c001c2f 	jal	0x70bc
1f750c:	03c02025 	move	a0,s8
1f7510:	02002025 	move	a0,s0
1f7514:	0c002771 	jal	0x9dc4
1f7518:	00002825 	move	a1,zero
1f751c:	32240001 	andi	a0,s1,0x1
1f7520:	1080000d 	beqz	a0,0x1f7558
1f7524:	00000000 	nop
1f7528:	1200000b 	beqz	s0,0x1f7558
1f752c:	00000000 	nop
1f7530:	0c0011e8 	jal	0x47a0
1f7534:	02002025 	move	a0,s0
1f7538:	8c440024 	lw	a0,36(v0)
1f753c:	8c85001c 	lw	a1,28(a0)
1f7540:	24a50030 	addiu	a1,a1,48
1f7544:	84a60000 	lh	a2,0(a1)
1f7548:	00862021 	addu	a0,a0,a2
1f754c:	8ca60004 	lw	a2,4(a1)
1f7550:	00c0f809 	jalr	a2
1f7554:	02002825 	move	a1,s0
1f7558:	8fb00000 	lw	s0,0(sp)
1f755c:	8fb10004 	lw	s1,4(sp)
1f7560:	8fb20008 	lw	s2,8(sp)
1f7564:	8fb3000c 	lw	s3,12(sp)
1f7568:	8fb40010 	lw	s4,16(sp)
1f756c:	8fb50014 	lw	s5,20(sp)
1f7570:	8fb60018 	lw	s6,24(sp)
1f7574:	8fb7001c 	lw	s7,28(sp)
1f7578:	8fbe0020 	lw	s8,32(sp)
1f757c:	8fbf0024 	lw	ra,36(sp)
1f7580:	03e00008 	jr	ra
1f7584:	27bd0030 	addiu	sp,sp,48
1f7588:	1000ffff 	b	0x1f7588
1f758c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eSurfacePropertyTable::~eSurfacePropertyTable(void)
// Address: 0x001f73b0, Size: 480B
// Obj: eAll_psp.obj

? cBaseArray__RemoveAll_void(void *);               /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32, s32); /* extern */
? cObject___dtor_cObject_void(void *, ?);           /* extern */

void eSurfacePropertyTable___dtor_eSurfacePropertyTable_void(void *arg0, s32 arg1) {
    ? (*temp_a2)(void *, void *, ?);
    ? (*temp_a2_2)(void *, void *, ?);
    ? (*temp_a2_3)(void *, void *, ?);
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    void **temp_s6;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_a1_3;
    void *temp_s2;
    void *temp_s7;
    void *temp_s7_2;
    void *temp_s8;

    if (arg0 != NULL) {
        arg0->unk4 = 0x381DC0;
        temp_s8 = arg0 + 0x44;
        if ((arg0 + 0x4C) != NULL) {
            temp_a0 = arg0->unk4C;
            var_s2 = 0;
            if (temp_a0 != NULL) {
                var_s2 = temp_a0->unk-4 & 0x3FFFFFFF;
            }
            var_s4 = 0;
            var_a1 = var_s2 > 0;
            if (var_a1 != 0) {
                var_s5 = 0;
                do {
                    temp_s6 = arg0->unk4C + var_s5;
                    if (temp_s6 != NULL) {
                        temp_s7 = *temp_s6;
                        var_a1_2 = 0;
                        if (temp_s7 != NULL) {
                            var_a1_2 = temp_s7->unk-4 & 0x3FFFFFFF;
                        }
                        var_a2 = 0;
                        var_a3 = var_a1_2 > 0;
                        if (var_a3 != 0) {
                            var_a2 = 1;
                            do {
                                var_a3 = var_a2 < var_a1_2;
                                var_a2 += 1;
                            } while (var_a3 != 0);
                        }
                        temp_s7_2 = temp_s7 - 4;
                        if (temp_s7 != NULL) {
                            if (temp_s7_2 != NULL) {
                                temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s7_2, var_a1_2, var_a2, var_a3)->unk24;
                                temp_a1 = temp_a0_2->unk1C;
                                temp_a2 = (temp_a1 + 0x30)->unk4;
                                temp_a2(temp_a0_2 + temp_a1->unk30, temp_s7_2, temp_a2);
                            }
                            *temp_s6 = NULL;
                        }
                    }
                    var_s4 += 1;
                    var_a1 = var_s4 < var_s2;
                    var_s5 += 4;
                } while (var_a1 != 0);
            }
            temp_s2 = arg0->unk4C - 4;
            if (arg0->unk4C != NULL) {
                if (temp_s2 != NULL) {
                    temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2, var_a1)->unk24;
                    temp_a1_2 = temp_a0_3->unk1C;
                    temp_a2_2 = (temp_a1_2 + 0x30)->unk4;
                    temp_a2_2(temp_a0_3 + temp_a1_2->unk30, temp_s2, temp_a2_2);
                }
                arg0->unk4C = NULL;
            }
        }
        if (temp_s8 != NULL) {
            cBaseArray__RemoveAll_void(temp_s8);
        }
        cObject___dtor_cObject_void(arg0, 0);
        if ((arg1 & 1) && (arg0 != NULL)) {
            temp_a0_4 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a1_3 = temp_a0_4->unk1C;
            temp_a2_3 = (temp_a1_3 + 0x30)->unk4;
            temp_a2_3(temp_a0_4 + temp_a1_3->unk30, arg0, temp_a2_3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
