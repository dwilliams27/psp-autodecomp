# Battle Packet: `eGeomMtl::~eGeomMtl(void)`

## Target

- Address: `0x001e2028`
- Size: `420` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eGeomMtl`
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
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`

## Placement

- Canonical source: `src/eGeomMtl.cpp`
- Header(s): `include/eGeomMtl.h`
- Allowed source(s): `src/eGeomMtl.cpp`, `src/eGeomMtl_dtor.cpp`
- Split-TU prefix: `src/eGeomMtl_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eGeomMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002cbd8` 372B `eGeomMtl::Write(cFile &) const` — `src/eGeomMtl.cpp`
- `0x0002ce90` 84B `eGeomMtl::eGeomMtl(cBase *)` — `src/eGeomMtl.cpp`
- `0x001e1ec8` 352B `eGeomMtl::GetType(void) const` — `src/eDynamicLightModelMtl.cpp`

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

### Exemplar 2: `eSimulatedController::~eSimulatedController(void)`

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

### Exemplar 3: `eOnePassModelMtl::~eOnePassModelMtl(void)`

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

### Exemplar 4: `eStandardGeomTrailMtl::~eStandardGeomTrailMtl(void)`

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


## Prior Failure Notes

### Note 1

- session `709fc535`; src `src/eGeomMtl_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/709fc535/001e2028__agent_self_reported_failure__src_eGeomMtl_dtor.cpp`

category=REG_ALLOC. Pure s2/s3 register-allocation drift. 10/420 bytes differ. Target allocates s3=arr60Addr (preserved across arr64 cleanup) and s2=arr64 entries; our build allocates s2=arr60Addr and s3=arr64 entries. Differing instructions are exclusively the rs/rt register fields in: 38 (addiu sN,s0,0x60), 3c (lw sM,0x64(s0)), 40 (beqz sM), 48 (lw a0,-4(sM)), 78 (beqz sM), 7c (addiu sM,sM,-4), 80 (beqzl sM), 8c (move a0,sM), 94 (move a1,sM). No control-flow, immediate, stack-frame, addressing-mode, or call-target differences anywhere in the function. Source closely mirrors the matched eDecalSystem_dtor.cpp template (sched=2, both array addresses declared upfront so arr60Addr is preserved across the arr64 cleanup, inline class-local operator delete, asm tail '.word 0x1000ffff/.word 0/.size __0oIeGeomMtldtv,0x1a4' for the SNC deleting-destructor sentinel, eMaterial_dtor extern with asm("__0oJeMaterialdtv")). Permuter ran 360s/4068 compiled candidates with --save-best and produced 0 improvements. This is the documented SNC context-dependent register-allocation tie-break described in docs/research/snc-register-allocation.md and docs/decisions/012-snc-context-dependent-regalloc.md — small TU-context differences flip the s2/s3 choice and there is no local source leverage. Per ADR-012 stop chasing once the diff is only callee-save reg naming.


## Disassembly

```asm
1e2028:	27bdffe0 	addiu	sp,sp,-32
1e202c:	afb00000 	sw	s0,0(sp)
1e2030:	00808025 	move	s0,a0
1e2034:	afb10004 	sw	s1,4(sp)
1e2038:	afb20008 	sw	s2,8(sp)
1e203c:	afb3000c 	sw	s3,12(sp)
1e2040:	afbf0010 	sw	ra,16(sp)
1e2044:	10800058 	beqz	a0,0x1e21a8
1e2048:	00a08825 	move	s1,a1
1e204c:	3c040038 	lui	a0,0x38
1e2050:	24840018 	addiu	a0,a0,24
1e2054:	ae040004 	sw	a0,4(s0)
1e2058:	26040064 	addiu	a0,s0,100
1e205c:	1080001f 	beqz	a0,0x1e20dc
1e2060:	26130060 	addiu	s3,s0,96
1e2064:	8e120064 	lw	s2,100(s0)
1e2068:	12400005 	beqz	s2,0x1e2080
1e206c:	34040000 	li	a0,0x0
1e2070:	8e44fffc 	lw	a0,-4(s2)
1e2074:	3c054000 	lui	a1,0x4000
1e2078:	24a5ffff 	addiu	a1,a1,-1
1e207c:	00852024 	and	a0,a0,a1
1e2080:	34050000 	li	a1,0x0
1e2084:	00a4302a 	slt	a2,a1,a0
1e2088:	10c00005 	beqz	a2,0x1e20a0
1e208c:	00000000 	nop
1e2090:	24a50001 	addiu	a1,a1,1
1e2094:	00a4302a 	slt	a2,a1,a0
1e2098:	14c0fffe 	bnez	a2,0x1e2094
1e209c:	24a50001 	addiu	a1,a1,1
1e20a0:	1240000e 	beqz	s2,0x1e20dc
1e20a4:	2652fffc 	addiu	s2,s2,-4
1e20a8:	5240000c 	beqzl	s2,0x1e20dc
1e20ac:	ae000064 	sw	zero,100(s0)
1e20b0:	0c0011e8 	jal	0x47a0
1e20b4:	02402025 	move	a0,s2
1e20b8:	8c440024 	lw	a0,36(v0)
1e20bc:	02402825 	move	a1,s2
1e20c0:	8c86001c 	lw	a2,28(a0)
1e20c4:	24c60030 	addiu	a2,a2,48
1e20c8:	84c70000 	lh	a3,0(a2)
1e20cc:	8cc60004 	lw	a2,4(a2)
1e20d0:	00c0f809 	jalr	a2
1e20d4:	00872021 	addu	a0,a0,a3
1e20d8:	ae000064 	sw	zero,100(s0)
1e20dc:	12600021 	beqz	s3,0x1e2164
1e20e0:	02002025 	move	a0,s0
1e20e4:	8e120060 	lw	s2,96(s0)
1e20e8:	12400005 	beqz	s2,0x1e2100
1e20ec:	34040000 	li	a0,0x0
1e20f0:	8e44fffc 	lw	a0,-4(s2)
1e20f4:	3c054000 	lui	a1,0x4000
1e20f8:	24a5ffff 	addiu	a1,a1,-1
1e20fc:	00852024 	and	a0,a0,a1
1e2100:	34050000 	li	a1,0x0
1e2104:	00a4302a 	slt	a2,a1,a0
1e2108:	10c00005 	beqz	a2,0x1e2120
1e210c:	00000000 	nop
1e2110:	24a50001 	addiu	a1,a1,1
1e2114:	00a4302a 	slt	a2,a1,a0
1e2118:	14c0fffe 	bnez	a2,0x1e2114
1e211c:	24a50001 	addiu	a1,a1,1
1e2120:	12400010 	beqz	s2,0x1e2164
1e2124:	02002025 	move	a0,s0
1e2128:	2652fffc 	addiu	s2,s2,-4
1e212c:	5240000c 	beqzl	s2,0x1e2160
1e2130:	ae000060 	sw	zero,96(s0)
1e2134:	0c0011e8 	jal	0x47a0
1e2138:	02402025 	move	a0,s2
1e213c:	8c440024 	lw	a0,36(v0)
1e2140:	02402825 	move	a1,s2
1e2144:	8c86001c 	lw	a2,28(a0)
1e2148:	24c60030 	addiu	a2,a2,48
1e214c:	84c70000 	lh	a3,0(a2)
1e2150:	8cc60004 	lw	a2,4(a2)
1e2154:	00c0f809 	jalr	a2
1e2158:	00872021 	addu	a0,a0,a3
1e215c:	ae000060 	sw	zero,96(s0)
1e2160:	02002025 	move	a0,s0
1e2164:	0c00b034 	jal	0x2c0d0
1e2168:	00002825 	move	a1,zero
1e216c:	32240001 	andi	a0,s1,0x1
1e2170:	1080000d 	beqz	a0,0x1e21a8
1e2174:	00000000 	nop
1e2178:	1200000b 	beqz	s0,0x1e21a8
1e217c:	00000000 	nop
1e2180:	0c0011e8 	jal	0x47a0
1e2184:	02002025 	move	a0,s0
1e2188:	8c440024 	lw	a0,36(v0)
1e218c:	02002825 	move	a1,s0
1e2190:	8c86001c 	lw	a2,28(a0)
1e2194:	24c60030 	addiu	a2,a2,48
1e2198:	84c70000 	lh	a3,0(a2)
1e219c:	8cc60004 	lw	a2,4(a2)
1e21a0:	00c0f809 	jalr	a2
1e21a4:	00872021 	addu	a0,a0,a3
1e21a8:	8fb00000 	lw	s0,0(sp)
1e21ac:	8fb10004 	lw	s1,4(sp)
1e21b0:	8fb20008 	lw	s2,8(sp)
1e21b4:	8fb3000c 	lw	s3,12(sp)
1e21b8:	8fbf0010 	lw	ra,16(sp)
1e21bc:	03e00008 	jr	ra
1e21c0:	27bd0020 	addiu	sp,sp,32
1e21c4:	1000ffff 	b	0x1e21c4
1e21c8:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eGeomMtl::~eGeomMtl(void)
// Address: 0x001e2028, Size: 420B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */

void eGeomMtl___dtor_eGeomMtl_void(void *arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    ? (*temp_a2_4)(void *, void *, ?, s16);
    ? (*temp_a2_6)(void *, void *, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_a3_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a2_2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_3;
    void *temp_a2_5;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_s2_3;
    void *temp_s2_4;

    if (arg0 != NULL) {
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
                    temp_a2 = temp_a0->unk1C;
                    temp_a3 = temp_a2->unk30;
                    temp_a2_2 = (temp_a2 + 0x30)->unk4;
                    temp_a2_2(temp_a0 + temp_a3, temp_s2_2, temp_a2_2, temp_a3);
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
                    temp_a2_3 = temp_a0_2->unk1C;
                    temp_a3_2 = temp_a2_3->unk30;
                    temp_a2_4 = (temp_a2_3 + 0x30)->unk4;
                    temp_a2_4(temp_a0_2 + temp_a3_2, temp_s2_4, temp_a2_4, temp_a3_2);
                }
                arg0->unk60 = NULL;
            }
        }
        eMaterial___dtor_eMaterial_void(arg0, 0);
        if ((arg1 & 1) && (arg0 != NULL)) {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a2_5 = temp_a0_3->unk1C;
            temp_a3_3 = temp_a2_5->unk30;
            temp_a2_6 = (temp_a2_5 + 0x30)->unk4;
            temp_a2_6(temp_a0_3 + temp_a3_3, arg0, temp_a2_6, temp_a3_3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
