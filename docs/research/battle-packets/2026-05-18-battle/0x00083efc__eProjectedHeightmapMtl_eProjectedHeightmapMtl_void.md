# Battle Packet: `eProjectedHeightmapMtl::~eProjectedHeightmapMtl(void)`

## Target

- Address: `0x00083efc`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eProjectedHeightmapMtl`
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
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`
- `0x00083eb0` `matched` 8B `eProjectedHeightmapMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eProjectedHeightmapMtl.cpp`
- Header(s): `include/eProjectedHeightmapMtl.h`
- Allowed source(s): `src/eProjectedHeightmapMtl.cpp`
- Split-TU prefix: `src/eProjectedHeightmapMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eProjectedHeightmapMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x00083c64` 104B `eProjectedHeightmapMtl::Write(cFile &) const` — `src/eProjectedHeightmapMtl.cpp`
- `0x00083ccc` 288B `eProjectedHeightmapMtl::Read(cFile &, cMemPool *)` — `src/eProjectedHeightmapMtl.cpp`
- `0x00083dec` 196B `eProjectedHeightmapMtl::PlatformRead(cFile &, cMemPool *)` — `src/eProjectedHeightmapMtl.cpp`
- `0x00083eb0` 8B `eProjectedHeightmapMtl::PlatformFree(void)` — `src/eProjectedHeightmapMtl_PlatformFree.cpp`
- `0x00083eb8` 68B `eProjectedHeightmapMtl::eProjectedHeightmapMtl(cBase *)` — `src/eProjectedHeightmapMtl.cpp`

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

- session `2b59df5f`; src `src/eProjectedHeightmapMtl.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/2b59df5f/00083efc__agent_self_reported_failure__src_eProjectedHeightmapMtl.cpp`

Closest destructor is a real C++ destructor based on the matched eDynamicLightModelMtl/eHeightmapShape array cleanup pattern. It is 10/440 bytes off after relocation masking. Disassembly shows the remaining mismatch is a register-allocation swap in the first array cleanup: target keeps the 0x60 field pointer in s3 and the 0x64 entries pointer in s2, while SNC emits field60 in s2 and entries in s3, with the same swap reflected at the second-block beqz. The actual operations, calls, branch structure, deleting tail, and epilogue otherwise line up.

Tried baseline pattern, explicit register variables, source-lifetime reordering around field60/field64, and register-variable plus empty-asm constraints; the latter two made the function much worse and were reverted. tools/permuter.py ran for 300 seconds against 0x00083efc, compiled 4177 candidates, and found zero improvement over the 10-byte baseline. This should stay retryable failed, not unmatchable_bnel: the diff is not a branch-likely vs branch+nop issue.


## Disassembly

```asm
83efc:	27bdffe0 	addiu	sp,sp,-32
83f00:	afb00000 	sw	s0,0(sp)
83f04:	afb10004 	sw	s1,4(sp)
83f08:	afb20008 	sw	s2,8(sp)
83f0c:	afb3000c 	sw	s3,12(sp)
83f10:	afbf0010 	sw	ra,16(sp)
83f14:	00808025 	move	s0,a0
83f18:	1080005d 	beqz	a0,0x84090
83f1c:	00a08825 	move	s1,a1
83f20:	3c040038 	lui	a0,0x38
83f24:	24845658 	addiu	a0,a0,22104
83f28:	ae040004 	sw	a0,4(s0)
83f2c:	0c020fac 	jal	0x83eb0
83f30:	02002025 	move	a0,s0
83f34:	3c040038 	lui	a0,0x38
83f38:	24840018 	addiu	a0,a0,24
83f3c:	ae040004 	sw	a0,4(s0)
83f40:	26040064 	addiu	a0,s0,100
83f44:	1080001f 	beqz	a0,0x83fc4
83f48:	26130060 	addiu	s3,s0,96
83f4c:	8e120064 	lw	s2,100(s0)
83f50:	12400005 	beqz	s2,0x83f68
83f54:	34040000 	li	a0,0x0
83f58:	8e44fffc 	lw	a0,-4(s2)
83f5c:	3c054000 	lui	a1,0x4000
83f60:	24a5ffff 	addiu	a1,a1,-1
83f64:	00852024 	and	a0,a0,a1
83f68:	34050000 	li	a1,0x0
83f6c:	00a4302a 	slt	a2,a1,a0
83f70:	10c00005 	beqz	a2,0x83f88
83f74:	00000000 	nop
83f78:	24a50001 	addiu	a1,a1,1
83f7c:	00a4302a 	slt	a2,a1,a0
83f80:	14c0fffe 	bnez	a2,0x83f7c
83f84:	24a50001 	addiu	a1,a1,1
83f88:	1240000e 	beqz	s2,0x83fc4
83f8c:	2652fffc 	addiu	s2,s2,-4
83f90:	5240000c 	beqzl	s2,0x83fc4
83f94:	ae000064 	sw	zero,100(s0)
83f98:	0c0011e8 	jal	0x47a0
83f9c:	02402025 	move	a0,s2
83fa0:	8c440024 	lw	a0,36(v0)
83fa4:	8c85001c 	lw	a1,28(a0)
83fa8:	24a50030 	addiu	a1,a1,48
83fac:	84a60000 	lh	a2,0(a1)
83fb0:	00862021 	addu	a0,a0,a2
83fb4:	8ca60004 	lw	a2,4(a1)
83fb8:	00c0f809 	jalr	a2
83fbc:	02402825 	move	a1,s2
83fc0:	ae000064 	sw	zero,100(s0)
83fc4:	12600021 	beqz	s3,0x8404c
83fc8:	02002025 	move	a0,s0
83fcc:	8e120060 	lw	s2,96(s0)
83fd0:	12400005 	beqz	s2,0x83fe8
83fd4:	34040000 	li	a0,0x0
83fd8:	8e44fffc 	lw	a0,-4(s2)
83fdc:	3c054000 	lui	a1,0x4000
83fe0:	24a5ffff 	addiu	a1,a1,-1
83fe4:	00852024 	and	a0,a0,a1
83fe8:	34050000 	li	a1,0x0
83fec:	00a4302a 	slt	a2,a1,a0
83ff0:	10c00005 	beqz	a2,0x84008
83ff4:	00000000 	nop
83ff8:	24a50001 	addiu	a1,a1,1
83ffc:	00a4302a 	slt	a2,a1,a0
84000:	14c0fffe 	bnez	a2,0x83ffc
84004:	24a50001 	addiu	a1,a1,1
84008:	12400010 	beqz	s2,0x8404c
8400c:	02002025 	move	a0,s0
84010:	2652fffc 	addiu	s2,s2,-4
84014:	5240000c 	beqzl	s2,0x84048
84018:	ae000060 	sw	zero,96(s0)
8401c:	0c0011e8 	jal	0x47a0
84020:	02402025 	move	a0,s2
84024:	8c440024 	lw	a0,36(v0)
84028:	8c85001c 	lw	a1,28(a0)
8402c:	24a50030 	addiu	a1,a1,48
84030:	84a60000 	lh	a2,0(a1)
84034:	00862021 	addu	a0,a0,a2
84038:	8ca60004 	lw	a2,4(a1)
8403c:	00c0f809 	jalr	a2
84040:	02402825 	move	a1,s2
84044:	ae000060 	sw	zero,96(s0)
84048:	02002025 	move	a0,s0
8404c:	0c00b034 	jal	0x2c0d0
84050:	00002825 	move	a1,zero
84054:	32240001 	andi	a0,s1,0x1
84058:	1080000d 	beqz	a0,0x84090
8405c:	00000000 	nop
84060:	1200000b 	beqz	s0,0x84090
84064:	00000000 	nop
84068:	0c0011e8 	jal	0x47a0
8406c:	02002025 	move	a0,s0
84070:	8c440024 	lw	a0,36(v0)
84074:	8c85001c 	lw	a1,28(a0)
84078:	24a50030 	addiu	a1,a1,48
8407c:	84a60000 	lh	a2,0(a1)
84080:	00862021 	addu	a0,a0,a2
84084:	8ca60004 	lw	a2,4(a1)
84088:	00c0f809 	jalr	a2
8408c:	02002825 	move	a1,s0
84090:	8fb00000 	lw	s0,0(sp)
84094:	8fb10004 	lw	s1,4(sp)
84098:	8fb20008 	lw	s2,8(sp)
8409c:	8fb3000c 	lw	s3,12(sp)
840a0:	8fbf0010 	lw	ra,16(sp)
840a4:	03e00008 	jr	ra
840a8:	27bd0020 	addiu	sp,sp,32
840ac:	1000ffff 	b	0x840ac
840b0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eProjectedHeightmapMtl::~eProjectedHeightmapMtl(void)
// Address: 0x00083efc, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */
? eProjectedHeightmapMtl__PlatformFree_void(void *); /* extern */

void eProjectedHeightmapMtl___dtor_eProjectedHeightmapMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385658;
        eProjectedHeightmapMtl__PlatformFree_void(arg0);
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
