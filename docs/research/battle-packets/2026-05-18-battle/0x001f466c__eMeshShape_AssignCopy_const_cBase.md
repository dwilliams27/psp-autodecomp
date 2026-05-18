# Battle Packet: `eMeshShape::AssignCopy(const cBase *)`

## Target

- Address: `0x001f466c`
- Size: `568` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMeshShape`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 14B; AssignCopy retry family`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `14`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `14`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x001daec4` `untried` 584B `cArrayBase<cArrayBase> & cArrayBase<mBox>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e0668` `untried` 584B `cArrayBase<cArrayBase> & cArrayBase<mPlane>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001f3d54` `untried` 584B `cArrayBase<cArrayBase> & cArrayBase<eMeshShapeTriList>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001f3f9c` `untried` 560B `cArrayBase<cArrayBase> & cArrayBase<eMeshShapeNode>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001f41cc` `untried` 616B `cArrayBase<cArrayBase> & cArrayBase<eMeshShapeTriangle>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001f4434` `untried` 568B `cArrayBase<cArrayBase> & cArrayBase<mVec3>::operator=(const cArrayBase<cArrayBase> &)`

## Placement

- Canonical source: `src/eMeshShape.cpp`
- Header(s): `include/eMeshShape.h`
- Allowed source(s): `src/eMeshShape.cpp`, `src/eMeshShape_AssignCopy.cpp`
- Split-TU prefix: `src/eMeshShape_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eMeshShape.h`

```cpp
#ifndef EMESHSHAPE_H
#define EMESHSHAPE_H

class eCollisionContactInfo;
class eHeightmapShape;
class eShape;
class cBase;
class cMemPool;
class cType;
class mOCS;

class eMeshShape {
public:
    eMeshShape(cBase *);

    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    const cType *GetType(void) const;
    float GetVolume(void) const;

    static eMeshShape *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0004ee2c` 80B `eMeshShape::eMeshShape(cBase *)` — `src/eMeshShape.cpp`
- `0x00050270` 8B `eMeshShape::Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eMeshShape_Collide_eMeshShape.cpp`
- `0x00050278` 8B `eMeshShape::Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const` — `src/eMeshShape_Collide_eHeightmapShape.cpp`
- `0x001f48a4` 124B `eMeshShape::New(cMemPool *, cBase *) static` — `src/eMeshShape.cpp`
- `0x001f4920` 216B `eMeshShape::GetType(void) const` — `src/eMeshShape.cpp`

## Matched Method Exemplars

### Exemplar 1: `eMultiSphereShape::AssignCopy(const cBase *)`

- Address: `0x0020914c`
- Size: `544` bytes
- Source: `src/eMultiSphereShape_AssignCopy.cpp`

```cpp
void eMultiSphereShape::AssignCopy(const cBase *base) {
    const eMultiSphereShape *other = 0;

    if (base != 0) {
        if (D_00046BBC == 0) {
            if (D_00040FE4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FE4 = cType::InitializeType(
                    0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
            }
            const cType *parentType = D_00040FE4;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x20936C;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046BBC = cType::InitializeType(
                0, 0, 0x1DB, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046BBC;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *type = entry->fn((void *)((const char *)base + offset));
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
                type = (cType *)type->mParent;
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
            other = (const eMultiSphereShape *)base;
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
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(float *)((char *)this + 0x80) =
        *(const float *)((const char *)other + 0x80);
    *(float *)((char *)this + 0x84) =
        *(const float *)((const char *)other + 0x84);
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);

    unsigned int i = 0;
    char *vecDst = (char *)this + 0x90;
    const char *vecSrc = (const char *)other + 0x90;
    do {
        float last = ((const float *)vecSrc)[3];
        *(v4sf_t *)vecDst = *(const v4sf_t *)vecSrc;
        ((float *)vecDst)[3] = last;
        __asm__ volatile("" ::: "memory");
        i++;
        vecDst += 0x10;
        vecSrc += 0x10;
    } while (i < 4U);
}
```

### Exemplar 2: `eOnePassModelMtl::AssignCopy(const cBase *)`

- Address: `0x002186c8`
- Size: `600` bytes
- Source: `src/eOnePassModelMtl_AssignCopy.cpp`

```cpp
// 0x002186c8 -- eOnePassModelMtl::AssignCopy(const cBase *)
void eOnePassModelMtl::AssignCopy(const cBase *base) {
    const eOnePassModelMtl *other = 0;

    if (base != 0) {
        if (D_00046C7C == 0) {
            if (D_00046B28 == 0) {
                if (D_00040FF8 == 0) {
                    if (D_00040FEC == 0) {
                        if (D_000385E4 == 0) {
                            if (D_000385E0 == 0) {
                                if (D_000385DC == 0) {
                                    const char *name = (const char *)0x36CD74;
                                    const char *desc = (const char *)0x36CD7C;
                                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                                    D_000385DC = cType::InitializeType(
                                        name, desc, 1, 0, 0, 0, 0, 0);
                                }
                                const cType *parentType = D_000385DC;
                                cBase *(*factory)(cMemPool *, cBase *) =
                                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                                D_000385E0 = cType::InitializeType(
                                    0, 0, 2, parentType, factory, 0, 0, 0);
                            }
                            D_000385E4 = cType::InitializeType(
                                0, 0, 3, D_000385E0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385E4;
                        const char *kindName = (const char *)0x36CDCC;
                        const char *kindDesc = (const char *)0x36CDD8;
                        __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                        D_00040FEC = cType::InitializeType(
                            0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
                    }
                    D_00040FF8 = cType::InitializeType(
                        0, 0, 0x12, D_00040FEC, 0, 0, 0, 0);
                }
                D_00046B28 = cType::InitializeType(
                    0, 0, 0x13, D_00040FF8, 0, 0, 0, 0);
            }
            const cType *parentType = D_00046B28;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x218920;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C7C = cType::InitializeType(
                0, 0, 0x1B, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C7C;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *type = entry->fn((void *)((const char *)base + offset));
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
                type = (cType *)type->mParent;
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
            other = (const eOnePassModelMtl *)base;
        }
    }

    *this = *other;
}
```

### Exemplar 3: `eRefractionModelMtl::AssignCopy(const cBase *)`

- Address: `0x00219fbc`
- Size: `600` bytes
- Source: `src/eRefractionModelMtl_AssignCopy.cpp`

```cpp
void eRefractionModelMtl::AssignCopy(const cBase *base) {
    const eRefractionModelMtl *other = 0;

    if (base != 0) {
        if (D_00046C94 == 0) {
            if (D_00046B28 == 0) {
                if (D_00040FF8 == 0) {
                    if (D_00040FEC == 0) {
                        if (D_000385E4 == 0) {
                            if (D_000385E0 == 0) {
                                if (D_000385DC == 0) {
                                    const char *name = (const char *)0x36CD74;
                                    const char *desc = (const char *)0x36CD7C;
                                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                                    D_000385DC = cType::InitializeType(
                                        name, desc, 1, 0, 0, 0, 0, 0);
                                }
                                const cType *parentType = D_000385DC;
                                cBase *(*factory)(cMemPool *, cBase *) =
                                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                                D_000385E0 = cType::InitializeType(
                                    0, 0, 2, parentType, factory, 0, 0, 0);
                            }
                            D_000385E4 = cType::InitializeType(
                                0, 0, 3, D_000385E0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385E4;
                        const char *kindName = (const char *)0x36CDCC;
                        const char *kindDesc = (const char *)0x36CDD8;
                        __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                        D_00040FEC = cType::InitializeType(
                            0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
                    }
                    D_00040FF8 = cType::InitializeType(
                        0, 0, 0x12, D_00040FEC, 0, 0, 0, 0);
                }
                D_00046B28 = cType::InitializeType(
                    0, 0, 0x13, D_00040FF8, 0, 0, 0, 0);
            }
            const cType *parentType = D_00046B28;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x21A214;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C94 = cType::InitializeType(
                0, 0, 0x13A, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C94;
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
            other = (const eRefractionModelMtl *)base;
        }
    }

    *this = *other;
}
```

### Exemplar 4: `eSilhouetteModelMtl::AssignCopy(const cBase *)`

- Address: `0x0021b0e8`
- Size: `600` bytes
- Source: `src/eSilhouetteModelMtl_AssignCopy.cpp`

```cpp
void eSilhouetteModelMtl::AssignCopy(const cBase *base) {
    const eSilhouetteModelMtl *other = 0;

    if (base != 0) {
        if (D_00046CA4 == 0) {
            if (D_00046B28 == 0) {
                if (D_00040FF8 == 0) {
                    if (D_00040FEC == 0) {
                        if (D_000385E4 == 0) {
                            if (D_000385E0 == 0) {
                                if (D_000385DC == 0) {
                                    const char *name = (const char *)0x36CD74;
                                    const char *desc = (const char *)0x36CD7C;
                                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                                    D_000385DC = cType::InitializeType(
                                        name, desc, 1, 0, 0, 0, 0, 0);
                                }
                                const cType *parentType = D_000385DC;
                                cBase *(*factory)(cMemPool *, cBase *) =
                                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                                D_000385E0 = cType::InitializeType(
                                    0, 0, 2, parentType, factory, 0, 0, 0);
                            }
                            D_000385E4 = cType::InitializeType(
                                0, 0, 3, D_000385E0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385E4;
                        const char *kindName = (const char *)0x36CDCC;
                        const char *kindDesc = (const char *)0x36CDD8;
                        __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                        D_00040FEC = cType::InitializeType(
                            0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
                    }
                    D_00040FF8 = cType::InitializeType(
                        0, 0, 0x12, D_00040FEC, 0, 0, 0, 0);
                }
                D_00046B28 = cType::InitializeType(
                    0, 0, 0x13, D_00040FF8, 0, 0, 0, 0);
            }
            const cType *parentType = D_00046B28;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x21B340;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046CA4 = cType::InitializeType(
                0, 0, 0x80, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046CA4;
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
            other = (const eSilhouetteModelMtl *)base;
        }
    }

    *this = *other;
}
```


## Prior Failure Notes

### Note 1

- session `70f42885`; src `src/eMeshShape_AssignCopy.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/70f42885/001f466c__agent_self_reported_failure__src_eMeshShape_AssignCopy.cpp`

Implemented eMeshShape::AssignCopy(const cBase *) in a split TU with a local class redeclaration, matching the target's type initialization chain for D_00046A04, virtual type walk, VFPU struct copies, byte/word/float field copies, six cArrayBase assignments, final u16 copy, and two trailing VFPU quad copies. Tried straight exemplar-style C, explicit name/desc and parent/factory temporaries, register-bound self/source/other variants, and reordered block-copy loads to match the expected a2/a3/t0 load grouping.

The closest source is the permuter-saved version in src/eMeshShape_AssignCopy.cpp. compare_func reports 14/568 bytes differing after relocation masking. The remaining bytes are a register-allocation drift: SNC assigns the destination object and copied object to different s-registers than the original in the prologue and throughout the copy tail (notably s0/s2 roles), while the control flow, calls, and copy order are otherwise aligned. A 60s permuter run improved the function from 67 differing bytes to the current 14 but did not find a match.


## Disassembly

```asm
1f466c:	27bdffe0 	addiu	sp,sp,-32
1f4670:	afb00000 	sw	s0,0(sp)
1f4674:	afb10004 	sw	s1,4(sp)
1f4678:	afb20008 	sw	s2,8(sp)
1f467c:	afb3000c 	sw	s3,12(sp)
1f4680:	afb40010 	sw	s4,16(sp)
1f4684:	afb50014 	sw	s5,20(sp)
1f4688:	afbf0018 	sw	ra,24(sp)
1f468c:	34120000 	li	s2,0x0
1f4690:	00808025 	move	s0,a0
1f4694:	10a00042 	beqz	a1,0x1f47a0
1f4698:	00a08825 	move	s1,a1
1f469c:	3c130004 	lui	s3,0x4
1f46a0:	8e646a04 	lw	a0,27140(s3)
1f46a4:	14800027 	bnez	a0,0x1f4744
1f46a8:	3c140004 	lui	s4,0x4
1f46ac:	8e840fe4 	lw	a0,4068(s4)
1f46b0:	14800019 	bnez	a0,0x1f4718
1f46b4:	3c150004 	lui	s5,0x4
1f46b8:	8ea485dc 	lw	a0,-31268(s5)
1f46bc:	1480000c 	bnez	a0,0x1f46f0
1f46c0:	3c040037 	lui	a0,0x37
1f46c4:	2484cd74 	addiu	a0,a0,-12940
1f46c8:	3c050037 	lui	a1,0x37
1f46cc:	24a5cd7c 	addiu	a1,a1,-12932
1f46d0:	34060001 	li	a2,0x1
1f46d4:	00003825 	move	a3,zero
1f46d8:	00004025 	move	t0,zero
1f46dc:	00004825 	move	t1,zero
1f46e0:	00005025 	move	t2,zero
1f46e4:	0c001d4d 	jal	0x7534
1f46e8:	00005825 	move	t3,zero
1f46ec:	aea285dc 	sw	v0,-31268(s5)
1f46f0:	8ea785dc 	lw	a3,-31268(s5)
1f46f4:	00002025 	move	a0,zero
1f46f8:	00002825 	move	a1,zero
1f46fc:	34060227 	li	a2,0x227
1f4700:	00004025 	move	t0,zero
1f4704:	00004825 	move	t1,zero
1f4708:	00005025 	move	t2,zero
1f470c:	0c001d4d 	jal	0x7534
1f4710:	00005825 	move	t3,zero
1f4714:	ae820fe4 	sw	v0,4068(s4)
1f4718:	8e870fe4 	lw	a3,4068(s4)
1f471c:	3c08001f 	lui	t0,0x1f
1f4720:	250848a4 	addiu	t0,t0,18596
1f4724:	00002025 	move	a0,zero
1f4728:	00002825 	move	a1,zero
1f472c:	3406022c 	li	a2,0x22c
1f4730:	00004825 	move	t1,zero
1f4734:	00005025 	move	t2,zero
1f4738:	0c001d4d 	jal	0x7534
1f473c:	00005825 	move	t3,zero
1f4740:	ae626a04 	sw	v0,27140(s3)
1f4744:	8e736a04 	lw	s3,27140(s3)
1f4748:	8e240004 	lw	a0,4(s1)
1f474c:	24850008 	addiu	a1,a0,8
1f4750:	84a40000 	lh	a0,0(a1)
1f4754:	8ca50004 	lw	a1,4(a1)
1f4758:	00a0f809 	jalr	a1
1f475c:	02242021 	addu	a0,s1,a0
1f4760:	16600003 	bnez	s3,0x1f4770
1f4764:	00402025 	move	a0,v0
1f4768:	1000000a 	b	0x1f4794
1f476c:	34040000 	li	a0,0x0
1f4770:	50800008 	beqzl	a0,0x1f4794
1f4774:	34040000 	li	a0,0x0
1f4778:	54930003 	bnel	a0,s3,0x1f4788
1f477c:	8c84001c 	lw	a0,28(a0)
1f4780:	10000004 	b	0x1f4794
1f4784:	34040001 	li	a0,0x1
1f4788:	1480fffb 	bnez	a0,0x1f4778
1f478c:	00000000 	nop
1f4790:	34040000 	li	a0,0x0
1f4794:	10800002 	beqz	a0,0x1f47a0
1f4798:	00000000 	nop
1f479c:	02209025 	move	s2,s1
1f47a0:	da460040 	lv.q        C120, 0x40($s2)
1f47a4:	fa060040 	sv.q        C120, 0x40($s0)
1f47a8:	da460010 	lv.q        C120, 0x10($s2)
1f47ac:	fa060010 	sv.q        C120, 0x10($s0)
1f47b0:	da460020 	lv.q        C120, 0x20($s2)
1f47b4:	fa060020 	sv.q        C120, 0x20($s0)
1f47b8:	da460030 	lv.q        C120, 0x30($s2)
1f47bc:	fa060030 	sv.q        C120, 0x30($s0)
1f47c0:	92440050 	lbu	a0,80(s2)
1f47c4:	a2040050 	sb	a0,80(s0)
1f47c8:	26040054 	addiu	a0,s0,84
1f47cc:	26450054 	addiu	a1,s2,84
1f47d0:	8ca60000 	lw	a2,0(a1)
1f47d4:	8ca70004 	lw	a3,4(a1)
1f47d8:	8ca80008 	lw	t0,8(a1)
1f47dc:	ac860000 	sw	a2,0(a0)
1f47e0:	ac870004 	sw	a3,4(a0)
1f47e4:	ac880008 	sw	t0,8(a0)
1f47e8:	8ca6000c 	lw	a2,12(a1)
1f47ec:	8ca70010 	lw	a3,16(a1)
1f47f0:	8ca50014 	lw	a1,20(a1)
1f47f4:	ac86000c 	sw	a2,12(a0)
1f47f8:	ac870010 	sw	a3,16(a0)
1f47fc:	ac850014 	sw	a1,20(a0)
1f4800:	8e44006c 	lw	a0,108(s2)
1f4804:	ae04006c 	sw	a0,108(s0)
1f4808:	8e440070 	lw	a0,112(s2)
1f480c:	ae040070 	sw	a0,112(s0)
1f4810:	c64c0074 	lwc1	$f12,116(s2)
1f4814:	e60c0074 	swc1	$f12,116(s0)
1f4818:	c64c0078 	lwc1	$f12,120(s2)
1f481c:	e60c0078 	swc1	$f12,120(s0)
1f4820:	26040080 	addiu	a0,s0,128
1f4824:	0c07d10d 	jal	0x1f4434
1f4828:	26450080 	addiu	a1,s2,128
1f482c:	26040084 	addiu	a0,s0,132
1f4830:	0c07819a 	jal	0x1e0668
1f4834:	26450084 	addiu	a1,s2,132
1f4838:	26040088 	addiu	a0,s0,136
1f483c:	0c07d073 	jal	0x1f41cc
1f4840:	26450088 	addiu	a1,s2,136
1f4844:	2604008c 	addiu	a0,s0,140
1f4848:	0c076bb1 	jal	0x1daec4
1f484c:	2645008c 	addiu	a1,s2,140
1f4850:	26040090 	addiu	a0,s0,144
1f4854:	0c07cfe7 	jal	0x1f3f9c
1f4858:	26450090 	addiu	a1,s2,144
1f485c:	26040094 	addiu	a0,s0,148
1f4860:	0c07cf55 	jal	0x1f3d54
1f4864:	26450094 	addiu	a1,s2,148
1f4868:	96440098 	lhu	a0,152(s2)
1f486c:	a6040098 	sh	a0,152(s0)
1f4870:	da4600a0 	lv.q        C120, 0xA0($s2)
1f4874:	fa0600a0 	sv.q        C120, 0xA0($s0)
1f4878:	da4600b0 	lv.q        C120, 0xB0($s2)
1f487c:	fa0600b0 	sv.q        C120, 0xB0($s0)
1f4880:	8fb00000 	lw	s0,0(sp)
1f4884:	8fb10004 	lw	s1,4(sp)
1f4888:	8fb20008 	lw	s2,8(sp)
1f488c:	8fb3000c 	lw	s3,12(sp)
1f4890:	8fb40010 	lw	s4,16(sp)
1f4894:	8fb50014 	lw	s5,20(sp)
1f4898:	8fbf0018 	lw	ra,24(sp)
1f489c:	03e00008 	jr	ra
1f48a0:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eMeshShape::AssignCopy(const cBase *)
// Address: 0x001f466c, Size: 568B
// Obj: eAll_psp.obj

? cArrayBase_cArrayBase_refcArrayBase_eMeshShapeNode___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_eMeshShapeTriList___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_eMeshShapeTriangle___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_mBox___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_mPlane___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_mVec3___operator_eq_constcArrayBase_cArrayBase_ref(f32, void *, void *, s32, s32); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

void eMeshShape__AssignCopy_constcBaseptr(void *arg0, void *arg1) {
    f32 temp_f12;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a1)(void *, ?);
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1_2;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x46A04 == 0) {
            if (*(s32 *)0x40FE4 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                }
                *(s32 *)0x40FE4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x227, *(s32 *)0x385DC);
            }
            *(s32 *)0x46A04 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x22C, *(s32 *)0x40FE4);
        }
        temp_s3 = *(s32 *)0x46A04;
        temp_a0 = arg1->unk4;
        temp_a1 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a1(arg1 + temp_a0->unk8, temp_a1);
        if (temp_s3 == 0) {
            goto block_15;
        }
        if (var_a0 != NULL) {
loop_10:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_15;
                }
                goto loop_10;
            }
            var_a0_2 = 1;
        } else {
block_15:
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            var_s2 = arg1;
        }
    }
    __asm__ volatile("lv.q C120, 0x40($s2)");
    __asm__ volatile("sv.q C120, 0x40($s0)");
    __asm__ volatile("lv.q C120, 0x10($s2)");
    __asm__ volatile("sv.q C120, 0x10($s0)");
    __asm__ volatile("lv.q C120, 0x20($s2)");
    __asm__ volatile("sv.q C120, 0x20($s0)");
    __asm__ volatile("lv.q C120, 0x30($s2)");
    __asm__ volatile("sv.q C120, 0x30($s0)");
    arg0->unk50 = (u8) var_s2->unk50;
    temp_a0_2 = arg0 + 0x54;
    temp_a1_2 = var_s2 + 0x54;
    arg0->unk54 = (s32) var_s2->unk54;
    temp_a0_2->unk4 = (s32) temp_a1_2->unk4;
    temp_a0_2->unk8 = (s32) temp_a1_2->unk8;
    temp_a2 = temp_a1_2->unkC;
    temp_a3 = temp_a1_2->unk10;
    temp_a0_2->unkC = temp_a2;
    temp_a0_2->unk10 = temp_a3;
    temp_a0_2->unk14 = (s32) temp_a1_2->unk14;
    arg0->unk6C = (s32) var_s2->unk6C;
    arg0->unk70 = (s32) var_s2->unk70;
    arg0->unk74 = (f32) var_s2->unk74;
    temp_f12 = var_s2->unk78;
    arg0->unk78 = temp_f12;
    cArrayBase_cArrayBase_refcArrayBase_mVec3___operator_eq_constcArrayBase_cArrayBase_ref(temp_f12, arg0 + 0x80, var_s2 + 0x80, temp_a2, temp_a3);
    cArrayBase_cArrayBase_refcArrayBase_mPlane___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x84, var_s2 + 0x84);
    cArrayBase_cArrayBase_refcArrayBase_eMeshShapeTriangle___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x88, var_s2 + 0x88);
    cArrayBase_cArrayBase_refcArrayBase_mBox___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x8C, var_s2 + 0x8C);
    cArrayBase_cArrayBase_refcArrayBase_eMeshShapeNode___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x90, var_s2 + 0x90);
    cArrayBase_cArrayBase_refcArrayBase_eMeshShapeTriList___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x94, var_s2 + 0x94);
    arg0->unk98 = (u16) var_s2->unk98;
    __asm__ volatile("lv.q C120, 0xa0($s2)");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    __asm__ volatile("lv.q C120, 0xb0($s2)");
    __asm__ volatile("sv.q C120, 0xb0($s0)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
