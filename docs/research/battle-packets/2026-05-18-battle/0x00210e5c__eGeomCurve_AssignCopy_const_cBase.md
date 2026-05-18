# Battle Packet: `eGeomCurve::AssignCopy(const cBase *)`

## Target

- Address: `0x00210e5c`
- Size: `416` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eGeomCurve`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 5B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `5`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `5`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x000780f0` `untried` 552B `eGeomCurve::operator=(const eGeomCurve &)`

## Placement

- Canonical source: `src/eGeomCurve.cpp`
- Header(s): `include/eGeomCurve.h`
- Allowed source(s): `src/eGeomCurve.cpp`
- Split-TU prefix: `src/eGeomCurve_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eGeomCurve.h`

Not found.

## Matched Same-Class Neighbors

- `0x00078318` 76B `eGeomCurve::Write(cFile &) const` — `src/eGeomCurve.cpp`
- `0x00078364` 188B `eGeomCurve::Read(cFile &, cMemPool *)` — `src/eGeomCurve.cpp`
- `0x000786b8` 124B `eGeomCurve::~eGeomCurve(void)` — `src/eGeomCurve.cpp`
- `0x00210ffc` 124B `eGeomCurve::New(cMemPool *, cBase *) static` — `src/eGeomCurve.cpp`
- `0x00211078` 276B `eGeomCurve::GetType(void) const` — `src/eGeomCurve.cpp`

## Matched Method Exemplars

### Exemplar 1: `eDynamicModel::AssignCopy(const cBase *)`

- Address: `0x001efc70`
- Size: `420` bytes
- Source: `src/eDynamicModel.cpp`

```cpp
void eDynamicModel::AssignCopy(const cBase *base) {
    const eDynamicModel *other = 0;

    if (base != 0) {
        if (D_000469DC == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                           (const char *)0x36CD7C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                       0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                                   0, 0, 0, 0);
            }
            D_000469DC = cType::InitializeType(0, 0, 0x2D, D_000469C0,
                                               &eDynamicModel::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_000469DC;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((void *)((const char *)base + offset), offset, (void *)fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = *(cType **)((char *)type + 0x1C);
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const eDynamicModel *)base;
        }
    }

    *this = *other;
}
```

### Exemplar 2: `eConfigBase::AssignCopy(const cBase *)`

- Address: `0x001df184`
- Size: `512` bytes
- Source: `src/eConfigBase.cpp`

```cpp
// ── eConfigBase::AssignCopy(const cBase *) @ 0x001df184 ──
void eConfigBase::AssignCopy(const cBase *base) {
    const eConfigBase *other = 0;

    if (base != 0) {
        if (D_00040E78 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040E78 = cType::InitializeType(0, 0, 0x28B, D_000385DC,
                                               &eConfigBase::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00040E78;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const eConfigBase *)base;
        }
    }

    *(int *)((char *)this + 0x08) = *(const int *)((const char *)other + 0x08);
    *(int *)((char *)this + 0x0C) = *(const int *)((const char *)other + 0x0C);
    *(int *)((char *)this + 0x10) = *(const int *)((const char *)other + 0x10);
    *(unsigned char *)((char *)this + 0x14) =
        *(const unsigned char *)((const char *)other + 0x14);
    *(int *)((char *)this + 0x18) = *(const int *)((const char *)other + 0x18);
    *(int *)((char *)this + 0x1C) = *(const int *)((const char *)other + 0x1C);
    *(int *)((char *)this + 0x20) = *(const int *)((const char *)other + 0x20);
    *(int *)((char *)this + 0x24) = *(const int *)((const char *)other + 0x24);
    *(unsigned char *)((char *)this + 0x28) =
        *(const unsigned char *)((const char *)other + 0x28);
    *(unsigned char *)((char *)this + 0x29) =
        *(const unsigned char *)((const char *)other + 0x29);
    *(unsigned char *)((char *)this + 0x2A) =
        *(const unsigned char *)((const char *)other + 0x2A);
    *(int *)((char *)this + 0x2C) = *(const int *)((const char *)other + 0x2C);
    *(int *)((char *)this + 0x30) = *(const int *)((const char *)other + 0x30);
    *(float *)((char *)this + 0x34) = *(const float *)((const char *)other + 0x34);
    *(float *)((char *)this + 0x38) = *(const float *)((const char *)other + 0x38);
    *(float *)((char *)this + 0x3C) = *(const float *)((const char *)other + 0x3C);
    *(float *)((char *)this + 0x40) = *(const float *)((const char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    *(unsigned char *)((char *)this + 0x48) =
        *(const unsigned char *)((const char *)other + 0x48);
    *(unsigned char *)((char *)this + 0x49) =
        *(const unsigned char *)((const char *)other + 0x49);
    *(float *)((char *)this + 0x4C) = *(const float *)((const char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(const float *)((const char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(const int *)((const char *)other + 0x54);
    *(int *)((char *)this + 0x58) = *(const int *)((const char *)other + 0x58);
    *(int *)((char *)this + 0x5C) = *(const int *)((const char *)other + 0x5C);
    *(int *)((char *)this + 0x60) = *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(const int *)((const char *)other + 0x68);
}
```

### Exemplar 3: `eReflectionModelMtl::AssignCopy(const cBase *)`

- Address: `0x00219784`
- Size: `312` bytes
- Source: `src/eReflectionModelMtl.cpp`

```cpp
void eReflectionModelMtl::AssignCopy(const cBase *base) {
    eReflectionModelMtl *other = dcast<eReflectionModelMtl>(base);
    ((eMaterial *)this)->operator=(*(const eMaterial *)other);
    *(unsigned char *)((char *)this + 0x5C) =
        *(const unsigned char *)((char *)other + 0x5C);
    *(unsigned char *)((char *)this + 0x5D) =
        *(const unsigned char *)((char *)other + 0x5D);
    *(unsigned char *)((char *)this + 0x5E) =
        *(const unsigned char *)((char *)other + 0x5E);
    *(signed char *)((char *)this + 0x5F) =
        *(const signed char *)((char *)other + 0x5F);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x64));
    int *dstWord = (int *)((char *)this + 0x68);
    int *srcWord = (int *)((char *)other + 0x68);
    int word = *srcWord;
    *dstWord = word;
    dstWord = (int *)((char *)this + 0x6C);
    srcWord = (int *)((char *)other + 0x6C);
    word = *srcWord;
    *dstWord = word;
    *(int *)((char *)this + 0x70) = *(const int *)((char *)other + 0x70);
    *(int *)((char *)this + 0x74) = *(const int *)((char *)other + 0x74);
    *(int *)((char *)this + 0x78) = *(const int *)((char *)other + 0x78);
    *(signed char *)((char *)this + 0x84) =
        *(const signed char *)((char *)other + 0x84);
    *(unsigned char *)((char *)this + 0x85) =
        *(const unsigned char *)((char *)other + 0x85);
    *(unsigned char *)((char *)this + 0x86) =
        *(const unsigned char *)((char *)other + 0x86);
    *(unsigned char *)((char *)this + 0x87) =
        *(const unsigned char *)((char *)other + 0x87);
    *(unsigned char *)((char *)this + 0x88) =
        *(const unsigned char *)((char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x89) =
        *(const unsigned char *)((char *)other + 0x89);
    __asm__ volatile("" ::: "memory");
    int i = 0;
    int *dst = (int *)((char *)this + 0x8C);
    int *src = (int *)((char *)other + 0x8C);
    do {
        int word0 = src[0];
        int word1 = src[1];
        int word2 = src[2];
        dst[0] = word0;
        dst[1] = word1;
        dst[2] = word2;
        int word3 = src[3];
        int word4 = src[4];
        dst[3] = word3;
        dst[4] = word4;
        i += 1;
        dst += 5;
        src += 5;
    } while (i < 5);
    {
        int *dstWord2 = (int *)((char *)this + 0xF0);
        int *srcWord2 = (int *)((char *)other + 0xF0);
        __asm__ volatile("" : "+r"(dstWord2), "+r"(srcWord2));
        int word2 = *srcWord2;
        *dstWord2 = word2;
    }
}
```

### Exemplar 4: `eTriangleShape::AssignCopy(const cBase *)`

- Address: `0x0020aa90`
- Size: `520` bytes
- Source: `src/eTriangleShape_AssignCopy.cpp`

```cpp
void eTriangleShape::AssignCopy(const cBase *base) {
    const eTriangleShape *other = 0;

    if (base != 0) {
        if (D_00046BDC == 0) {
            if (D_00040FE4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FE4 = cType::InitializeType(0, 0, 0x227,
                                                   D_000385DC, 0, 0, 0, 0);
            }
            const cType *parentType = D_00040FE4;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x20AC98;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046BDC = cType::InitializeType(0, 0, 0x2D5, parentType,
                                               factory, 0, 0, 0);
        }

        cType *target = D_00046BDC;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        VTableSlot *slot = (VTableSlot *)((char *)classDesc + 8);
        short offset = slot->offset;
        cType *type = slot->getType((void *)((const char *)base + offset));
        int ok;

        if (target != 0) {
            goto have_target;
        }
        ok = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop:
            if (type == target) {
                ok = 1;
            } else {
                type = ((cTypeNode *)type)->parent;
                if (type != 0) {
                    goto loop;
                }
                goto invalid;
            }
        } else {
invalid:
            ok = 0;
        }

cast_done:
        if (ok != 0) {
            other = (const eTriangleShape *)base;
        }
    }

    *(v4sf_t *)((char *)this + 0x40) =
        *(const v4sf_t *)((const char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) =
        *(const v4sf_t *)((const char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) =
        *(const v4sf_t *)((const char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) =
        *(const v4sf_t *)((const char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) =
        *(const unsigned char *)((const char *)other + 0x50);
    __asm__ volatile("" ::: "memory");

    int *dst = (int *)((char *)this + 0x54);
    const int *src = (const int *)((const char *)other + 0x54);
    int word0 = src[0];
    int word1 = src[1];
    int word2 = src[2];
    dst[0] = word0;
    dst[1] = word1;
    dst[2] = word2;
    int word3 = src[3];
    int word4 = src[4];
    int word5 = src[5];
    dst[3] = word3;
    dst[4] = word4;
    dst[5] = word5;

    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    __asm__ volatile("" ::: "memory");

    unsigned int i = 0;
    char *vecDst = (char *)this + 0x80;
    const char *vecSrc = (const char *)other + 0x80;
    do {
        *(v4sf_t *)vecDst = *(const v4sf_t *)vecSrc;
        i++;
        vecDst += 0x10;
        vecSrc += 0x10;
    } while (i < 3U);

    *(int *)((char *)this + 0xB0) =
        *(const int *)((const char *)other + 0xB0);
}
```


## Prior Failure Notes

### Note 1

- session `31a9e2a1`

Best result is 5/416 bytes different. The type initialization ladder, prologue, branch structure, bnel cast loop, final operator= call, and all existing eGeomCurve matches are aligned. The remaining mismatch is isolated to the dynamic cast dispatch setup after D_00046C20 is loaded: expected uses `addiu a1,a0,8; lh a0,0(a1); lw a1,4(a1); jalr a1; addu a0,s1,a0`, while the best C emits the semantically equivalent `addiu a0,a0,8; lh a1,0(a0); lw a2,4(a0); jalr a2; addu a0,s1,a1`. This is a register allocation/form mismatch, not bnel divergence.

Tried the exemplar dcast shape, changing eGeomCurve::New to the cBase* factory ABI, split asm constraints for the cBase name/desc and factory pointer, a memory barrier before final InitializeType, target-load pinning before classDesc, one/two/three-argument DispatchEntry callback signatures, explicit local register bindings for a0/a1, direct field access, intermediate entryVoid, declaration-order changes for offset/fn, short vs int offset, volatile function pointer (worse), and a 300s permuter run with 8856 candidates / 3694 compiled / zero improvements. Next retry should focus only on coercing the dispatch entry temp into a1 without adding instructions; everything else is already at the best known shape.


## Disassembly

```asm
210e5c:	27bdffe0 	addiu	sp,sp,-32
210e60:	afb00000 	sw	s0,0(sp)
210e64:	afb10004 	sw	s1,4(sp)
210e68:	afb20008 	sw	s2,8(sp)
210e6c:	afb3000c 	sw	s3,12(sp)
210e70:	afb40010 	sw	s4,16(sp)
210e74:	afb50014 	sw	s5,20(sp)
210e78:	afb60018 	sw	s6,24(sp)
210e7c:	afbf001c 	sw	ra,28(sp)
210e80:	34120000 	li	s2,0x0
210e84:	00808025 	move	s0,a0
210e88:	10a0004f 	beqz	a1,0x210fc8
210e8c:	00a08825 	move	s1,a1
210e90:	3c130004 	lui	s3,0x4
210e94:	8e646c20 	lw	a0,27680(s3)
210e98:	14800034 	bnez	a0,0x210f6c
210e9c:	3c140004 	lui	s4,0x4
210ea0:	8e8469c0 	lw	a0,27072(s4)
210ea4:	14800026 	bnez	a0,0x210f40
210ea8:	3c150004 	lui	s5,0x4
210eac:	8ea40ff4 	lw	a0,4084(s5)
210eb0:	14800019 	bnez	a0,0x210f18
210eb4:	3c160004 	lui	s6,0x4
210eb8:	8ec485dc 	lw	a0,-31268(s6)
210ebc:	1480000c 	bnez	a0,0x210ef0
210ec0:	3c040037 	lui	a0,0x37
210ec4:	2484cd74 	addiu	a0,a0,-12940
210ec8:	3c050037 	lui	a1,0x37
210ecc:	24a5cd7c 	addiu	a1,a1,-12932
210ed0:	34060001 	li	a2,0x1
210ed4:	00003825 	move	a3,zero
210ed8:	00004025 	move	t0,zero
210edc:	00004825 	move	t1,zero
210ee0:	00005025 	move	t2,zero
210ee4:	0c001d4d 	jal	0x7534
210ee8:	00005825 	move	t3,zero
210eec:	aec285dc 	sw	v0,-31268(s6)
210ef0:	8ec785dc 	lw	a3,-31268(s6)
210ef4:	00002025 	move	a0,zero
210ef8:	00002825 	move	a1,zero
210efc:	34060016 	li	a2,0x16
210f00:	00004025 	move	t0,zero
210f04:	00004825 	move	t1,zero
210f08:	00005025 	move	t2,zero
210f0c:	0c001d4d 	jal	0x7534
210f10:	00005825 	move	t3,zero
210f14:	aea20ff4 	sw	v0,4084(s5)
210f18:	8ea70ff4 	lw	a3,4084(s5)
210f1c:	00002025 	move	a0,zero
210f20:	00002825 	move	a1,zero
210f24:	34060017 	li	a2,0x17
210f28:	00004025 	move	t0,zero
210f2c:	00004825 	move	t1,zero
210f30:	00005025 	move	t2,zero
210f34:	0c001d4d 	jal	0x7534
210f38:	00005825 	move	t3,zero
210f3c:	ae8269c0 	sw	v0,27072(s4)
210f40:	8e8769c0 	lw	a3,27072(s4)
210f44:	3c080021 	lui	t0,0x21
210f48:	25080ffc 	addiu	t0,t0,4092
210f4c:	00002025 	move	a0,zero
210f50:	00002825 	move	a1,zero
210f54:	340601d4 	li	a2,0x1d4
210f58:	00004825 	move	t1,zero
210f5c:	00005025 	move	t2,zero
210f60:	0c001d4d 	jal	0x7534
210f64:	00005825 	move	t3,zero
210f68:	ae626c20 	sw	v0,27680(s3)
210f6c:	8e736c20 	lw	s3,27680(s3)
210f70:	8e240004 	lw	a0,4(s1)
210f74:	24850008 	addiu	a1,a0,8
210f78:	84a40000 	lh	a0,0(a1)
210f7c:	8ca50004 	lw	a1,4(a1)
210f80:	00a0f809 	jalr	a1
210f84:	02242021 	addu	a0,s1,a0
210f88:	16600003 	bnez	s3,0x210f98
210f8c:	00402025 	move	a0,v0
210f90:	1000000a 	b	0x210fbc
210f94:	34040000 	li	a0,0x0
210f98:	50800008 	beqzl	a0,0x210fbc
210f9c:	34040000 	li	a0,0x0
210fa0:	54930003 	bnel	a0,s3,0x210fb0
210fa4:	8c84001c 	lw	a0,28(a0)
210fa8:	10000004 	b	0x210fbc
210fac:	34040001 	li	a0,0x1
210fb0:	1480fffb 	bnez	a0,0x210fa0
210fb4:	00000000 	nop
210fb8:	34040000 	li	a0,0x0
210fbc:	10800003 	beqz	a0,0x210fcc
210fc0:	02002025 	move	a0,s0
210fc4:	02209025 	move	s2,s1
210fc8:	02002025 	move	a0,s0
210fcc:	0c01e03c 	jal	0x780f0
210fd0:	02402825 	move	a1,s2
210fd4:	8fb00000 	lw	s0,0(sp)
210fd8:	8fb10004 	lw	s1,4(sp)
210fdc:	8fb20008 	lw	s2,8(sp)
210fe0:	8fb3000c 	lw	s3,12(sp)
210fe4:	8fb40010 	lw	s4,16(sp)
210fe8:	8fb50014 	lw	s5,20(sp)
210fec:	8fb60018 	lw	s6,24(sp)
210ff0:	8fbf001c 	lw	ra,28(sp)
210ff4:	03e00008 	jr	ra
210ff8:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eGeomCurve::AssignCopy(const cBase *)
// Address: 0x00210e5c, Size: 416B
// Obj: eAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? eGeomCurve__operator_eq_consteGeomCurveref(s32, void *); /* extern */

void eGeomCurve__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a1)(void *, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x46C20 == 0) {
            if (*(s32 *)0x469C0 == 0) {
                if (*(s32 *)0x40FF4 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                    }
                    *(s32 *)0x40FF4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x16, *(s32 *)0x385DC);
                }
                *(s32 *)0x469C0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x17, *(s32 *)0x40FF4);
            }
            *(s32 *)0x46C20 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x1D4, *(s32 *)0x469C0);
        }
        temp_s3 = *(s32 *)0x46C20;
        temp_a0 = arg1->unk4;
        temp_a1 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a1(arg1 + temp_a0->unk8, temp_a1);
        if (temp_s3 == 0) {
            goto block_17;
        }
        if (var_a0 != NULL) {
loop_12:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_17;
                }
                goto loop_12;
            }
            var_a0_2 = 1;
        } else {
block_17:
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            var_s2 = arg1;
        }
    }
    eGeomCurve__operator_eq_consteGeomCurveref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
