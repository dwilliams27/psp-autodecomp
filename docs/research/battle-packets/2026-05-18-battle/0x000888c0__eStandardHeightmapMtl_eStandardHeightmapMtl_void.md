# Battle Packet: `eStandardHeightmapMtl::~eStandardHeightmapMtl(void)`

## Target

- Address: `0x000888c0`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eStandardHeightmapMtl`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; codegen-specific diagnosis; manageable size`
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
- `0x00088874` `matched` 8B `eStandardHeightmapMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eStandardHeightmapMtl.cpp`
- Header(s): `include/eStandardHeightmapMtl.h`
- Allowed source(s): `src/eStandardHeightmapMtl.cpp`, `src/eStandardHeightmapMtl_dtor.cpp`
- Split-TU prefix: `src/eStandardHeightmapMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eStandardHeightmapMtl.h`

```cpp
#ifndef ESTANDARDHEIGHTMAPMTL_H
#define ESTANDARDHEIGHTMAPMTL_H

class cBase;
class cFile;
class cMemPool;
class cType;

class eStandardHeightmapMtl {
public:
    eStandardHeightmapMtl(cBase *);
    const cType *GetType(void) const;
    void CreateData(void);
    void PlatformFree(void);
    void Unapply(void) const;
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static eStandardHeightmapMtl *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00088674` 88B `eStandardHeightmapMtl::Write(cFile &) const` — `src/eStandardHeightmapMtl.cpp`
- `0x000886cc` 256B `eStandardHeightmapMtl::Read(cFile &, cMemPool *)` — `src/eStandardHeightmapMtl.cpp`
- `0x000887cc` 168B `eStandardHeightmapMtl::PlatformRead(cFile &, cMemPool *)` — `src/eStandardHeightmapMtl.cpp`
- `0x00088874` 8B `eStandardHeightmapMtl::PlatformFree(void)` — `src/eStandardGeomTrailMtl.cpp`
- `0x0008887c` 68B `eStandardHeightmapMtl::eStandardHeightmapMtl(cBase *)` — `src/eStandardHeightmapMtl.cpp`

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

- session `10519bd5`; src `src/eStandardHeightmapMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/10519bd5/000888c0__agent_self_reported_failure__src_eStandardHeightmapMtl_dtor.cpp`

category=REG_ALLOC. 10/440 byte diff after relocation masking, all single-byte register-field renames (s2 <-> s3) in the array-processing block (offsets 0x4c, 0x50, 0x54, 0x5c, 0x8c, 0x90, 0x94, 0xa0, 0xc0, 0xc8). Pattern: SNC cached the second array's base address (this+0x60) in a callee-save reg across the first array's work; in our build it lands in s2, expected has s3, and the working 'entries' variable swaps the other way. Source structure (split-TU, ODR-isolated local class redecl, eMaterial_dtor extern, inline operator delete, sched=1 pragma) mirrors the close-but-not-matching siblings eProjectedModelMtl_dtor.cpp and eShadowVolumeModelMtl_dtor.cpp, which produce the IDENTICAL 10-byte diff. Tried three source restructurings: (1) swap arr60Addr/arr64Addr declaration order, (2) hoist 'entries' to outer scope as a shared local, (3) keep entries block-scoped. None moved the allocator. Permuter ran 300.5s, 3865 compiled candidates, 0 improvements (best=10 throughout). This matches the documented REG_ALLOC drift in docs/research/snc-register-allocation.md and ADR-012 (context-dependent SNC register allocator). Source for retry is preserved in src/eStandardHeightmapMtl_dtor.cpp; the .size and trap-pattern __asm__ are in place so the only delta is the s2/s3 tie-break.


## Disassembly

```asm
888c0:	27bdffe0 	addiu	sp,sp,-32
888c4:	afb00000 	sw	s0,0(sp)
888c8:	afb10004 	sw	s1,4(sp)
888cc:	afb20008 	sw	s2,8(sp)
888d0:	afb3000c 	sw	s3,12(sp)
888d4:	afbf0010 	sw	ra,16(sp)
888d8:	00808025 	move	s0,a0
888dc:	1080005d 	beqz	a0,0x88a54
888e0:	00a08825 	move	s1,a1
888e4:	3c040038 	lui	a0,0x38
888e8:	24845cc0 	addiu	a0,a0,23744
888ec:	ae040004 	sw	a0,4(s0)
888f0:	0c02221d 	jal	0x88874
888f4:	02002025 	move	a0,s0
888f8:	3c040038 	lui	a0,0x38
888fc:	24840018 	addiu	a0,a0,24
88900:	ae040004 	sw	a0,4(s0)
88904:	26040064 	addiu	a0,s0,100
88908:	1080001f 	beqz	a0,0x88988
8890c:	26130060 	addiu	s3,s0,96
88910:	8e120064 	lw	s2,100(s0)
88914:	12400005 	beqz	s2,0x8892c
88918:	34040000 	li	a0,0x0
8891c:	8e44fffc 	lw	a0,-4(s2)
88920:	3c054000 	lui	a1,0x4000
88924:	24a5ffff 	addiu	a1,a1,-1
88928:	00852024 	and	a0,a0,a1
8892c:	34050000 	li	a1,0x0
88930:	00a4302a 	slt	a2,a1,a0
88934:	10c00005 	beqz	a2,0x8894c
88938:	00000000 	nop
8893c:	24a50001 	addiu	a1,a1,1
88940:	00a4302a 	slt	a2,a1,a0
88944:	14c0fffe 	bnez	a2,0x88940
88948:	24a50001 	addiu	a1,a1,1
8894c:	1240000e 	beqz	s2,0x88988
88950:	2652fffc 	addiu	s2,s2,-4
88954:	5240000c 	beqzl	s2,0x88988
88958:	ae000064 	sw	zero,100(s0)
8895c:	0c0011e8 	jal	0x47a0
88960:	02402025 	move	a0,s2
88964:	8c440024 	lw	a0,36(v0)
88968:	8c85001c 	lw	a1,28(a0)
8896c:	24a50030 	addiu	a1,a1,48
88970:	84a60000 	lh	a2,0(a1)
88974:	00862021 	addu	a0,a0,a2
88978:	8ca60004 	lw	a2,4(a1)
8897c:	00c0f809 	jalr	a2
88980:	02402825 	move	a1,s2
88984:	ae000064 	sw	zero,100(s0)
88988:	12600021 	beqz	s3,0x88a10
8898c:	02002025 	move	a0,s0
88990:	8e120060 	lw	s2,96(s0)
88994:	12400005 	beqz	s2,0x889ac
88998:	34040000 	li	a0,0x0
8899c:	8e44fffc 	lw	a0,-4(s2)
889a0:	3c054000 	lui	a1,0x4000
889a4:	24a5ffff 	addiu	a1,a1,-1
889a8:	00852024 	and	a0,a0,a1
889ac:	34050000 	li	a1,0x0
889b0:	00a4302a 	slt	a2,a1,a0
889b4:	10c00005 	beqz	a2,0x889cc
889b8:	00000000 	nop
889bc:	24a50001 	addiu	a1,a1,1
889c0:	00a4302a 	slt	a2,a1,a0
889c4:	14c0fffe 	bnez	a2,0x889c0
889c8:	24a50001 	addiu	a1,a1,1
889cc:	12400010 	beqz	s2,0x88a10
889d0:	02002025 	move	a0,s0
889d4:	2652fffc 	addiu	s2,s2,-4
889d8:	5240000c 	beqzl	s2,0x88a0c
889dc:	ae000060 	sw	zero,96(s0)
889e0:	0c0011e8 	jal	0x47a0
889e4:	02402025 	move	a0,s2
889e8:	8c440024 	lw	a0,36(v0)
889ec:	8c85001c 	lw	a1,28(a0)
889f0:	24a50030 	addiu	a1,a1,48
889f4:	84a60000 	lh	a2,0(a1)
889f8:	00862021 	addu	a0,a0,a2
889fc:	8ca60004 	lw	a2,4(a1)
88a00:	00c0f809 	jalr	a2
88a04:	02402825 	move	a1,s2
88a08:	ae000060 	sw	zero,96(s0)
88a0c:	02002025 	move	a0,s0
88a10:	0c00b034 	jal	0x2c0d0
88a14:	00002825 	move	a1,zero
88a18:	32240001 	andi	a0,s1,0x1
88a1c:	1080000d 	beqz	a0,0x88a54
88a20:	00000000 	nop
88a24:	1200000b 	beqz	s0,0x88a54
88a28:	00000000 	nop
88a2c:	0c0011e8 	jal	0x47a0
88a30:	02002025 	move	a0,s0
88a34:	8c440024 	lw	a0,36(v0)
88a38:	8c85001c 	lw	a1,28(a0)
88a3c:	24a50030 	addiu	a1,a1,48
88a40:	84a60000 	lh	a2,0(a1)
88a44:	00862021 	addu	a0,a0,a2
88a48:	8ca60004 	lw	a2,4(a1)
88a4c:	00c0f809 	jalr	a2
88a50:	02002825 	move	a1,s0
88a54:	8fb00000 	lw	s0,0(sp)
88a58:	8fb10004 	lw	s1,4(sp)
88a5c:	8fb20008 	lw	s2,8(sp)
88a60:	8fb3000c 	lw	s3,12(sp)
88a64:	8fbf0010 	lw	ra,16(sp)
88a68:	03e00008 	jr	ra
88a6c:	27bd0020 	addiu	sp,sp,32
88a70:	1000ffff 	b	0x88a70
88a74:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eStandardHeightmapMtl::~eStandardHeightmapMtl(void)
// Address: 0x000888c0, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eStandardHeightmapMtl__PlatformFree_void(void *); /* extern */

void eStandardHeightmapMtl___dtor_eStandardHeightmapMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385CC0;
        eStandardHeightmapMtl__PlatformFree_void(arg0);
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
