# Battle Packet: `eBspTree::~eBspTree(void)`

## Target

- Address: `0x000219bc`
- Size: `452` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBspTree`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `167`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `constant_cse_codegen`, `needs_layout`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00021990` `matched` 8B `eBspTree::PlatformFree(void)`

## Placement

- Canonical source: `src/eBspTree.cpp`
- Header(s): `include/eBspTree.h`
- Allowed source(s): `src/eBspTree.cpp`, `src/eBspTree_dtor.cpp`
- Split-TU prefix: `src/eBspTree_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eBspTree.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002160c` 348B `eBspTree::Write(cFile &) const` — `src/eBspTree.cpp`
- `0x000218e8` 168B `eBspTree::PlatformRead(cFile &, cMemPool *)` — `src/eBspTree.cpp`
- `0x00021990` 8B `eBspTree::PlatformFree(void)` — `src/cFileSystemPlatform.cpp`
- `0x00021998` 36B `eBspTree::eBspTree(cBase *)` — `src/mVec3.cpp`
- `0x00021ce4` 112B `eBspTree::Contains(const eShape *, const mOCS &) const` — `src/eBspTree.cpp`

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

- session `67440fe3`; src `src/eBspTree_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260510_175602/67440fe3/000219bc__agent_self_reported_failure__src_eBspTree_dtor.cpp`

category=REG_ALLOC. Source code is structurally complete and the diff is 13/452 bytes — a pure callee-saved register naming swap (s2/s3) for the cached 'arr8 = this+8' pointer vs the 'entries = *(this+0xC)' temporary in the first cleanup loop. Pattern matches docs/research/snc-register-allocation.md exactly: same instruction selection, same scheduling, same delay-slot caching, only the s2/s3 fields differ across ~12 instructions plus 4 jal relocation immediates. Tried reordering the arrC/arr8 declarations both ways — no effect, mine assigns s2 to arr8 and s3 to entries while expected does the opposite. Permuter ran 180s, 2241 candidates compiled, zero improvements (started 13, ended 13). Split-TU pattern used (src/eBspTree_dtor.cpp) with local redeclaration of eBspTree (PlatformFree + ~eBspTree + operator delete) following gcValNavMeshDirection_dtor.cpp template. Trailer __asm__('.word 0x1000ffff ... .size __0oIeBspTreedtv, 0x1C4') reproduces the trailing infinite-loop padding. The inner null check before the final base-vtable write was reproduced via explicit `if (this != 0) *(void**)((char*)this+4) = 0x37E6A8;` (matches cObject_dtor.cpp pattern). operator delete includes `if (p != 0)` guard to reproduce the second `beqz this` before the deletion call. Stack frame, register save set, all immediates, all loads/stores, all branches match — only the s2/s3 naming differs. No source leverage available.


## Disassembly

```asm
219bc:	27bdffe0 	addiu	sp,sp,-32
219c0:	afb00000 	sw	s0,0(sp)
219c4:	00808025 	move	s0,a0
219c8:	afb10004 	sw	s1,4(sp)
219cc:	afb20008 	sw	s2,8(sp)
219d0:	afb3000c 	sw	s3,12(sp)
219d4:	afbf0010 	sw	ra,16(sp)
219d8:	10800060 	beqz	a0,0x21b5c
219dc:	00a08825 	move	s1,a1
219e0:	3c040038 	lui	a0,0x38
219e4:	2484fb80 	addiu	a0,a0,-1152
219e8:	ae040004 	sw	a0,4(s0)
219ec:	0c008664 	jal	0x21990
219f0:	02002025 	move	a0,s0
219f4:	2604000c 	addiu	a0,s0,12
219f8:	10800024 	beqz	a0,0x21a8c
219fc:	26130008 	addiu	s3,s0,8
21a00:	8e12000c 	lw	s2,12(s0)
21a04:	12400005 	beqz	s2,0x21a1c
21a08:	34040000 	li	a0,0x0
21a0c:	8e44fffc 	lw	a0,-4(s2)
21a10:	3c054000 	lui	a1,0x4000
21a14:	24a5ffff 	addiu	a1,a1,-1
21a18:	00852024 	and	a0,a0,a1
21a1c:	34050000 	li	a1,0x0
21a20:	00a4302a 	slt	a2,a1,a0
21a24:	10c00005 	beqz	a2,0x21a3c
21a28:	00000000 	nop
21a2c:	24a50001 	addiu	a1,a1,1
21a30:	00a4302a 	slt	a2,a1,a0
21a34:	14c0fffe 	bnez	a2,0x21a30
21a38:	24a50001 	addiu	a1,a1,1
21a3c:	12400013 	beqz	s2,0x21a8c
21a40:	00000000 	nop
21a44:	8e44fffc 	lw	a0,-4(s2)
21a48:	00042782 	srl	a0,a0,0x1e
21a4c:	00042080 	sll	a0,a0,0x2
21a50:	02449023 	subu	s2,s2,a0
21a54:	2652fffc 	addiu	s2,s2,-4
21a58:	5240000c 	beqzl	s2,0x21a8c
21a5c:	ae00000c 	sw	zero,12(s0)
21a60:	0c0011e8 	jal	0x47a0
21a64:	02402025 	move	a0,s2
21a68:	8c440024 	lw	a0,36(v0)
21a6c:	02402825 	move	a1,s2
21a70:	8c86001c 	lw	a2,28(a0)
21a74:	24c60030 	addiu	a2,a2,48
21a78:	84c70000 	lh	a3,0(a2)
21a7c:	8cc60004 	lw	a2,4(a2)
21a80:	00c0f809 	jalr	a2
21a84:	00872021 	addu	a0,a0,a3
21a88:	ae00000c 	sw	zero,12(s0)
21a8c:	1260001f 	beqz	s3,0x21b0c
21a90:	00000000 	nop
21a94:	8e120008 	lw	s2,8(s0)
21a98:	12400005 	beqz	s2,0x21ab0
21a9c:	34040000 	li	a0,0x0
21aa0:	8e44fffc 	lw	a0,-4(s2)
21aa4:	3c054000 	lui	a1,0x4000
21aa8:	24a5ffff 	addiu	a1,a1,-1
21aac:	00852024 	and	a0,a0,a1
21ab0:	34050000 	li	a1,0x0
21ab4:	00a4302a 	slt	a2,a1,a0
21ab8:	10c00005 	beqz	a2,0x21ad0
21abc:	00000000 	nop
21ac0:	24a50001 	addiu	a1,a1,1
21ac4:	00a4302a 	slt	a2,a1,a0
21ac8:	14c0fffe 	bnez	a2,0x21ac4
21acc:	24a50001 	addiu	a1,a1,1
21ad0:	1240000e 	beqz	s2,0x21b0c
21ad4:	2652fffc 	addiu	s2,s2,-4
21ad8:	5240000c 	beqzl	s2,0x21b0c
21adc:	ae000008 	sw	zero,8(s0)
21ae0:	0c0011e8 	jal	0x47a0
21ae4:	02402025 	move	a0,s2
21ae8:	8c440024 	lw	a0,36(v0)
21aec:	02402825 	move	a1,s2
21af0:	8c86001c 	lw	a2,28(a0)
21af4:	24c60030 	addiu	a2,a2,48
21af8:	84c70000 	lh	a3,0(a2)
21afc:	8cc60004 	lw	a2,4(a2)
21b00:	00c0f809 	jalr	a2
21b04:	00872021 	addu	a0,a0,a3
21b08:	ae000008 	sw	zero,8(s0)
21b0c:	12000005 	beqz	s0,0x21b24
21b10:	32240001 	andi	a0,s1,0x1
21b14:	3c040038 	lui	a0,0x38
21b18:	2484e6a8 	addiu	a0,a0,-6488
21b1c:	ae040004 	sw	a0,4(s0)
21b20:	32240001 	andi	a0,s1,0x1
21b24:	1080000d 	beqz	a0,0x21b5c
21b28:	00000000 	nop
21b2c:	1200000b 	beqz	s0,0x21b5c
21b30:	00000000 	nop
21b34:	0c0011e8 	jal	0x47a0
21b38:	02002025 	move	a0,s0
21b3c:	8c440024 	lw	a0,36(v0)
21b40:	02002825 	move	a1,s0
21b44:	8c86001c 	lw	a2,28(a0)
21b48:	24c60030 	addiu	a2,a2,48
21b4c:	84c70000 	lh	a3,0(a2)
21b50:	8cc60004 	lw	a2,4(a2)
21b54:	00c0f809 	jalr	a2
21b58:	00872021 	addu	a0,a0,a3
21b5c:	8fb00000 	lw	s0,0(sp)
21b60:	8fb10004 	lw	s1,4(sp)
21b64:	8fb20008 	lw	s2,8(sp)
21b68:	8fb3000c 	lw	s3,12(sp)
21b6c:	8fbf0010 	lw	ra,16(sp)
21b70:	03e00008 	jr	ra
21b74:	27bd0020 	addiu	sp,sp,32
21b78:	1000ffff 	b	0x21b78
21b7c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eBspTree::~eBspTree(void)
// Address: 0x000219bc, Size: 452B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? eBspTree__PlatformFree_void(void *);              /* extern */

void eBspTree___dtor_eBspTree_void(void *arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    ? (*temp_a2_4)(void *, void *, ?, s16);
    ? (*temp_a2_6)(void *, void *, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_a3_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
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
        arg0->unk4 = 0x37FB80;
        eBspTree__PlatformFree_void(arg0);
        if ((arg0 + 0xC) != NULL) {
            temp_s2 = arg0->unkC;
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
            if (temp_s2 != NULL) {
                temp_s2_2 = (temp_s2 - (((u32) temp_s2->unk-4 >> 0x1E) * 4)) - 4;
                if (temp_s2_2 != NULL) {
                    temp_a0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_2, var_a1, var_a2)->unk24;
                    temp_a2 = temp_a0->unk1C;
                    temp_a3 = temp_a2->unk30;
                    temp_a2_2 = (temp_a2 + 0x30)->unk4;
                    temp_a2_2(temp_a0 + temp_a3, temp_s2_2, temp_a2_2, temp_a3);
                }
                arg0->unkC = NULL;
            }
        }
        if ((arg0 + 8) != NULL) {
            temp_s2_3 = arg0->unk8;
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
            temp_s2_4 = temp_s2_3 - 4;
            if (temp_s2_3 != NULL) {
                if (temp_s2_4 != NULL) {
                    temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_4, var_a1_2, var_a2_2)->unk24;
                    temp_a2_3 = temp_a0_2->unk1C;
                    temp_a3_2 = temp_a2_3->unk30;
                    temp_a2_4 = (temp_a2_3 + 0x30)->unk4;
                    temp_a2_4(temp_a0_2 + temp_a3_2, temp_s2_4, temp_a2_4, temp_a3_2);
                }
                arg0->unk8 = NULL;
            }
        }
        var_a0_3 = arg1 & 1;
        if (arg0 != NULL) {
            arg0->unk4 = 0x37E6A8;
            var_a0_3 = arg1 & 1;
        }
        if ((var_a0_3 != 0) && (arg0 != NULL)) {
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
