# Battle Packet: `eFont::~eFont(void)`

## Target

- Address: `0x001e7efc`
- Size: `560` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eFont`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `135`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00009dc4` `failed` 432B `cObject::~cObject(void)`

## Placement

- Canonical source: `src/eFont.cpp`
- Header(s): `include/eFont.h`
- Allowed source(s): `src/eFont.cpp`
- Split-TU prefix: `src/eFont_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eFont.h`

```cpp
#ifndef EFONT_H
#define EFONT_H

class cReadBlock;

class eFont {
public:
    void ReadKerningTableOld(cReadBlock &);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00039278` 384B `eFont::Write(cFile &) const` — `src/eFont.cpp`
- `0x00039710` 104B `eFont::eFont(cBase *)` — `src/eFont.cpp`
- `0x00039b14` 52B `eFont::HasTags(const wchar_t *) static` — `src/eInput.cpp`
- `0x001e7948` 140B `eFont::AssignCopy(const cBase *)` — `src/eFont.cpp`
- `0x001e79d4` 124B `eFont::New(cMemPool *, cBase *) static` — `src/eFont.cpp`

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

### Exemplar 3: `eDynamicMesh::~eDynamicMesh(void)`

- Address: `0x0004e2cc`
- Size: `644` bytes
- Source: `src/eDynamicMesh.cpp`

```cpp
eDynamicMesh::~eDynamicMesh() {
    *(void **)((char *)this + 4) = (void *)0x381740;

    Free();

    cBaseArray *shapes = (cBaseArray *)((char *)this + 0x74);
    cArrayBase<eDynamicMeshLookAt> *lookAts =
        (cArrayBase<eDynamicMeshLookAt> *)((char *)this + 0x70);
    cArrayBase<eDynamicMeshBone> *bones =
        (cArrayBase<eDynamicMeshBone> *)((char *)this + 0x6C);
    cArrayBase<eDynamicMeshNode> *nodes =
        (cArrayBase<eDynamicMeshNode> *)((char *)this + 0x68);

    if (shapes != 0) {
        shapes->RemoveAll();
    }

    if (lookAts != 0) {
        void *entries = *(void **)((char *)this + 0x70);
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
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x70) = 0;
        }
    }

    if (bones != 0) {
        void *entries = *(void **)((char *)this + 0x6C);
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
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x6C) = 0;
        }
    }

    if (nodes != 0) {
        void *entries = *(void **)((char *)this + 0x68);
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
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x68) = 0;
        }
    }

    eMesh___dtor_eMesh_void(this, 0);
}
```

### Exemplar 4: `eDecalSystem::~eDecalSystem(void)`

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


## Prior Failure Notes

### Note 1

- session `055c0846`; src `src/eFont.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/055c0846/001e7efc__agent_self_reported_failure__src_eFont.cpp`

Added the canonical eFont destructor body using the matched cArray cleanup pattern for offsets 0x60, 0x5C, and 0x48, inline class-local operator delete with the null guard needed for the deleting tail, implicit cObject base destruction, and the trailing infinite-loop words with an explicit .size so the symbol size is 0x230. Existing eFont ctor/Write/AssignCopy/New/GetType remain matched. The current destructor is a 12-byte mismatch: instruction selection, stack size, control flow, calls, and immediates match, but SNC assigns the preserved locals as s2/s3/s4 differently from the original around the array address/entries variables. Tried local declaration reordering, shared entries variable, register-asm hints, and a 300-second permuter run; the permuter compiled 3614 candidates and found zero improvement. category=REG_ALLOC.


## Disassembly

```asm
1e7efc:	27bdffe0 	addiu	sp,sp,-32
1e7f00:	afb00000 	sw	s0,0(sp)
1e7f04:	00808025 	move	s0,a0
1e7f08:	afb10004 	sw	s1,4(sp)
1e7f0c:	afb20008 	sw	s2,8(sp)
1e7f10:	afb3000c 	sw	s3,12(sp)
1e7f14:	afb40010 	sw	s4,16(sp)
1e7f18:	afbf0014 	sw	ra,20(sp)
1e7f1c:	10800079 	beqz	a0,0x1e8104
1e7f20:	00a08825 	move	s1,a1
1e7f24:	3c040038 	lui	a0,0x38
1e7f28:	24840748 	addiu	a0,a0,1864
1e7f2c:	ae040004 	sw	a0,4(s0)
1e7f30:	26040060 	addiu	a0,s0,96
1e7f34:	2613005c 	addiu	s3,s0,92
1e7f38:	1080001f 	beqz	a0,0x1e7fb8
1e7f3c:	26140048 	addiu	s4,s0,72
1e7f40:	8e120060 	lw	s2,96(s0)
1e7f44:	12400005 	beqz	s2,0x1e7f5c
1e7f48:	34040000 	li	a0,0x0
1e7f4c:	8e44fffc 	lw	a0,-4(s2)
1e7f50:	3c054000 	lui	a1,0x4000
1e7f54:	24a5ffff 	addiu	a1,a1,-1
1e7f58:	00852024 	and	a0,a0,a1
1e7f5c:	34050000 	li	a1,0x0
1e7f60:	00a4302a 	slt	a2,a1,a0
1e7f64:	10c00005 	beqz	a2,0x1e7f7c
1e7f68:	00000000 	nop
1e7f6c:	24a50001 	addiu	a1,a1,1
1e7f70:	00a4302a 	slt	a2,a1,a0
1e7f74:	14c0fffe 	bnez	a2,0x1e7f70
1e7f78:	24a50001 	addiu	a1,a1,1
1e7f7c:	1240000e 	beqz	s2,0x1e7fb8
1e7f80:	2652fffc 	addiu	s2,s2,-4
1e7f84:	5240000c 	beqzl	s2,0x1e7fb8
1e7f88:	ae000060 	sw	zero,96(s0)
1e7f8c:	0c0011e8 	jal	0x47a0
1e7f90:	02402025 	move	a0,s2
1e7f94:	8c440024 	lw	a0,36(v0)
1e7f98:	02402825 	move	a1,s2
1e7f9c:	8c86001c 	lw	a2,28(a0)
1e7fa0:	24c60030 	addiu	a2,a2,48
1e7fa4:	84c70000 	lh	a3,0(a2)
1e7fa8:	8cc60004 	lw	a2,4(a2)
1e7fac:	00c0f809 	jalr	a2
1e7fb0:	00872021 	addu	a0,a0,a3
1e7fb4:	ae000060 	sw	zero,96(s0)
1e7fb8:	1260001f 	beqz	s3,0x1e8038
1e7fbc:	00000000 	nop
1e7fc0:	8e12005c 	lw	s2,92(s0)
1e7fc4:	12400005 	beqz	s2,0x1e7fdc
1e7fc8:	34040000 	li	a0,0x0
1e7fcc:	8e44fffc 	lw	a0,-4(s2)
1e7fd0:	3c054000 	lui	a1,0x4000
1e7fd4:	24a5ffff 	addiu	a1,a1,-1
1e7fd8:	00852024 	and	a0,a0,a1
1e7fdc:	34050000 	li	a1,0x0
1e7fe0:	00a4302a 	slt	a2,a1,a0
1e7fe4:	10c00005 	beqz	a2,0x1e7ffc
1e7fe8:	00000000 	nop
1e7fec:	24a50001 	addiu	a1,a1,1
1e7ff0:	00a4302a 	slt	a2,a1,a0
1e7ff4:	14c0fffe 	bnez	a2,0x1e7ff0
1e7ff8:	24a50001 	addiu	a1,a1,1
1e7ffc:	1240000e 	beqz	s2,0x1e8038
1e8000:	2652fffc 	addiu	s2,s2,-4
1e8004:	5240000c 	beqzl	s2,0x1e8038
1e8008:	ae00005c 	sw	zero,92(s0)
1e800c:	0c0011e8 	jal	0x47a0
1e8010:	02402025 	move	a0,s2
1e8014:	8c440024 	lw	a0,36(v0)
1e8018:	02402825 	move	a1,s2
1e801c:	8c86001c 	lw	a2,28(a0)
1e8020:	24c60030 	addiu	a2,a2,48
1e8024:	84c70000 	lh	a3,0(a2)
1e8028:	8cc60004 	lw	a2,4(a2)
1e802c:	00c0f809 	jalr	a2
1e8030:	00872021 	addu	a0,a0,a3
1e8034:	ae00005c 	sw	zero,92(s0)
1e8038:	12800021 	beqz	s4,0x1e80c0
1e803c:	02002025 	move	a0,s0
1e8040:	8e120048 	lw	s2,72(s0)
1e8044:	12400005 	beqz	s2,0x1e805c
1e8048:	34040000 	li	a0,0x0
1e804c:	8e44fffc 	lw	a0,-4(s2)
1e8050:	3c054000 	lui	a1,0x4000
1e8054:	24a5ffff 	addiu	a1,a1,-1
1e8058:	00852024 	and	a0,a0,a1
1e805c:	34050000 	li	a1,0x0
1e8060:	00a4302a 	slt	a2,a1,a0
1e8064:	10c00005 	beqz	a2,0x1e807c
1e8068:	00000000 	nop
1e806c:	24a50001 	addiu	a1,a1,1
1e8070:	00a4302a 	slt	a2,a1,a0
1e8074:	14c0fffe 	bnez	a2,0x1e8070
1e8078:	24a50001 	addiu	a1,a1,1
1e807c:	12400010 	beqz	s2,0x1e80c0
1e8080:	02002025 	move	a0,s0
1e8084:	2652fffc 	addiu	s2,s2,-4
1e8088:	5240000c 	beqzl	s2,0x1e80bc
1e808c:	ae000048 	sw	zero,72(s0)
1e8090:	0c0011e8 	jal	0x47a0
1e8094:	02402025 	move	a0,s2
1e8098:	8c440024 	lw	a0,36(v0)
1e809c:	02402825 	move	a1,s2
1e80a0:	8c86001c 	lw	a2,28(a0)
1e80a4:	24c60030 	addiu	a2,a2,48
1e80a8:	84c70000 	lh	a3,0(a2)
1e80ac:	8cc60004 	lw	a2,4(a2)
1e80b0:	00c0f809 	jalr	a2
1e80b4:	00872021 	addu	a0,a0,a3
1e80b8:	ae000048 	sw	zero,72(s0)
1e80bc:	02002025 	move	a0,s0
1e80c0:	0c002771 	jal	0x9dc4
1e80c4:	00002825 	move	a1,zero
1e80c8:	32240001 	andi	a0,s1,0x1
1e80cc:	1080000d 	beqz	a0,0x1e8104
1e80d0:	00000000 	nop
1e80d4:	1200000b 	beqz	s0,0x1e8104
1e80d8:	00000000 	nop
1e80dc:	0c0011e8 	jal	0x47a0
1e80e0:	02002025 	move	a0,s0
1e80e4:	8c440024 	lw	a0,36(v0)
1e80e8:	02002825 	move	a1,s0
1e80ec:	8c86001c 	lw	a2,28(a0)
1e80f0:	24c60030 	addiu	a2,a2,48
1e80f4:	84c70000 	lh	a3,0(a2)
1e80f8:	8cc60004 	lw	a2,4(a2)
1e80fc:	00c0f809 	jalr	a2
1e8100:	00872021 	addu	a0,a0,a3
1e8104:	8fb00000 	lw	s0,0(sp)
1e8108:	8fb10004 	lw	s1,4(sp)
1e810c:	8fb20008 	lw	s2,8(sp)
1e8110:	8fb3000c 	lw	s3,12(sp)
1e8114:	8fb40010 	lw	s4,16(sp)
1e8118:	8fbf0014 	lw	ra,20(sp)
1e811c:	03e00008 	jr	ra
1e8120:	27bd0020 	addiu	sp,sp,32
1e8124:	1000ffff 	b	0x1e8124
1e8128:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eFont::~eFont(void)
// Address: 0x001e7efc, Size: 560B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? cObject___dtor_cObject_void(void *, ?);           /* extern */

void eFont___dtor_eFont_void(void *arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    ? (*temp_a2_4)(void *, void *, ?, s16);
    ? (*temp_a2_6)(void *, void *, ?, s16);
    ? (*temp_a2_8)(void *, void *, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_a3_3;
    s16 temp_a3_4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a2;
    void *temp_a2_3;
    void *temp_a2_5;
    void *temp_a2_7;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_s2_3;
    void *temp_s2_4;
    void *temp_s2_5;
    void *temp_s2_6;

    if (arg0 != NULL) {
        arg0->unk4 = 0x380748;
        if ((arg0 + 0x60) != NULL) {
            temp_s2 = arg0->unk60;
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
                arg0->unk60 = NULL;
            }
        }
        if ((arg0 + 0x5C) != NULL) {
            temp_s2_3 = arg0->unk5C;
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
                arg0->unk5C = NULL;
            }
        }
        if ((arg0 + 0x48) != NULL) {
            temp_s2_5 = arg0->unk48;
            var_a0_3 = 0;
            if (temp_s2_5 != NULL) {
                var_a0_3 = temp_s2_5->unk-4 & 0x3FFFFFFF;
            }
            var_a1_3 = 0;
            var_a2_3 = var_a0_3 > 0;
            if (var_a2_3 != 0) {
                var_a1_3 = 1;
                do {
                    var_a2_3 = var_a1_3 < var_a0_3;
                    var_a1_3 += 1;
                } while (var_a2_3 != 0);
            }
            if (temp_s2_5 != NULL) {
                temp_s2_6 = temp_s2_5 - 4;
                if (temp_s2_6 != NULL) {
                    temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_6, var_a1_3, var_a2_3)->unk24;
                    temp_a2_5 = temp_a0_3->unk1C;
                    temp_a3_3 = temp_a2_5->unk30;
                    temp_a2_6 = (temp_a2_5 + 0x30)->unk4;
                    temp_a2_6(temp_a0_3 + temp_a3_3, temp_s2_6, temp_a2_6, temp_a3_3);
                }
                arg0->unk48 = NULL;
            }
        }
        cObject___dtor_cObject_void(arg0, 0);
        if ((arg1 & 1) && (arg0 != NULL)) {
            temp_a0_4 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a2_7 = temp_a0_4->unk1C;
            temp_a3_4 = temp_a2_7->unk30;
            temp_a2_8 = (temp_a2_7 + 0x30)->unk4;
            temp_a2_8(temp_a0_4 + temp_a3_4, arg0, temp_a2_8, temp_a3_4);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
