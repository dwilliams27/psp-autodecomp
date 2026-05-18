# Battle Packet: `eTwoPassModelMtl::~eTwoPassModelMtl(void)`

## Target

- Address: `0x0008b384`
- Size: `464` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eTwoPassModelMtl`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`
- `0x00031f10` `failed` 100B `eTexCoordGen::~eTexCoordGen(void)`
- `0x0008b300` `matched` 8B `eTwoPassModelMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eTwoPassModelMtl.cpp`
- Header(s): `include/eTwoPassModelMtl.h`
- Allowed source(s): `src/eTwoPassModelMtl.cpp`, `src/eTwoPassModelMtl_dtor.cpp`
- Split-TU prefix: `src/eTwoPassModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eTwoPassModelMtl.h`

```cpp
#ifndef ETWOPASSMODELMTL_H
#define ETWOPASSMODELMTL_H

class cBase;
class cFile;
class cMemPool;
class eDrawInfo;
class mOCS;
class eColor;
class cType;

class eTwoPassModelMtl {
public:
    eTwoPassModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    static eTwoPassModelMtl *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0008afb8` 204B `eTwoPassModelMtl::Write(cFile &) const` — `src/eTwoPassModelMtl.cpp`
- `0x0008b084` 468B `eTwoPassModelMtl::Read(cFile &, cMemPool *)` — `src/eTwoPassModelMtl_Read.cpp`
- `0x0008b258` 168B `eTwoPassModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eTwoPassModelMtl.cpp`
- `0x0008b300` 8B `eTwoPassModelMtl::PlatformFree(void)` — `src/eStandardWeatherEffectMtl.cpp`
- `0x0008b308` 124B `eTwoPassModelMtl::eTwoPassModelMtl(cBase *)` — `src/eTwoPassModelMtl.cpp`

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

- session `10519bd5`; src `src/eTwoPassModelMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/10519bd5/0008b384__agent_self_reported_failure__src_eTwoPassModelMtl_dtor.cpp`

category=REG_ALLOC. 10/464 byte diff after relocation masking, all single-byte register-field renames (s2 <-> s3) at offsets 0x64, 0x68, 0x6c, 0x74, 0xa4, 0xa8, 0xac, 0xb8, 0xd8, 0xe0. Same pattern as eStandardHeightmapMtl_dtor.cpp and the matched-shape siblings eProjectedModelMtl_dtor.cpp / eShadowVolumeModelMtl_dtor.cpp: SNC cached the second array's base in a callee-save reg across the first array's work and chose s2 where the original picked s3. Function has the additional eTexCoordGen_dtor(this+0xF0, 2) and eTexCoordGen_dtor(this+0x7C, 2) calls between PlatformFree and the second vtable swap; those bytes match. Permuter ran 300.6s, 2697 compiled candidates, 0 improvements (best=10 throughout). Per docs/research/snc-register-allocation.md the recommended action once the diff collapses to callee-save renames is to stop and classify REG_ALLOC; further source perturbation risks regressing matched siblings (ADR-012). Source preserved at src/eTwoPassModelMtl_dtor.cpp.


## Disassembly

```asm
8b384:	27bdffe0 	addiu	sp,sp,-32
8b388:	afb00000 	sw	s0,0(sp)
8b38c:	afb10004 	sw	s1,4(sp)
8b390:	afb20008 	sw	s2,8(sp)
8b394:	afb3000c 	sw	s3,12(sp)
8b398:	afbf0010 	sw	ra,16(sp)
8b39c:	00808025 	move	s0,a0
8b3a0:	10800063 	beqz	a0,0x8b530
8b3a4:	00a08825 	move	s1,a1
8b3a8:	3c040038 	lui	a0,0x38
8b3ac:	24845ee8 	addiu	a0,a0,24296
8b3b0:	ae040004 	sw	a0,4(s0)
8b3b4:	0c022cc0 	jal	0x8b300
8b3b8:	02002025 	move	a0,s0
8b3bc:	260400f0 	addiu	a0,s0,240
8b3c0:	0c00c7c4 	jal	0x31f10
8b3c4:	34050002 	li	a1,0x2
8b3c8:	2604007c 	addiu	a0,s0,124
8b3cc:	0c00c7c4 	jal	0x31f10
8b3d0:	34050002 	li	a1,0x2
8b3d4:	3c040038 	lui	a0,0x38
8b3d8:	24840018 	addiu	a0,a0,24
8b3dc:	ae040004 	sw	a0,4(s0)
8b3e0:	26040064 	addiu	a0,s0,100
8b3e4:	1080001f 	beqz	a0,0x8b464
8b3e8:	26130060 	addiu	s3,s0,96
8b3ec:	8e120064 	lw	s2,100(s0)
8b3f0:	12400005 	beqz	s2,0x8b408
8b3f4:	34040000 	li	a0,0x0
8b3f8:	8e44fffc 	lw	a0,-4(s2)
8b3fc:	3c054000 	lui	a1,0x4000
8b400:	24a5ffff 	addiu	a1,a1,-1
8b404:	00852024 	and	a0,a0,a1
8b408:	34050000 	li	a1,0x0
8b40c:	00a4302a 	slt	a2,a1,a0
8b410:	10c00005 	beqz	a2,0x8b428
8b414:	00000000 	nop
8b418:	24a50001 	addiu	a1,a1,1
8b41c:	00a4302a 	slt	a2,a1,a0
8b420:	14c0fffe 	bnez	a2,0x8b41c
8b424:	24a50001 	addiu	a1,a1,1
8b428:	1240000e 	beqz	s2,0x8b464
8b42c:	2652fffc 	addiu	s2,s2,-4
8b430:	5240000c 	beqzl	s2,0x8b464
8b434:	ae000064 	sw	zero,100(s0)
8b438:	0c0011e8 	jal	0x47a0
8b43c:	02402025 	move	a0,s2
8b440:	8c440024 	lw	a0,36(v0)
8b444:	8c85001c 	lw	a1,28(a0)
8b448:	24a50030 	addiu	a1,a1,48
8b44c:	84a60000 	lh	a2,0(a1)
8b450:	00862021 	addu	a0,a0,a2
8b454:	8ca60004 	lw	a2,4(a1)
8b458:	00c0f809 	jalr	a2
8b45c:	02402825 	move	a1,s2
8b460:	ae000064 	sw	zero,100(s0)
8b464:	12600021 	beqz	s3,0x8b4ec
8b468:	02002025 	move	a0,s0
8b46c:	8e120060 	lw	s2,96(s0)
8b470:	12400005 	beqz	s2,0x8b488
8b474:	34040000 	li	a0,0x0
8b478:	8e44fffc 	lw	a0,-4(s2)
8b47c:	3c054000 	lui	a1,0x4000
8b480:	24a5ffff 	addiu	a1,a1,-1
8b484:	00852024 	and	a0,a0,a1
8b488:	34050000 	li	a1,0x0
8b48c:	00a4302a 	slt	a2,a1,a0
8b490:	10c00005 	beqz	a2,0x8b4a8
8b494:	00000000 	nop
8b498:	24a50001 	addiu	a1,a1,1
8b49c:	00a4302a 	slt	a2,a1,a0
8b4a0:	14c0fffe 	bnez	a2,0x8b49c
8b4a4:	24a50001 	addiu	a1,a1,1
8b4a8:	12400010 	beqz	s2,0x8b4ec
8b4ac:	02002025 	move	a0,s0
8b4b0:	2652fffc 	addiu	s2,s2,-4
8b4b4:	5240000c 	beqzl	s2,0x8b4e8
8b4b8:	ae000060 	sw	zero,96(s0)
8b4bc:	0c0011e8 	jal	0x47a0
8b4c0:	02402025 	move	a0,s2
8b4c4:	8c440024 	lw	a0,36(v0)
8b4c8:	8c85001c 	lw	a1,28(a0)
8b4cc:	24a50030 	addiu	a1,a1,48
8b4d0:	84a60000 	lh	a2,0(a1)
8b4d4:	00862021 	addu	a0,a0,a2
8b4d8:	8ca60004 	lw	a2,4(a1)
8b4dc:	00c0f809 	jalr	a2
8b4e0:	02402825 	move	a1,s2
8b4e4:	ae000060 	sw	zero,96(s0)
8b4e8:	02002025 	move	a0,s0
8b4ec:	0c00b034 	jal	0x2c0d0
8b4f0:	00002825 	move	a1,zero
8b4f4:	32240001 	andi	a0,s1,0x1
8b4f8:	1080000d 	beqz	a0,0x8b530
8b4fc:	00000000 	nop
8b500:	1200000b 	beqz	s0,0x8b530
8b504:	00000000 	nop
8b508:	0c0011e8 	jal	0x47a0
8b50c:	02002025 	move	a0,s0
8b510:	8c440024 	lw	a0,36(v0)
8b514:	8c85001c 	lw	a1,28(a0)
8b518:	24a50030 	addiu	a1,a1,48
8b51c:	84a60000 	lh	a2,0(a1)
8b520:	00862021 	addu	a0,a0,a2
8b524:	8ca60004 	lw	a2,4(a1)
8b528:	00c0f809 	jalr	a2
8b52c:	02002825 	move	a1,s0
8b530:	8fb00000 	lw	s0,0(sp)
8b534:	8fb10004 	lw	s1,4(sp)
8b538:	8fb20008 	lw	s2,8(sp)
8b53c:	8fb3000c 	lw	s3,12(sp)
8b540:	8fbf0010 	lw	ra,16(sp)
8b544:	03e00008 	jr	ra
8b548:	27bd0020 	addiu	sp,sp,32
8b54c:	1000ffff 	b	0x8b54c
8b550:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eTwoPassModelMtl::~eTwoPassModelMtl(void)
// Address: 0x0008b384, Size: 464B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eTexCoordGen___dtor_eTexCoordGen_void(void *, ?); /* extern */
? eTwoPassModelMtl__PlatformFree_void(void *);      /* extern */

void eTwoPassModelMtl___dtor_eTwoPassModelMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385EE8;
        eTwoPassModelMtl__PlatformFree_void(arg0);
        eTexCoordGen___dtor_eTexCoordGen_void(arg0 + 0xF0, 2);
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
