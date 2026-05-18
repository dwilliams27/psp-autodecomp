# Battle Packet: `eReflectionModelMtl::~eReflectionModelMtl(void)`

## Target

- Address: `0x00084444`
- Size: `452` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eReflectionModelMtl`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `168`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`
- `0x00031f10` `failed` 100B `eTexCoordGen::~eTexCoordGen(void)`
- `0x000843cc` `matched` 8B `eReflectionModelMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eReflectionModelMtl.cpp`
- Header(s): `include/eReflectionModelMtl.h`
- Allowed source(s): `src/eReflectionModelMtl.cpp`, `src/eReflectionModelMtl_dtor.cpp`
- Split-TU prefix: `src/eReflectionModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eReflectionModelMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x000840d4` 172B `eReflectionModelMtl::Write(cFile &) const` — `src/eReflectionModelMtl.cpp`
- `0x00084180` 420B `eReflectionModelMtl::Read(cFile &, cMemPool *)` — `src/eReflectionModelMtl_Read.cpp`
- `0x00084324` 168B `eReflectionModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eReflectionModelMtl.cpp`
- `0x000843cc` 8B `eReflectionModelMtl::PlatformFree(void)` — `src/eReflectionModelMtl_PlatformFree.cpp`
- `0x000843d4` 112B `eReflectionModelMtl::eReflectionModelMtl(cBase *)` — `src/eReflectionModelMtl.cpp`

## Matched Method Exemplars

### Exemplar 1: `eDecalSystem::~eDecalSystem(void)`

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

### Exemplar 2: `eOnePassModelMtl::~eOnePassModelMtl(void)`

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

### Exemplar 3: `eStandardGeomTrailMtl::~eStandardGeomTrailMtl(void)`

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

- session `4183810d`; src `src/eReflectionModelMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/4183810d/00084444__agent_self_reported_failure__src_eReflectionModelMtl_dtor.cpp`

category=REG_ALLOC. Implemented canonical split-TU C++ destructor matching the sibling material destructor pattern: vtable swap to 0x385710, PlatformFree(), eTexCoordGen_dtor(this+0x7C, 2), base eMaterial vtable swap, cArray cleanup at 0x64 and 0x60, eMaterial_dtor(this, 0), and inlined pool delete tail. Final compare is 10/452 bytes different; all observed differences are the first array/p60 saved-register allocation swap, with current using s2 for p60 and s3 for the first entries pointer where the original uses s3 for p60 and s2 for entries. Control flow, calls, stack frame, immediates, offsets, branch-likely forms, and delete tail otherwise match.

Tried the straight sibling pattern, explicit register declarations for p60/entries, stronger no-op register constraints, and a hoisted-second-array source shape; the constraint and hoisting variants worsened the diff substantially, while the sibling/register-declaration pattern returns to the 10-byte saved-register rename. Ran tools/permuter.py for 300.6s on 0x00084444 with 6,504 candidates generated and 2,725 compiled; best stayed at 10 bytes with zero improvements. This matches docs/research/snc-register-allocation.md / ADR-012 context-dependent regalloc drift.


## Disassembly

```asm
84444:	27bdffe0 	addiu	sp,sp,-32
84448:	afb00000 	sw	s0,0(sp)
8444c:	afb10004 	sw	s1,4(sp)
84450:	afb20008 	sw	s2,8(sp)
84454:	afb3000c 	sw	s3,12(sp)
84458:	afbf0010 	sw	ra,16(sp)
8445c:	00808025 	move	s0,a0
84460:	10800060 	beqz	a0,0x845e4
84464:	00a08825 	move	s1,a1
84468:	3c040038 	lui	a0,0x38
8446c:	24845710 	addiu	a0,a0,22288
84470:	ae040004 	sw	a0,4(s0)
84474:	0c0210f3 	jal	0x843cc
84478:	02002025 	move	a0,s0
8447c:	2604007c 	addiu	a0,s0,124
84480:	0c00c7c4 	jal	0x31f10
84484:	34050002 	li	a1,0x2
84488:	3c040038 	lui	a0,0x38
8448c:	24840018 	addiu	a0,a0,24
84490:	ae040004 	sw	a0,4(s0)
84494:	26040064 	addiu	a0,s0,100
84498:	1080001f 	beqz	a0,0x84518
8449c:	26130060 	addiu	s3,s0,96
844a0:	8e120064 	lw	s2,100(s0)
844a4:	12400005 	beqz	s2,0x844bc
844a8:	34040000 	li	a0,0x0
844ac:	8e44fffc 	lw	a0,-4(s2)
844b0:	3c054000 	lui	a1,0x4000
844b4:	24a5ffff 	addiu	a1,a1,-1
844b8:	00852024 	and	a0,a0,a1
844bc:	34050000 	li	a1,0x0
844c0:	00a4302a 	slt	a2,a1,a0
844c4:	10c00005 	beqz	a2,0x844dc
844c8:	00000000 	nop
844cc:	24a50001 	addiu	a1,a1,1
844d0:	00a4302a 	slt	a2,a1,a0
844d4:	14c0fffe 	bnez	a2,0x844d0
844d8:	24a50001 	addiu	a1,a1,1
844dc:	1240000e 	beqz	s2,0x84518
844e0:	2652fffc 	addiu	s2,s2,-4
844e4:	5240000c 	beqzl	s2,0x84518
844e8:	ae000064 	sw	zero,100(s0)
844ec:	0c0011e8 	jal	0x47a0
844f0:	02402025 	move	a0,s2
844f4:	8c440024 	lw	a0,36(v0)
844f8:	8c85001c 	lw	a1,28(a0)
844fc:	24a50030 	addiu	a1,a1,48
84500:	84a60000 	lh	a2,0(a1)
84504:	00862021 	addu	a0,a0,a2
84508:	8ca60004 	lw	a2,4(a1)
8450c:	00c0f809 	jalr	a2
84510:	02402825 	move	a1,s2
84514:	ae000064 	sw	zero,100(s0)
84518:	12600021 	beqz	s3,0x845a0
8451c:	02002025 	move	a0,s0
84520:	8e120060 	lw	s2,96(s0)
84524:	12400005 	beqz	s2,0x8453c
84528:	34040000 	li	a0,0x0
8452c:	8e44fffc 	lw	a0,-4(s2)
84530:	3c054000 	lui	a1,0x4000
84534:	24a5ffff 	addiu	a1,a1,-1
84538:	00852024 	and	a0,a0,a1
8453c:	34050000 	li	a1,0x0
84540:	00a4302a 	slt	a2,a1,a0
84544:	10c00005 	beqz	a2,0x8455c
84548:	00000000 	nop
8454c:	24a50001 	addiu	a1,a1,1
84550:	00a4302a 	slt	a2,a1,a0
84554:	14c0fffe 	bnez	a2,0x84550
84558:	24a50001 	addiu	a1,a1,1
8455c:	12400010 	beqz	s2,0x845a0
84560:	02002025 	move	a0,s0
84564:	2652fffc 	addiu	s2,s2,-4
84568:	5240000c 	beqzl	s2,0x8459c
8456c:	ae000060 	sw	zero,96(s0)
84570:	0c0011e8 	jal	0x47a0
84574:	02402025 	move	a0,s2
84578:	8c440024 	lw	a0,36(v0)
8457c:	8c85001c 	lw	a1,28(a0)
84580:	24a50030 	addiu	a1,a1,48
84584:	84a60000 	lh	a2,0(a1)
84588:	00862021 	addu	a0,a0,a2
8458c:	8ca60004 	lw	a2,4(a1)
84590:	00c0f809 	jalr	a2
84594:	02402825 	move	a1,s2
84598:	ae000060 	sw	zero,96(s0)
8459c:	02002025 	move	a0,s0
845a0:	0c00b034 	jal	0x2c0d0
845a4:	00002825 	move	a1,zero
845a8:	32240001 	andi	a0,s1,0x1
845ac:	1080000d 	beqz	a0,0x845e4
845b0:	00000000 	nop
845b4:	1200000b 	beqz	s0,0x845e4
845b8:	00000000 	nop
845bc:	0c0011e8 	jal	0x47a0
845c0:	02002025 	move	a0,s0
845c4:	8c440024 	lw	a0,36(v0)
845c8:	8c85001c 	lw	a1,28(a0)
845cc:	24a50030 	addiu	a1,a1,48
845d0:	84a60000 	lh	a2,0(a1)
845d4:	00862021 	addu	a0,a0,a2
845d8:	8ca60004 	lw	a2,4(a1)
845dc:	00c0f809 	jalr	a2
845e0:	02002825 	move	a1,s0
845e4:	8fb00000 	lw	s0,0(sp)
845e8:	8fb10004 	lw	s1,4(sp)
845ec:	8fb20008 	lw	s2,8(sp)
845f0:	8fb3000c 	lw	s3,12(sp)
845f4:	8fbf0010 	lw	ra,16(sp)
845f8:	03e00008 	jr	ra
845fc:	27bd0020 	addiu	sp,sp,32
84600:	1000ffff 	b	0x84600
84604:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eReflectionModelMtl::~eReflectionModelMtl(void)
// Address: 0x00084444, Size: 452B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eReflectionModelMtl__PlatformFree_void(void *);   /* extern */
? eTexCoordGen___dtor_eTexCoordGen_void(void *, ?); /* extern */

void eReflectionModelMtl___dtor_eReflectionModelMtl_void(void *arg0, s32 arg1) {
    ? (*temp_a2)(void *, void *, ?);
    ? (*temp_a2_2)(void *, void *, ?);
    ? (*temp_a2_3)(void *, void *, ?);
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a2_2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_a1_3;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_s2_3;
    void *temp_s2_4;

    if (arg0 != NULL) {
        arg0->unk4 = 0x385710;
        eReflectionModelMtl__PlatformFree_void(arg0);
        eTexCoordGen___dtor_eTexCoordGen_void(arg0 + 0x7C, 2);
        arg0->unk4 = 0x380018;
        if ((arg0 + 0x64) != NULL) {
            temp_s2 = arg0->unk64;
            var_a0 = 0;
            if (temp_s2 != NULL) {
                var_a0 = temp_s2->unk-4 & 0x3FFFFFFF;
            }
            var_a1 = 0;
            var_a2 = var_a0 > 0;
            if (var_a2 != 0) {
                var_a1 = 1;
                do {
                    var_a2 = var_a1 < var_a0;
                    var_a1 += 1;
                } while (var_a2 != 0);
            }
            temp_s2_2 = temp_s2 - 4;
            if (temp_s2 != NULL) {
                if (temp_s2_2 != NULL) {
                    temp_a0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_2, var_a1, var_a2)->unk24;
                    temp_a1 = temp_a0->unk1C;
                    temp_a2 = (temp_a1 + 0x30)->unk4;
                    temp_a2(temp_a0 + temp_a1->unk30, temp_s2_2, temp_a2);
                }
                arg0->unk64 = NULL;
            }
        }
        if ((arg0 + 0x60) != NULL) {
            temp_s2_3 = arg0->unk60;
            var_a0_2 = 0;
            if (temp_s2_3 != NULL) {
                var_a0_2 = temp_s2_3->unk-4 & 0x3FFFFFFF;
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
            if (temp_s2_3 != NULL) {
                temp_s2_4 = temp_s2_3 - 4;
                if (temp_s2_4 != NULL) {
                    temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_4, var_a1_2, var_a2_2)->unk24;
                    temp_a1_2 = temp_a0_2->unk1C;
                    temp_a2_2 = (temp_a1_2 + 0x30)->unk4;
                    temp_a2_2(temp_a0_2 + temp_a1_2->unk30, temp_s2_4, temp_a2_2);
                }
                arg0->unk60 = NULL;
            }
        }
        eMaterial___dtor_eMaterial_void(arg0, 0);
        if ((arg1 & 1) && (arg0 != NULL)) {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a1_3 = temp_a0_3->unk1C;
            temp_a2_3 = (temp_a1_3 + 0x30)->unk4;
            temp_a2_3(temp_a0_3 + temp_a1_3->unk30, arg0, temp_a2_3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
