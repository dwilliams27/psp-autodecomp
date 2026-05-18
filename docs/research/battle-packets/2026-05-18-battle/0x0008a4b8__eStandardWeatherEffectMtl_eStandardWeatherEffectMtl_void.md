# Battle Packet: `eStandardWeatherEffectMtl::~eStandardWeatherEffectMtl(void)`

## Target

- Address: `0x0008a4b8`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eStandardWeatherEffectMtl`
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
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`
- `0x0008a460` `matched` 8B `eStandardWeatherEffectMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eStandardWeatherEffectMtl.cpp`
- Header(s): `include/eStandardWeatherEffectMtl.h`
- Allowed source(s): `src/eStandardWeatherEffectMtl.cpp`, `src/eStandardWeatherEffectMtl_dtor.cpp`
- Split-TU prefix: `src/eStandardWeatherEffectMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eStandardWeatherEffectMtl.h`

```cpp
#ifndef ESTANDARDWEATHEREFFECTMTL_H
#define ESTANDARDWEATHEREFFECTMTL_H

class cFile;
class cBase;
class cMemPool;
class cType;

class eStandardWeatherEffectMtl {
public:
    eStandardWeatherEffectMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eStandardWeatherEffectMtl *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0008a260` 88B `eStandardWeatherEffectMtl::Write(cFile &) const` — `src/eStandardWeatherEffectMtl.cpp`
- `0x0008a2b8` 256B `eStandardWeatherEffectMtl::Read(cFile &, cMemPool *)` — `src/eStandardWeatherEffectMtl_Read.cpp`
- `0x0008a3b8` 168B `eStandardWeatherEffectMtl::PlatformRead(cFile &, cMemPool *)` — `src/eStandardWeatherEffectMtl.cpp`
- `0x0008a460` 8B `eStandardWeatherEffectMtl::PlatformFree(void)` — `src/eStandardHeightmapMtl.cpp`
- `0x0008a468` 80B `eStandardWeatherEffectMtl::eStandardWeatherEffectMtl(cBase *)` — `src/eStandardWeatherEffectMtl.cpp`

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

- session `77af6fd6`; src `src/eStandardWeatherEffectMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/77af6fd6/0008a4b8__agent_self_reported_failure__src_eStandardWeatherEffectMtl_dtor.cpp`

category=REG_ALLOC. Added a split-TU canonical C++ destructor with local class redeclaration and inline pool-delete tail, mirroring the eStandardParticleSystemMtl destructor shape with the weather vtable and PlatformFree call. The closest source matches stack frame, control flow, calls, vtable constants, branch-likely forms, array cleanup offsets, eMaterial_dtor tail, delete-tail scheduling, and padding size; final compare is 10/440 bytes different. The remaining bytes are the same first-array saved-register allocation swap: current code uses s3 for the first entries/basePtr value and s2 for p60, while the original uses s2 for entries/basePtr and s3 for p60.

Tried the same restructurings as the particle-system destructor: straight sibling pattern, offset-load memory barriers, p60 lifetime hoist, explicit register bindings/constraints, and an entries lifetime variant. Only the offset barriers improved scheduling; the remaining diff stayed at the 10-byte register rename. Ran `python3 tools/permuter.py src/eStandardWeatherEffectMtl_dtor.cpp 0x0008a4b8 --time 600 --save-best`; it generated 16,944 candidates, compiled 7,211, and found zero improvement from the 10-byte baseline.


## Disassembly

```asm
8a4b8:	27bdffe0 	addiu	sp,sp,-32
8a4bc:	afb00000 	sw	s0,0(sp)
8a4c0:	afb10004 	sw	s1,4(sp)
8a4c4:	afb20008 	sw	s2,8(sp)
8a4c8:	afb3000c 	sw	s3,12(sp)
8a4cc:	afbf0010 	sw	ra,16(sp)
8a4d0:	00808025 	move	s0,a0
8a4d4:	1080005d 	beqz	a0,0x8a64c
8a4d8:	00a08825 	move	s1,a1
8a4dc:	3c040038 	lui	a0,0x38
8a4e0:	24845e30 	addiu	a0,a0,24112
8a4e4:	ae040004 	sw	a0,4(s0)
8a4e8:	0c022918 	jal	0x8a460
8a4ec:	02002025 	move	a0,s0
8a4f0:	3c040038 	lui	a0,0x38
8a4f4:	24840018 	addiu	a0,a0,24
8a4f8:	ae040004 	sw	a0,4(s0)
8a4fc:	26040064 	addiu	a0,s0,100
8a500:	1080001f 	beqz	a0,0x8a580
8a504:	26130060 	addiu	s3,s0,96
8a508:	8e120064 	lw	s2,100(s0)
8a50c:	12400005 	beqz	s2,0x8a524
8a510:	34040000 	li	a0,0x0
8a514:	8e44fffc 	lw	a0,-4(s2)
8a518:	3c054000 	lui	a1,0x4000
8a51c:	24a5ffff 	addiu	a1,a1,-1
8a520:	00852024 	and	a0,a0,a1
8a524:	34050000 	li	a1,0x0
8a528:	00a4302a 	slt	a2,a1,a0
8a52c:	10c00005 	beqz	a2,0x8a544
8a530:	00000000 	nop
8a534:	24a50001 	addiu	a1,a1,1
8a538:	00a4302a 	slt	a2,a1,a0
8a53c:	14c0fffe 	bnez	a2,0x8a538
8a540:	24a50001 	addiu	a1,a1,1
8a544:	1240000e 	beqz	s2,0x8a580
8a548:	2652fffc 	addiu	s2,s2,-4
8a54c:	5240000c 	beqzl	s2,0x8a580
8a550:	ae000064 	sw	zero,100(s0)
8a554:	0c0011e8 	jal	0x47a0
8a558:	02402025 	move	a0,s2
8a55c:	8c440024 	lw	a0,36(v0)
8a560:	8c85001c 	lw	a1,28(a0)
8a564:	24a50030 	addiu	a1,a1,48
8a568:	84a60000 	lh	a2,0(a1)
8a56c:	00862021 	addu	a0,a0,a2
8a570:	8ca60004 	lw	a2,4(a1)
8a574:	00c0f809 	jalr	a2
8a578:	02402825 	move	a1,s2
8a57c:	ae000064 	sw	zero,100(s0)
8a580:	12600021 	beqz	s3,0x8a608
8a584:	02002025 	move	a0,s0
8a588:	8e120060 	lw	s2,96(s0)
8a58c:	12400005 	beqz	s2,0x8a5a4
8a590:	34040000 	li	a0,0x0
8a594:	8e44fffc 	lw	a0,-4(s2)
8a598:	3c054000 	lui	a1,0x4000
8a59c:	24a5ffff 	addiu	a1,a1,-1
8a5a0:	00852024 	and	a0,a0,a1
8a5a4:	34050000 	li	a1,0x0
8a5a8:	00a4302a 	slt	a2,a1,a0
8a5ac:	10c00005 	beqz	a2,0x8a5c4
8a5b0:	00000000 	nop
8a5b4:	24a50001 	addiu	a1,a1,1
8a5b8:	00a4302a 	slt	a2,a1,a0
8a5bc:	14c0fffe 	bnez	a2,0x8a5b8
8a5c0:	24a50001 	addiu	a1,a1,1
8a5c4:	12400010 	beqz	s2,0x8a608
8a5c8:	02002025 	move	a0,s0
8a5cc:	2652fffc 	addiu	s2,s2,-4
8a5d0:	5240000c 	beqzl	s2,0x8a604
8a5d4:	ae000060 	sw	zero,96(s0)
8a5d8:	0c0011e8 	jal	0x47a0
8a5dc:	02402025 	move	a0,s2
8a5e0:	8c440024 	lw	a0,36(v0)
8a5e4:	8c85001c 	lw	a1,28(a0)
8a5e8:	24a50030 	addiu	a1,a1,48
8a5ec:	84a60000 	lh	a2,0(a1)
8a5f0:	00862021 	addu	a0,a0,a2
8a5f4:	8ca60004 	lw	a2,4(a1)
8a5f8:	00c0f809 	jalr	a2
8a5fc:	02402825 	move	a1,s2
8a600:	ae000060 	sw	zero,96(s0)
8a604:	02002025 	move	a0,s0
8a608:	0c00b034 	jal	0x2c0d0
8a60c:	00002825 	move	a1,zero
8a610:	32240001 	andi	a0,s1,0x1
8a614:	1080000d 	beqz	a0,0x8a64c
8a618:	00000000 	nop
8a61c:	1200000b 	beqz	s0,0x8a64c
8a620:	00000000 	nop
8a624:	0c0011e8 	jal	0x47a0
8a628:	02002025 	move	a0,s0
8a62c:	8c440024 	lw	a0,36(v0)
8a630:	8c85001c 	lw	a1,28(a0)
8a634:	24a50030 	addiu	a1,a1,48
8a638:	84a60000 	lh	a2,0(a1)
8a63c:	00862021 	addu	a0,a0,a2
8a640:	8ca60004 	lw	a2,4(a1)
8a644:	00c0f809 	jalr	a2
8a648:	02002825 	move	a1,s0
8a64c:	8fb00000 	lw	s0,0(sp)
8a650:	8fb10004 	lw	s1,4(sp)
8a654:	8fb20008 	lw	s2,8(sp)
8a658:	8fb3000c 	lw	s3,12(sp)
8a65c:	8fbf0010 	lw	ra,16(sp)
8a660:	03e00008 	jr	ra
8a664:	27bd0020 	addiu	sp,sp,32
8a668:	1000ffff 	b	0x8a668
8a66c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eStandardWeatherEffectMtl::~eStandardWeatherEffectMtl(void)
// Address: 0x0008a4b8, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eStandardWeatherEffectMtl__PlatformFree_void(void *); /* extern */

void eStandardWeatherEffectMtl___dtor_eStandardWeatherEffectMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385E30;
        eStandardWeatherEffectMtl__PlatformFree_void(arg0);
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
