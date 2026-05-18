# Battle Packet: `eStandardParticleSystemMtl::~eStandardParticleSystemMtl(void)`

## Target

- Address: `0x0008974c`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eStandardParticleSystemMtl`
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
- `0x000896f4` `matched` 8B `eStandardParticleSystemMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eStandardParticleSystemMtl.cpp`
- Header(s): `include/eStandardParticleSystemMtl.h`
- Allowed source(s): `src/eStandardParticleSystemMtl.cpp`, `src/eStandardParticleSystemMtl_dtor.cpp`
- Split-TU prefix: `src/eStandardParticleSystemMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eStandardParticleSystemMtl.h`

```cpp
#ifndef ESTANDARDPARTICLESYSTEMMTL_H
#define ESTANDARDPARTICLESYSTEMMTL_H

class cBase;
class cFile;
class cMemPool;
class cType;

class eStandardParticleSystemMtl {
public:
    eStandardParticleSystemMtl(cBase *);
    const cType *GetType(void) const;
    void CreateData(void);
    void PlatformFree(void);
    void PlatformRead(cFile &, cMemPool *);
    void Unapply(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eStandardParticleSystemMtl *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x000894f4` 88B `eStandardParticleSystemMtl::Write(cFile &) const` — `src/eStandardParticleSystemMtl.cpp`
- `0x0008954c` 256B `eStandardParticleSystemMtl::Read(cFile &, cMemPool *)` — `src/eStandardHeightmapMtl.cpp`
- `0x0008964c` 168B `eStandardParticleSystemMtl::PlatformRead(cFile &, cMemPool *)` — `src/eStandardParticleSystemMtl.cpp`
- `0x000896f4` 8B `eStandardParticleSystemMtl::PlatformFree(void)` — `src/eStandardHeightmapMtl.cpp`
- `0x000896fc` 80B `eStandardParticleSystemMtl::eStandardParticleSystemMtl(cBase *)` — `src/eStandardParticleSystemMtl.cpp`

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

- session `77af6fd6`; src `src/eStandardParticleSystemMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/77af6fd6/0008974c__agent_self_reported_failure__src_eStandardParticleSystemMtl_dtor.cpp`

category=REG_ALLOC. Added a split-TU canonical C++ destructor with local class redeclaration and inline pool-delete tail, following the matched material destructor siblings. The closest source matches stack frame, control flow, calls, vtable constants, branch-likely forms, array cleanup offsets, eMaterial_dtor tail, delete-tail scheduling, and padding size; final compare is 10/440 bytes different. The remaining bytes are the first array cleanup's saved-register allocation: current code uses s3 for the first entries/basePtr value and s2 for p60, while the original uses s2 for entries/basePtr and s3 for p60. The second array block and final delete tail already use the expected scheduling/register form.

Tried the straight sibling pattern, adding memory barriers after DeleteRec::offset loads to force the original delete-call scheduling, hoisting p60 after the first cleanup, explicit register bindings/constraints for p60 and entries, and an entries lifetime variant. The hoist and constraint variants worsened the function shape; the sibling pattern plus offset barriers returns to the 10-byte saved-register rename. Ran `python3 tools/permuter.py src/eStandardParticleSystemMtl_dtor.cpp 0x0008974c --time 600 --save-best`; it generated 17,304 candidates, compiled 7,399, and found zero improvement from the 10-byte baseline.


## Disassembly

```asm
8974c:	27bdffe0 	addiu	sp,sp,-32
89750:	afb00000 	sw	s0,0(sp)
89754:	afb10004 	sw	s1,4(sp)
89758:	afb20008 	sw	s2,8(sp)
8975c:	afb3000c 	sw	s3,12(sp)
89760:	afbf0010 	sw	ra,16(sp)
89764:	00808025 	move	s0,a0
89768:	1080005d 	beqz	a0,0x898e0
8976c:	00a08825 	move	s1,a1
89770:	3c040038 	lui	a0,0x38
89774:	24845d78 	addiu	a0,a0,23928
89778:	ae040004 	sw	a0,4(s0)
8977c:	0c0225bd 	jal	0x896f4
89780:	02002025 	move	a0,s0
89784:	3c040038 	lui	a0,0x38
89788:	24840018 	addiu	a0,a0,24
8978c:	ae040004 	sw	a0,4(s0)
89790:	26040064 	addiu	a0,s0,100
89794:	1080001f 	beqz	a0,0x89814
89798:	26130060 	addiu	s3,s0,96
8979c:	8e120064 	lw	s2,100(s0)
897a0:	12400005 	beqz	s2,0x897b8
897a4:	34040000 	li	a0,0x0
897a8:	8e44fffc 	lw	a0,-4(s2)
897ac:	3c054000 	lui	a1,0x4000
897b0:	24a5ffff 	addiu	a1,a1,-1
897b4:	00852024 	and	a0,a0,a1
897b8:	34050000 	li	a1,0x0
897bc:	00a4302a 	slt	a2,a1,a0
897c0:	10c00005 	beqz	a2,0x897d8
897c4:	00000000 	nop
897c8:	24a50001 	addiu	a1,a1,1
897cc:	00a4302a 	slt	a2,a1,a0
897d0:	14c0fffe 	bnez	a2,0x897cc
897d4:	24a50001 	addiu	a1,a1,1
897d8:	1240000e 	beqz	s2,0x89814
897dc:	2652fffc 	addiu	s2,s2,-4
897e0:	5240000c 	beqzl	s2,0x89814
897e4:	ae000064 	sw	zero,100(s0)
897e8:	0c0011e8 	jal	0x47a0
897ec:	02402025 	move	a0,s2
897f0:	8c440024 	lw	a0,36(v0)
897f4:	8c85001c 	lw	a1,28(a0)
897f8:	24a50030 	addiu	a1,a1,48
897fc:	84a60000 	lh	a2,0(a1)
89800:	00862021 	addu	a0,a0,a2
89804:	8ca60004 	lw	a2,4(a1)
89808:	00c0f809 	jalr	a2
8980c:	02402825 	move	a1,s2
89810:	ae000064 	sw	zero,100(s0)
89814:	12600021 	beqz	s3,0x8989c
89818:	02002025 	move	a0,s0
8981c:	8e120060 	lw	s2,96(s0)
89820:	12400005 	beqz	s2,0x89838
89824:	34040000 	li	a0,0x0
89828:	8e44fffc 	lw	a0,-4(s2)
8982c:	3c054000 	lui	a1,0x4000
89830:	24a5ffff 	addiu	a1,a1,-1
89834:	00852024 	and	a0,a0,a1
89838:	34050000 	li	a1,0x0
8983c:	00a4302a 	slt	a2,a1,a0
89840:	10c00005 	beqz	a2,0x89858
89844:	00000000 	nop
89848:	24a50001 	addiu	a1,a1,1
8984c:	00a4302a 	slt	a2,a1,a0
89850:	14c0fffe 	bnez	a2,0x8984c
89854:	24a50001 	addiu	a1,a1,1
89858:	12400010 	beqz	s2,0x8989c
8985c:	02002025 	move	a0,s0
89860:	2652fffc 	addiu	s2,s2,-4
89864:	5240000c 	beqzl	s2,0x89898
89868:	ae000060 	sw	zero,96(s0)
8986c:	0c0011e8 	jal	0x47a0
89870:	02402025 	move	a0,s2
89874:	8c440024 	lw	a0,36(v0)
89878:	8c85001c 	lw	a1,28(a0)
8987c:	24a50030 	addiu	a1,a1,48
89880:	84a60000 	lh	a2,0(a1)
89884:	00862021 	addu	a0,a0,a2
89888:	8ca60004 	lw	a2,4(a1)
8988c:	00c0f809 	jalr	a2
89890:	02402825 	move	a1,s2
89894:	ae000060 	sw	zero,96(s0)
89898:	02002025 	move	a0,s0
8989c:	0c00b034 	jal	0x2c0d0
898a0:	00002825 	move	a1,zero
898a4:	32240001 	andi	a0,s1,0x1
898a8:	1080000d 	beqz	a0,0x898e0
898ac:	00000000 	nop
898b0:	1200000b 	beqz	s0,0x898e0
898b4:	00000000 	nop
898b8:	0c0011e8 	jal	0x47a0
898bc:	02002025 	move	a0,s0
898c0:	8c440024 	lw	a0,36(v0)
898c4:	8c85001c 	lw	a1,28(a0)
898c8:	24a50030 	addiu	a1,a1,48
898cc:	84a60000 	lh	a2,0(a1)
898d0:	00862021 	addu	a0,a0,a2
898d4:	8ca60004 	lw	a2,4(a1)
898d8:	00c0f809 	jalr	a2
898dc:	02002825 	move	a1,s0
898e0:	8fb00000 	lw	s0,0(sp)
898e4:	8fb10004 	lw	s1,4(sp)
898e8:	8fb20008 	lw	s2,8(sp)
898ec:	8fb3000c 	lw	s3,12(sp)
898f0:	8fbf0010 	lw	ra,16(sp)
898f4:	03e00008 	jr	ra
898f8:	27bd0020 	addiu	sp,sp,32
898fc:	1000ffff 	b	0x898fc
89900:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eStandardParticleSystemMtl::~eStandardParticleSystemMtl(void)
// Address: 0x0008974c, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eStandardParticleSystemMtl__PlatformFree_void(void *); /* extern */

void eStandardParticleSystemMtl___dtor_eStandardParticleSystemMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385D78;
        eStandardParticleSystemMtl__PlatformFree_void(arg0);
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
