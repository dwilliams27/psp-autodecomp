# Battle Packet: `eSkin::~eSkin(void)`

## Target

- Address: `0x001f1f20`
- Size: `420` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSkin`
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
- `0x00009dc4` `failed` 432B `cObject::~cObject(void)`

## Placement

- Canonical source: `src/eSkin.cpp`
- Header(s): `include/eSkin.h`
- Allowed source(s): `src/eSkin.cpp`, `src/eSkin_dtor.cpp`
- Split-TU prefix: `src/eSkin_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eSkin.h`

Not found.

## Matched Same-Class Neighbors

- `0x0004bcbc` 328B `eSkin::Write(cFile &) const` — `src/ePortal.cpp`
- `0x0004be04` 212B `eSkin::Read(cFile &, cMemPool *)` — `src/eSkin.cpp`
- `0x001f16cc` 88B `eSkin::AssignCopy(const cBase *)` — `src/eSkin.cpp`
- `0x001f1724` 144B `eSkin::New(cMemPool *, cBase *) static` — `src/eSkin.cpp`
- `0x001f17b4` 288B `eSkin::GetType(void) const` — `src/eSkin.cpp`

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

- session `709fc535`; src `src/eSkin_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/709fc535/001f1f20__agent_self_reported_failure__src_eSkin_dtor.cpp`

category=REG_ALLOC. Identical s2/s3 register-allocation drift to eGeomMtl::~eGeomMtl. 10/420 bytes differ. Target allocates s3=arr44Addr (preserved across arr48 cleanup) and s2=arr48 entries; our build allocates s2=arr44Addr and s3=arr48 entries. Differing instructions are exclusively the rs/rt register fields at offsets 38, 3c, 40, 48, 78, 7c, 80, 8c, ac, b4 — control flow, immediates, stack frame, vtable address (0x3815F8), addressing modes, and call targets all match. Source modeled on ePath_dtor.cpp (sched=1, sw_s0..sw_ra then 'move s0,a0' then 'beqz a0' with 'move s1,a1' in delay slot, inline class-local operator delete with __asm__ memory barrier between rec offset load and base computation, cObject_dtor extern with asm("__0oHcObjectdtv"), asm tail '.word 0x1000ffff/.word 0/.size __0oFeSkindtv,0x1a4'). Permuter ran 360s/4630 compiled candidates --save-best with 0 improvements. Same SNC context-dependent allocator tie-break described in docs/research/snc-register-allocation.md.


## Disassembly

```asm
1f1f20:	27bdffe0 	addiu	sp,sp,-32
1f1f24:	afb00000 	sw	s0,0(sp)
1f1f28:	afb10004 	sw	s1,4(sp)
1f1f2c:	afb20008 	sw	s2,8(sp)
1f1f30:	afb3000c 	sw	s3,12(sp)
1f1f34:	afbf0010 	sw	ra,16(sp)
1f1f38:	00808025 	move	s0,a0
1f1f3c:	10800058 	beqz	a0,0x1f20a0
1f1f40:	00a08825 	move	s1,a1
1f1f44:	3c040038 	lui	a0,0x38
1f1f48:	248415f8 	addiu	a0,a0,5624
1f1f4c:	ae040004 	sw	a0,4(s0)
1f1f50:	26040048 	addiu	a0,s0,72
1f1f54:	1080001f 	beqz	a0,0x1f1fd4
1f1f58:	26130044 	addiu	s3,s0,68
1f1f5c:	8e120048 	lw	s2,72(s0)
1f1f60:	12400005 	beqz	s2,0x1f1f78
1f1f64:	34040000 	li	a0,0x0
1f1f68:	8e44fffc 	lw	a0,-4(s2)
1f1f6c:	3c054000 	lui	a1,0x4000
1f1f70:	24a5ffff 	addiu	a1,a1,-1
1f1f74:	00852024 	and	a0,a0,a1
1f1f78:	34050000 	li	a1,0x0
1f1f7c:	00a4302a 	slt	a2,a1,a0
1f1f80:	10c00005 	beqz	a2,0x1f1f98
1f1f84:	00000000 	nop
1f1f88:	24a50001 	addiu	a1,a1,1
1f1f8c:	00a4302a 	slt	a2,a1,a0
1f1f90:	14c0fffe 	bnez	a2,0x1f1f8c
1f1f94:	24a50001 	addiu	a1,a1,1
1f1f98:	1240000e 	beqz	s2,0x1f1fd4
1f1f9c:	2652fffc 	addiu	s2,s2,-4
1f1fa0:	5240000c 	beqzl	s2,0x1f1fd4
1f1fa4:	ae000048 	sw	zero,72(s0)
1f1fa8:	0c0011e8 	jal	0x47a0
1f1fac:	02402025 	move	a0,s2
1f1fb0:	8c440024 	lw	a0,36(v0)
1f1fb4:	8c85001c 	lw	a1,28(a0)
1f1fb8:	24a50030 	addiu	a1,a1,48
1f1fbc:	84a60000 	lh	a2,0(a1)
1f1fc0:	00862021 	addu	a0,a0,a2
1f1fc4:	8ca60004 	lw	a2,4(a1)
1f1fc8:	00c0f809 	jalr	a2
1f1fcc:	02402825 	move	a1,s2
1f1fd0:	ae000048 	sw	zero,72(s0)
1f1fd4:	12600021 	beqz	s3,0x1f205c
1f1fd8:	02002025 	move	a0,s0
1f1fdc:	8e120044 	lw	s2,68(s0)
1f1fe0:	12400005 	beqz	s2,0x1f1ff8
1f1fe4:	34040000 	li	a0,0x0
1f1fe8:	8e44fffc 	lw	a0,-4(s2)
1f1fec:	3c054000 	lui	a1,0x4000
1f1ff0:	24a5ffff 	addiu	a1,a1,-1
1f1ff4:	00852024 	and	a0,a0,a1
1f1ff8:	34050000 	li	a1,0x0
1f1ffc:	00a4302a 	slt	a2,a1,a0
1f2000:	10c00005 	beqz	a2,0x1f2018
1f2004:	00000000 	nop
1f2008:	24a50001 	addiu	a1,a1,1
1f200c:	00a4302a 	slt	a2,a1,a0
1f2010:	14c0fffe 	bnez	a2,0x1f200c
1f2014:	24a50001 	addiu	a1,a1,1
1f2018:	12400010 	beqz	s2,0x1f205c
1f201c:	02002025 	move	a0,s0
1f2020:	2652fffc 	addiu	s2,s2,-4
1f2024:	5240000c 	beqzl	s2,0x1f2058
1f2028:	ae000044 	sw	zero,68(s0)
1f202c:	0c0011e8 	jal	0x47a0
1f2030:	02402025 	move	a0,s2
1f2034:	8c440024 	lw	a0,36(v0)
1f2038:	8c85001c 	lw	a1,28(a0)
1f203c:	24a50030 	addiu	a1,a1,48
1f2040:	84a60000 	lh	a2,0(a1)
1f2044:	00862021 	addu	a0,a0,a2
1f2048:	8ca60004 	lw	a2,4(a1)
1f204c:	00c0f809 	jalr	a2
1f2050:	02402825 	move	a1,s2
1f2054:	ae000044 	sw	zero,68(s0)
1f2058:	02002025 	move	a0,s0
1f205c:	0c002771 	jal	0x9dc4
1f2060:	00002825 	move	a1,zero
1f2064:	32240001 	andi	a0,s1,0x1
1f2068:	1080000d 	beqz	a0,0x1f20a0
1f206c:	00000000 	nop
1f2070:	1200000b 	beqz	s0,0x1f20a0
1f2074:	00000000 	nop
1f2078:	0c0011e8 	jal	0x47a0
1f207c:	02002025 	move	a0,s0
1f2080:	8c440024 	lw	a0,36(v0)
1f2084:	8c85001c 	lw	a1,28(a0)
1f2088:	24a50030 	addiu	a1,a1,48
1f208c:	84a60000 	lh	a2,0(a1)
1f2090:	00862021 	addu	a0,a0,a2
1f2094:	8ca60004 	lw	a2,4(a1)
1f2098:	00c0f809 	jalr	a2
1f209c:	02002825 	move	a1,s0
1f20a0:	8fb00000 	lw	s0,0(sp)
1f20a4:	8fb10004 	lw	s1,4(sp)
1f20a8:	8fb20008 	lw	s2,8(sp)
1f20ac:	8fb3000c 	lw	s3,12(sp)
1f20b0:	8fbf0010 	lw	ra,16(sp)
1f20b4:	03e00008 	jr	ra
1f20b8:	27bd0020 	addiu	sp,sp,32
1f20bc:	1000ffff 	b	0x1f20bc
1f20c0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eSkin::~eSkin(void)
// Address: 0x001f1f20, Size: 420B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? cObject___dtor_cObject_void(void *, ?);           /* extern */

void eSkin___dtor_eSkin_void(void *arg0, s32 arg1) {
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
        arg0->unk4 = 0x3815F8;
        if ((arg0 + 0x48) != NULL) {
            temp_s2 = arg0->unk48;
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
                arg0->unk48 = NULL;
            }
        }
        if ((arg0 + 0x44) != NULL) {
            temp_s2_3 = arg0->unk44;
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
                arg0->unk44 = NULL;
            }
        }
        cObject___dtor_cObject_void(arg0, 0);
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
