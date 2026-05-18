# Battle Packet: `eRefractionModelMtl::~eRefractionModelMtl(void)`

## Target

- Address: `0x000855e4`
- Size: `452` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eRefractionModelMtl`
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
- `0x0008555c` `matched` 8B `eRefractionModelMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eRefractionModelMtl.cpp`
- Header(s): `include/eRefractionModelMtl.h`
- Allowed source(s): `src/eRefractionModelMtl.cpp`, `src/eRefractionModelMtl_dtor.cpp`
- Split-TU prefix: `src/eRefractionModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eRefractionModelMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x00085228` 184B `eRefractionModelMtl::Write(cFile &) const` — `src/eRefractionModelMtl.cpp`
- `0x000852e0` 440B `eRefractionModelMtl::Read(cFile &, cMemPool *)` — `src/eRefractionModelMtl_Read.cpp`
- `0x00085498` 196B `eRefractionModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eRefractionModelMtl.cpp`
- `0x0008555c` 8B `eRefractionModelMtl::PlatformFree(void)` — `src/eRefractionModelMtl_PlatformFree.cpp`
- `0x00085564` 128B `eRefractionModelMtl::eRefractionModelMtl(cBase *)` — `src/eRefractionModelMtl.cpp`

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

- session `4183810d`; src `src/eRefractionModelMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/4183810d/000855e4__agent_self_reported_failure__src_eRefractionModelMtl_dtor.cpp`

category=REG_ALLOC. Implemented the same canonical split-TU destructor as eReflectionModelMtl with the refraction-specific vtable 0x385878 and PlatformFree() call. Final compare is 10/452 bytes different with the same saved-register allocation shape as reflection: current uses s2 for p60 and s3 for the first entries pointer, while the original uses s3 for p60 and s2 for entries. The function shape, calls, stack frame, immediates, offsets, branch-likely forms, array cleanup, eMaterial_dtor tail, and operator-delete tail otherwise match.

Tried the straight sibling pattern, explicit register declarations for p60/entries, stronger no-op register constraints, and a hoisted-second-array source shape. The bad variants produced ~299-309 byte diffs, and the closest source returns to the same 10-byte register rename. Because this source is structurally identical to the reflection destructor except for vtable/callee and reflection's 300.6s permuter pass found zero improvement, this is classified as the same retryable REG_ALLOC drift rather than bnel/unmatchable.


## Disassembly

```asm
855e4:	27bdffe0 	addiu	sp,sp,-32
855e8:	afb00000 	sw	s0,0(sp)
855ec:	afb10004 	sw	s1,4(sp)
855f0:	afb20008 	sw	s2,8(sp)
855f4:	afb3000c 	sw	s3,12(sp)
855f8:	afbf0010 	sw	ra,16(sp)
855fc:	00808025 	move	s0,a0
85600:	10800060 	beqz	a0,0x85784
85604:	00a08825 	move	s1,a1
85608:	3c040038 	lui	a0,0x38
8560c:	24845878 	addiu	a0,a0,22648
85610:	ae040004 	sw	a0,4(s0)
85614:	0c021557 	jal	0x8555c
85618:	02002025 	move	a0,s0
8561c:	2604007c 	addiu	a0,s0,124
85620:	0c00c7c4 	jal	0x31f10
85624:	34050002 	li	a1,0x2
85628:	3c040038 	lui	a0,0x38
8562c:	24840018 	addiu	a0,a0,24
85630:	ae040004 	sw	a0,4(s0)
85634:	26040064 	addiu	a0,s0,100
85638:	1080001f 	beqz	a0,0x856b8
8563c:	26130060 	addiu	s3,s0,96
85640:	8e120064 	lw	s2,100(s0)
85644:	12400005 	beqz	s2,0x8565c
85648:	34040000 	li	a0,0x0
8564c:	8e44fffc 	lw	a0,-4(s2)
85650:	3c054000 	lui	a1,0x4000
85654:	24a5ffff 	addiu	a1,a1,-1
85658:	00852024 	and	a0,a0,a1
8565c:	34050000 	li	a1,0x0
85660:	00a4302a 	slt	a2,a1,a0
85664:	10c00005 	beqz	a2,0x8567c
85668:	00000000 	nop
8566c:	24a50001 	addiu	a1,a1,1
85670:	00a4302a 	slt	a2,a1,a0
85674:	14c0fffe 	bnez	a2,0x85670
85678:	24a50001 	addiu	a1,a1,1
8567c:	1240000e 	beqz	s2,0x856b8
85680:	2652fffc 	addiu	s2,s2,-4
85684:	5240000c 	beqzl	s2,0x856b8
85688:	ae000064 	sw	zero,100(s0)
8568c:	0c0011e8 	jal	0x47a0
85690:	02402025 	move	a0,s2
85694:	8c440024 	lw	a0,36(v0)
85698:	8c85001c 	lw	a1,28(a0)
8569c:	24a50030 	addiu	a1,a1,48
856a0:	84a60000 	lh	a2,0(a1)
856a4:	00862021 	addu	a0,a0,a2
856a8:	8ca60004 	lw	a2,4(a1)
856ac:	00c0f809 	jalr	a2
856b0:	02402825 	move	a1,s2
856b4:	ae000064 	sw	zero,100(s0)
856b8:	12600021 	beqz	s3,0x85740
856bc:	02002025 	move	a0,s0
856c0:	8e120060 	lw	s2,96(s0)
856c4:	12400005 	beqz	s2,0x856dc
856c8:	34040000 	li	a0,0x0
856cc:	8e44fffc 	lw	a0,-4(s2)
856d0:	3c054000 	lui	a1,0x4000
856d4:	24a5ffff 	addiu	a1,a1,-1
856d8:	00852024 	and	a0,a0,a1
856dc:	34050000 	li	a1,0x0
856e0:	00a4302a 	slt	a2,a1,a0
856e4:	10c00005 	beqz	a2,0x856fc
856e8:	00000000 	nop
856ec:	24a50001 	addiu	a1,a1,1
856f0:	00a4302a 	slt	a2,a1,a0
856f4:	14c0fffe 	bnez	a2,0x856f0
856f8:	24a50001 	addiu	a1,a1,1
856fc:	12400010 	beqz	s2,0x85740
85700:	02002025 	move	a0,s0
85704:	2652fffc 	addiu	s2,s2,-4
85708:	5240000c 	beqzl	s2,0x8573c
8570c:	ae000060 	sw	zero,96(s0)
85710:	0c0011e8 	jal	0x47a0
85714:	02402025 	move	a0,s2
85718:	8c440024 	lw	a0,36(v0)
8571c:	8c85001c 	lw	a1,28(a0)
85720:	24a50030 	addiu	a1,a1,48
85724:	84a60000 	lh	a2,0(a1)
85728:	00862021 	addu	a0,a0,a2
8572c:	8ca60004 	lw	a2,4(a1)
85730:	00c0f809 	jalr	a2
85734:	02402825 	move	a1,s2
85738:	ae000060 	sw	zero,96(s0)
8573c:	02002025 	move	a0,s0
85740:	0c00b034 	jal	0x2c0d0
85744:	00002825 	move	a1,zero
85748:	32240001 	andi	a0,s1,0x1
8574c:	1080000d 	beqz	a0,0x85784
85750:	00000000 	nop
85754:	1200000b 	beqz	s0,0x85784
85758:	00000000 	nop
8575c:	0c0011e8 	jal	0x47a0
85760:	02002025 	move	a0,s0
85764:	8c440024 	lw	a0,36(v0)
85768:	8c85001c 	lw	a1,28(a0)
8576c:	24a50030 	addiu	a1,a1,48
85770:	84a60000 	lh	a2,0(a1)
85774:	00862021 	addu	a0,a0,a2
85778:	8ca60004 	lw	a2,4(a1)
8577c:	00c0f809 	jalr	a2
85780:	02002825 	move	a1,s0
85784:	8fb00000 	lw	s0,0(sp)
85788:	8fb10004 	lw	s1,4(sp)
8578c:	8fb20008 	lw	s2,8(sp)
85790:	8fb3000c 	lw	s3,12(sp)
85794:	8fbf0010 	lw	ra,16(sp)
85798:	03e00008 	jr	ra
8579c:	27bd0020 	addiu	sp,sp,32
857a0:	1000ffff 	b	0x857a0
857a4:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eRefractionModelMtl::~eRefractionModelMtl(void)
// Address: 0x000855e4, Size: 452B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eRefractionModelMtl__PlatformFree_void(void *);   /* extern */
? eTexCoordGen___dtor_eTexCoordGen_void(void *, ?); /* extern */

void eRefractionModelMtl___dtor_eRefractionModelMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385878;
        eRefractionModelMtl__PlatformFree_void(arg0);
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
