# Battle Packet: `eDynamicLightHeightmapMtl::~eDynamicLightHeightmapMtl(void)`

## Target

- Address: `0x000809d4`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicLightHeightmapMtl`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; correct function size; codegen-specific diagnosis; manageable size`
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
- `0x00080974` `matched` 8B `eDynamicLightHeightmapMtl::PlatformFree(void)`

## Placement

- Canonical source: `src/eDynamicLightHeightmapMtl.cpp`
- Header(s): `include/eDynamicLightHeightmapMtl.h`
- Allowed source(s): `src/eDynamicLightHeightmapMtl.cpp`
- Split-TU prefix: `src/eDynamicLightHeightmapMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eDynamicLightHeightmapMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x00080790` 76B `eDynamicLightHeightmapMtl::Write(cFile &) const` — `src/eDynamicLightHeightmapMtl.cpp`
- `0x000807dc` 212B `eDynamicLightHeightmapMtl::Read(cFile &, cMemPool *)` — `src/eDynamicLightHeightmapMtl_Read.cpp`
- `0x000808b0` 196B `eDynamicLightHeightmapMtl::PlatformRead(cFile &, cMemPool *)` — `src/eStandardWeatherEffectMtl.cpp`
- `0x00080974` 8B `eDynamicLightHeightmapMtl::PlatformFree(void)` — `src/eDynamicLightHeightmapMtl_PlatformFree.cpp`
- `0x0008097c` 88B `eDynamicLightHeightmapMtl::eDynamicLightHeightmapMtl(cBase *)` — `src/eDynamicLightHeightmapMtl.cpp`

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

- session `dd71eb79`; src `src/eDynamicLightHeightmapMtl.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/dd71eb79/000809d4__agent_self_reported_failure__src_eDynamicLightHeightmapMtl.cpp`

Closest source is a canonical C++ destructor in src/eDynamicLightHeightmapMtl.cpp with inline array cleanup and deleting-destructor tail. It emits the correct 440-byte symbol including the trailing infinite-loop padding, calls PlatformFree(), clears both arrays at 0x64 and 0x60, calls eMaterial::~eMaterial(this, 0), and emits the operator-delete tail. Final compare is 10/440 bytes different. The only remaining diff is the first array cleanup register allocation: compiled code keeps the 0x60 field address in s2 and the 0x64 array data in s3, while the target has the 0x60 field address in s3 and the 0x64 array data in s2. The second array cleanup and all other bytes match after relocation masking.

Tried hand-expanded cleanup, moving the destructor block to end-of-file to get the .size/padding correct, volatile/register locals for s2/s3, wrapper destructor form, delaying the 0x60 field computation, keeping the data pointer live with an empty asm constraint, and a local sched=2 override. The wrapper form added an extra saved register; sched=2 worsened to 24 bytes. tools/permuter.py ran for 300 seconds, generated 7392 candidates, compiled 3570, and found zero improvement from the 10-byte baseline. This is retryable, not an unmatchable_bnel case.


## Disassembly

```asm
809d4:	27bdffe0 	addiu	sp,sp,-32
809d8:	afb00000 	sw	s0,0(sp)
809dc:	afb10004 	sw	s1,4(sp)
809e0:	afb20008 	sw	s2,8(sp)
809e4:	afb3000c 	sw	s3,12(sp)
809e8:	afbf0010 	sw	ra,16(sp)
809ec:	00808025 	move	s0,a0
809f0:	1080005d 	beqz	a0,0x80b68
809f4:	00a08825 	move	s1,a1
809f8:	3c040038 	lui	a0,0x38
809fc:	24845160 	addiu	a0,a0,20832
80a00:	ae040004 	sw	a0,4(s0)
80a04:	0c02025d 	jal	0x80974
80a08:	02002025 	move	a0,s0
80a0c:	3c040038 	lui	a0,0x38
80a10:	24840018 	addiu	a0,a0,24
80a14:	ae040004 	sw	a0,4(s0)
80a18:	26040064 	addiu	a0,s0,100
80a1c:	1080001f 	beqz	a0,0x80a9c
80a20:	26130060 	addiu	s3,s0,96
80a24:	8e120064 	lw	s2,100(s0)
80a28:	12400005 	beqz	s2,0x80a40
80a2c:	34040000 	li	a0,0x0
80a30:	8e44fffc 	lw	a0,-4(s2)
80a34:	3c054000 	lui	a1,0x4000
80a38:	24a5ffff 	addiu	a1,a1,-1
80a3c:	00852024 	and	a0,a0,a1
80a40:	34050000 	li	a1,0x0
80a44:	00a4302a 	slt	a2,a1,a0
80a48:	10c00005 	beqz	a2,0x80a60
80a4c:	00000000 	nop
80a50:	24a50001 	addiu	a1,a1,1
80a54:	00a4302a 	slt	a2,a1,a0
80a58:	14c0fffe 	bnez	a2,0x80a54
80a5c:	24a50001 	addiu	a1,a1,1
80a60:	1240000e 	beqz	s2,0x80a9c
80a64:	2652fffc 	addiu	s2,s2,-4
80a68:	5240000c 	beqzl	s2,0x80a9c
80a6c:	ae000064 	sw	zero,100(s0)
80a70:	0c0011e8 	jal	0x47a0
80a74:	02402025 	move	a0,s2
80a78:	8c440024 	lw	a0,36(v0)
80a7c:	8c85001c 	lw	a1,28(a0)
80a80:	24a50030 	addiu	a1,a1,48
80a84:	84a60000 	lh	a2,0(a1)
80a88:	00862021 	addu	a0,a0,a2
80a8c:	8ca60004 	lw	a2,4(a1)
80a90:	00c0f809 	jalr	a2
80a94:	02402825 	move	a1,s2
80a98:	ae000064 	sw	zero,100(s0)
80a9c:	12600021 	beqz	s3,0x80b24
80aa0:	02002025 	move	a0,s0
80aa4:	8e120060 	lw	s2,96(s0)
80aa8:	12400005 	beqz	s2,0x80ac0
80aac:	34040000 	li	a0,0x0
80ab0:	8e44fffc 	lw	a0,-4(s2)
80ab4:	3c054000 	lui	a1,0x4000
80ab8:	24a5ffff 	addiu	a1,a1,-1
80abc:	00852024 	and	a0,a0,a1
80ac0:	34050000 	li	a1,0x0
80ac4:	00a4302a 	slt	a2,a1,a0
80ac8:	10c00005 	beqz	a2,0x80ae0
80acc:	00000000 	nop
80ad0:	24a50001 	addiu	a1,a1,1
80ad4:	00a4302a 	slt	a2,a1,a0
80ad8:	14c0fffe 	bnez	a2,0x80ad4
80adc:	24a50001 	addiu	a1,a1,1
80ae0:	12400010 	beqz	s2,0x80b24
80ae4:	02002025 	move	a0,s0
80ae8:	2652fffc 	addiu	s2,s2,-4
80aec:	5240000c 	beqzl	s2,0x80b20
80af0:	ae000060 	sw	zero,96(s0)
80af4:	0c0011e8 	jal	0x47a0
80af8:	02402025 	move	a0,s2
80afc:	8c440024 	lw	a0,36(v0)
80b00:	8c85001c 	lw	a1,28(a0)
80b04:	24a50030 	addiu	a1,a1,48
80b08:	84a60000 	lh	a2,0(a1)
80b0c:	00862021 	addu	a0,a0,a2
80b10:	8ca60004 	lw	a2,4(a1)
80b14:	00c0f809 	jalr	a2
80b18:	02402825 	move	a1,s2
80b1c:	ae000060 	sw	zero,96(s0)
80b20:	02002025 	move	a0,s0
80b24:	0c00b034 	jal	0x2c0d0
80b28:	00002825 	move	a1,zero
80b2c:	32240001 	andi	a0,s1,0x1
80b30:	1080000d 	beqz	a0,0x80b68
80b34:	00000000 	nop
80b38:	1200000b 	beqz	s0,0x80b68
80b3c:	00000000 	nop
80b40:	0c0011e8 	jal	0x47a0
80b44:	02002025 	move	a0,s0
80b48:	8c440024 	lw	a0,36(v0)
80b4c:	8c85001c 	lw	a1,28(a0)
80b50:	24a50030 	addiu	a1,a1,48
80b54:	84a60000 	lh	a2,0(a1)
80b58:	00862021 	addu	a0,a0,a2
80b5c:	8ca60004 	lw	a2,4(a1)
80b60:	00c0f809 	jalr	a2
80b64:	02002825 	move	a1,s0
80b68:	8fb00000 	lw	s0,0(sp)
80b6c:	8fb10004 	lw	s1,4(sp)
80b70:	8fb20008 	lw	s2,8(sp)
80b74:	8fb3000c 	lw	s3,12(sp)
80b78:	8fbf0010 	lw	ra,16(sp)
80b7c:	03e00008 	jr	ra
80b80:	27bd0020 	addiu	sp,sp,32
80b84:	1000ffff 	b	0x80b84
80b88:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eDynamicLightHeightmapMtl::~eDynamicLightHeightmapMtl(void)
// Address: 0x000809d4, Size: 440B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eDynamicLightHeightmapMtl__PlatformFree_void(void *); /* extern */
? eMaterial___dtor_eMaterial_void(void *, ?);       /* extern */

void eDynamicLightHeightmapMtl___dtor_eDynamicLightHeightmapMtl_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x385160;
        eDynamicLightHeightmapMtl__PlatformFree_void(arg0);
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
