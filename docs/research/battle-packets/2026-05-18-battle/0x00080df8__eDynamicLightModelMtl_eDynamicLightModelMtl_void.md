# Battle Packet: `eDynamicLightModelMtl::~eDynamicLightModelMtl(void)`

## Target

- Address: `0x00080df8`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicLightModelMtl`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `167`
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
- `0x00080d90` `matched` 8B `eDynamicLightModelMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eDynamicLightModelMtl.cpp`
- Header(s): `include/eDynamicLightModelMtl.h`
- Allowed source(s): `src/eDynamicLightModelMtl.cpp`
- Split-TU prefix: `src/eDynamicLightModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eDynamicLightModelMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x00080bac` 76B `eDynamicLightModelMtl::Write(cFile &) const` — `src/eDynamicLightModelMtl.cpp`
- `0x00080bf8` 212B `eDynamicLightModelMtl::Read(cFile &, cMemPool *)` — `src/eDynamicLightModelMtl.cpp`
- `0x00080ccc` 196B `eDynamicLightModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eStandardWeatherEffectMtl.cpp`
- `0x00080d90` 8B `eDynamicLightModelMtl::PlatformFree(void)` — `src/eDynamicLightModelMtl_PlatformFree.cpp`
- `0x00080d98` 96B `eDynamicLightModelMtl::eDynamicLightModelMtl(cBase *)` — `src/eDynamicLightModelMtl.cpp`

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

- session `1c97b88c`; src `src/eDynamicLightModelMtl.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/1c97b88c/00080df8__agent_self_reported_failure__src_eDynamicLightModelMtl.cpp`

Closest source is a canonical C++ destructor with inline class-local operator delete and the matched two-word destructor tail. After relocation masking the function is 10/440 bytes off: the only remaining body differences are register allocation in the first cArray cleanup block. Original uses s3 for the saved field60 address and s2 for entries; SNC keeps emitting s2 for the saved field address and s3 for the first entries pointer, while the second cleanup block and deleting destructor tail match structurally.

Tried the matched eHeightmapShape/eBipedController destructor pattern, direct field addressing, field60 pointer reuse vs direct loads/stores, register-pinned locals using s3/$19 and s2/$18, and reordering entries/field60 declarations. Read docs/research/snc-delay-slots.md before the later variants. Ran tools/permuter.py for 300s from the 10-byte baseline: 7,584 candidates generated, 3,543 compiled, no improvement. This is retryable register-allocation/source-shape work, not bnel/unmatchable.


## Disassembly

```asm
80df8:	27bdffe0 	addiu	sp,sp,-32
80dfc:	afb00000 	sw	s0,0(sp)
80e00:	afb10004 	sw	s1,4(sp)
80e04:	afb20008 	sw	s2,8(sp)
80e08:	afb3000c 	sw	s3,12(sp)
80e0c:	afbf0010 	sw	ra,16(sp)
80e10:	00808025 	move	s0,a0
80e14:	1080005d 	beqz	a0,0x80f8c
80e18:	00a08825 	move	s1,a1
80e1c:	3c040038 	lui	a0,0x38
80e20:	24845218 	addiu	a0,a0,21016
80e24:	ae040004 	sw	a0,4(s0)
80e28:	0c020364 	jal	0x80d90
80e2c:	02002025 	move	a0,s0
80e30:	3c040038 	lui	a0,0x38
80e34:	24840018 	addiu	a0,a0,24
80e38:	ae040004 	sw	a0,4(s0)
80e3c:	26040064 	addiu	a0,s0,100
80e40:	1080001f 	beqz	a0,0x80ec0
80e44:	26130060 	addiu	s3,s0,96
80e48:	8e120064 	lw	s2,100(s0)
80e4c:	12400005 	beqz	s2,0x80e64
80e50:	34040000 	li	a0,0x0
80e54:	8e44fffc 	lw	a0,-4(s2)
80e58:	3c054000 	lui	a1,0x4000
80e5c:	24a5ffff 	addiu	a1,a1,-1
80e60:	00852024 	and	a0,a0,a1
80e64:	34050000 	li	a1,0x0
80e68:	00a4302a 	slt	a2,a1,a0
80e6c:	10c00005 	beqz	a2,0x80e84
80e70:	00000000 	nop
80e74:	24a50001 	addiu	a1,a1,1
80e78:	00a4302a 	slt	a2,a1,a0
80e7c:	14c0fffe 	bnez	a2,0x80e78
80e80:	24a50001 	addiu	a1,a1,1
80e84:	1240000e 	beqz	s2,0x80ec0
80e88:	2652fffc 	addiu	s2,s2,-4
80e8c:	5240000c 	beqzl	s2,0x80ec0
80e90:	ae000064 	sw	zero,100(s0)
80e94:	0c0011e8 	jal	0x47a0
80e98:	02402025 	move	a0,s2
80e9c:	8c440024 	lw	a0,36(v0)
80ea0:	8c85001c 	lw	a1,28(a0)
80ea4:	24a50030 	addiu	a1,a1,48
80ea8:	84a60000 	lh	a2,0(a1)
80eac:	00862021 	addu	a0,a0,a2
80eb0:	8ca60004 	lw	a2,4(a1)
80eb4:	00c0f809 	jalr	a2
80eb8:	02402825 	move	a1,s2
80ebc:	ae000064 	sw	zero,100(s0)
80ec0:	12600021 	beqz	s3,0x80f48
80ec4:	02002025 	move	a0,s0
80ec8:	8e120060 	lw	s2,96(s0)
80ecc:	12400005 	beqz	s2,0x80ee4
80ed0:	34040000 	li	a0,0x0
80ed4:	8e44fffc 	lw	a0,-4(s2)
80ed8:	3c054000 	lui	a1,0x4000
80edc:	24a5ffff 	addiu	a1,a1,-1
80ee0:	00852024 	and	a0,a0,a1
80ee4:	34050000 	li	a1,0x0
80ee8:	00a4302a 	slt	a2,a1,a0
80eec:	10c00005 	beqz	a2,0x80f04
80ef0:	00000000 	nop
80ef4:	24a50001 	addiu	a1,a1,1
80ef8:	00a4302a 	slt	a2,a1,a0
80efc:	14c0fffe 	bnez	a2,0x80ef8
80f00:	24a50001 	addiu	a1,a1,1
80f04:	12400010 	beqz	s2,0x80f48
80f08:	02002025 	move	a0,s0
80f0c:	2652fffc 	addiu	s2,s2,-4
80f10:	5240000c 	beqzl	s2,0x80f44
80f14:	ae000060 	sw	zero,96(s0)
80f18:	0c0011e8 	jal	0x47a0
80f1c:	02402025 	move	a0,s2
80f20:	8c440024 	lw	a0,36(v0)
80f24:	8c85001c 	lw	a1,28(a0)
80f28:	24a50030 	addiu	a1,a1,48
80f2c:	84a60000 	lh	a2,0(a1)
80f30:	00862021 	addu	a0,a0,a2
80f34:	8ca60004 	lw	a2,4(a1)
80f38:	00c0f809 	jalr	a2
80f3c:	02402825 	move	a1,s2
80f40:	ae000060 	sw	zero,96(s0)
80f44:	02002025 	move	a0,s0
80f48:	0c00b034 	jal	0x2c0d0
80f4c:	00002825 	move	a1,zero
80f50:	32240001 	andi	a0,s1,0x1
80f54:	1080000d 	beqz	a0,0x80f8c
80f58:	00000000 	nop
80f5c:	1200000b 	beqz	s0,0x80f8c
80f60:	00000000 	nop
80f64:	0c0011e8 	jal	0x47a0
80f68:	02002025 	move	a0,s0
80f6c:	8c440024 	lw	a0,36(v0)
80f70:	8c85001c 	lw	a1,28(a0)
80f74:	24a50030 	addiu	a1,a1,48
80f78:	84a60000 	lh	a2,0(a1)
80f7c:	00862021 	addu	a0,a0,a2
80f80:	8ca60004 	lw	a2,4(a1)
80f84:	00c0f809 	jalr	a2
80f88:	02002825 	move	a1,s0
80f8c:	8fb00000 	lw	s0,0(sp)
80f90:	8fb10004 	lw	s1,4(sp)
80f94:	8fb20008 	lw	s2,8(sp)
80f98:	8fb3000c 	lw	s3,12(sp)
80f9c:	8fbf0010 	lw	ra,16(sp)
80fa0:	03e00008 	jr	ra
80fa4:	27bd0020 	addiu	sp,sp,32
80fa8:	1000ffff 	b	0x80fa8
80fac:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eDynamicLightModelMtl::~eDynamicLightModelMtl(void)
// Address: 0x00080df8, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eDynamicLightModelMtl__PlatformFree_void(void *); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */

void eDynamicLightModelMtl___dtor_eDynamicLightModelMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385218;
        eDynamicLightModelMtl__PlatformFree_void(arg0);
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
