# Battle Packet: `eCameraEffectGeom::AssignCopy(const cBase *)`

## Target

- Address: `0x001e2d00`
- Size: `500` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eCameraEffectGeom`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `94`
- priority: `6`
- reason: `has failure_notes; rich notes; near miss 36B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `36`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `36`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`

## Placement

- Canonical source: `src/eCameraEffectGeom.cpp`
- Header(s): `include/eCameraEffectGeom.h`
- Allowed source(s): `src/eCameraEffectGeom.cpp`
- Split-TU prefix: `src/eCameraEffectGeom_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eCameraEffectGeom.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002d148` 76B `eCameraEffectGeom::Write(cFile &) const` — `src/eCameraEffectGeom.cpp`
- `0x0002d194` 188B `eCameraEffectGeom::Read(cFile &, cMemPool *)` — `src/eCameraEffectGeom.cpp`
- `0x0002d250` 8B `eCameraEffectGeom::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const` — `src/eCollision.cpp`
- `0x001e2ef4` 136B `eCameraEffectGeom::New(cMemPool *, cBase *) static` — `src/eCameraEffectGeom.cpp`
- `0x001e2f7c` 220B `eCameraEffectGeom::GetType(void) const` — `src/eCameraEffectGeom.cpp`

## Matched Method Exemplars

### Exemplar 1: `eConfigBase::AssignCopy(const cBase *)`

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

### Exemplar 2: `eTriangleShape::AssignCopy(const cBase *)`

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

### Exemplar 3: `eMultiSphereShape::AssignCopy(const cBase *)`

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

### Exemplar 4: `eDynamicModel::AssignCopy(const cBase *)`

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


## Prior Failure Notes

### Note 1

- session `44109243`

Implemented a full C++ AssignCopy body with explicit inline runtime type initialization/walk instead of an external dcast call, then matched the 500-byte function size and correct branch-likely type-walk shape. Manual attempts covered external dcast, explicit type-dispatch struct, goto-shaped parent walk, do/while-shaped parent walk, sched/register barriers, and different long-lived self/src/other local orderings. A 5-minute permuter run generated 8040 candidates / 3781 compiled and improved the target from 73 bytes different to 41 in its summary; the saved source currently compares at 36/500 bytes different.

The remaining diff is not a bnel compiler-divergence case. The branch-likely type walk is structurally correct, but SNC keeps allocating destination/source long-lived values differently from the original around the prologue and copy body. The next attempt should focus on forcing the original s0=this, s1=source, s2=checked-source allocation without perturbing the type initialization schedule.


## Disassembly

```asm
1e2d00:	27bdffe0 	addiu	sp,sp,-32
1e2d04:	afb00000 	sw	s0,0(sp)
1e2d08:	afb20008 	sw	s2,8(sp)
1e2d0c:	34120000 	li	s2,0x0
1e2d10:	00808025 	move	s0,a0
1e2d14:	afb10004 	sw	s1,4(sp)
1e2d18:	afb3000c 	sw	s3,12(sp)
1e2d1c:	afb40010 	sw	s4,16(sp)
1e2d20:	afb50014 	sw	s5,20(sp)
1e2d24:	afbf0018 	sw	ra,24(sp)
1e2d28:	10a00043 	beqz	a1,0x1e2e38
1e2d2c:	00a08825 	move	s1,a1
1e2d30:	3c130004 	lui	s3,0x4
1e2d34:	8e641004 	lw	a0,4100(s3)
1e2d38:	14800028 	bnez	a0,0x1e2ddc
1e2d3c:	3c140004 	lui	s4,0x4
1e2d40:	8e840ff4 	lw	a0,4084(s4)
1e2d44:	1480001b 	bnez	a0,0x1e2db4
1e2d48:	3c08001e 	lui	t0,0x1e
1e2d4c:	3c150004 	lui	s5,0x4
1e2d50:	8ea485dc 	lw	a0,-31268(s5)
1e2d54:	1480000c 	bnez	a0,0x1e2d88
1e2d58:	3c040037 	lui	a0,0x37
1e2d5c:	3c050037 	lui	a1,0x37
1e2d60:	34060001 	li	a2,0x1
1e2d64:	00003825 	move	a3,zero
1e2d68:	00004025 	move	t0,zero
1e2d6c:	00004825 	move	t1,zero
1e2d70:	00005025 	move	t2,zero
1e2d74:	00005825 	move	t3,zero
1e2d78:	2484cd74 	addiu	a0,a0,-12940
1e2d7c:	0c001d4d 	jal	0x7534
1e2d80:	24a5cd7c 	addiu	a1,a1,-12932
1e2d84:	aea285dc 	sw	v0,-31268(s5)
1e2d88:	8ea785dc 	lw	a3,-31268(s5)
1e2d8c:	00002025 	move	a0,zero
1e2d90:	00002825 	move	a1,zero
1e2d94:	34060016 	li	a2,0x16
1e2d98:	00004025 	move	t0,zero
1e2d9c:	00004825 	move	t1,zero
1e2da0:	00005025 	move	t2,zero
1e2da4:	0c001d4d 	jal	0x7534
1e2da8:	00005825 	move	t3,zero
1e2dac:	ae820ff4 	sw	v0,4084(s4)
1e2db0:	3c08001e 	lui	t0,0x1e
1e2db4:	8e870ff4 	lw	a3,4084(s4)
1e2db8:	00002025 	move	a0,zero
1e2dbc:	00002825 	move	a1,zero
1e2dc0:	340601b9 	li	a2,0x1b9
1e2dc4:	00004825 	move	t1,zero
1e2dc8:	00005025 	move	t2,zero
1e2dcc:	00005825 	move	t3,zero
1e2dd0:	0c001d4d 	jal	0x7534
1e2dd4:	25082ef4 	addiu	t0,t0,12020
1e2dd8:	ae621004 	sw	v0,4100(s3)
1e2ddc:	8e240004 	lw	a0,4(s1)
1e2de0:	8e731004 	lw	s3,4100(s3)
1e2de4:	24840008 	addiu	a0,a0,8
1e2de8:	84850000 	lh	a1,0(a0)
1e2dec:	8c860004 	lw	a2,4(a0)
1e2df0:	00c0f809 	jalr	a2
1e2df4:	02252021 	addu	a0,s1,a1
1e2df8:	16600003 	bnez	s3,0x1e2e08
1e2dfc:	00402025 	move	a0,v0
1e2e00:	1000000a 	b	0x1e2e2c
1e2e04:	34040000 	li	a0,0x0
1e2e08:	50800008 	beqzl	a0,0x1e2e2c
1e2e0c:	34040000 	li	a0,0x0
1e2e10:	54930003 	bnel	a0,s3,0x1e2e20
1e2e14:	8c84001c 	lw	a0,28(a0)
1e2e18:	10000004 	b	0x1e2e2c
1e2e1c:	34040001 	li	a0,0x1
1e2e20:	1480fffb 	bnez	a0,0x1e2e10
1e2e24:	00000000 	nop
1e2e28:	34040000 	li	a0,0x0
1e2e2c:	10800002 	beqz	a0,0x1e2e38
1e2e30:	00000000 	nop
1e2e34:	02209025 	move	s2,s1
1e2e38:	da460040 	lv.q        C120, 0x40($s2)
1e2e3c:	fa060040 	sv.q        C120, 0x40($s0)
1e2e40:	da460010 	lv.q        C120, 0x10($s2)
1e2e44:	fa060010 	sv.q        C120, 0x10($s0)
1e2e48:	da460020 	lv.q        C120, 0x20($s2)
1e2e4c:	fa060020 	sv.q        C120, 0x20($s0)
1e2e50:	da460030 	lv.q        C120, 0x30($s2)
1e2e54:	fa060030 	sv.q        C120, 0x30($s0)
1e2e58:	c64c005c 	lwc1	$f12,92(s2)
1e2e5c:	da460050 	lv.q        C120, 0x50($s2)
1e2e60:	fa060050 	sv.q        C120, 0x50($s0)
1e2e64:	e60c005c 	swc1	$f12,92(s0)
1e2e68:	8e440060 	lw	a0,96(s2)
1e2e6c:	ae040060 	sw	a0,96(s0)
1e2e70:	8e440064 	lw	a0,100(s2)
1e2e74:	ae040064 	sw	a0,100(s0)
1e2e78:	8e440068 	lw	a0,104(s2)
1e2e7c:	ae040068 	sw	a0,104(s0)
1e2e80:	8e44006c 	lw	a0,108(s2)
1e2e84:	ae04006c 	sw	a0,108(s0)
1e2e88:	8e440070 	lw	a0,112(s2)
1e2e8c:	ae040070 	sw	a0,112(s0)
1e2e90:	c64c0074 	lwc1	$f12,116(s2)
1e2e94:	e60c0074 	swc1	$f12,116(s0)
1e2e98:	c64c0078 	lwc1	$f12,120(s2)
1e2e9c:	e60c0078 	swc1	$f12,120(s0)
1e2ea0:	8e44007c 	lw	a0,124(s2)
1e2ea4:	ae04007c 	sw	a0,124(s0)
1e2ea8:	8e440080 	lw	a0,128(s2)
1e2eac:	ae040080 	sw	a0,128(s0)
1e2eb0:	8e440084 	lw	a0,132(s2)
1e2eb4:	ae040084 	sw	a0,132(s0)
1e2eb8:	8e440088 	lw	a0,136(s2)
1e2ebc:	ae040088 	sw	a0,136(s0)
1e2ec0:	9244008c 	lbu	a0,140(s2)
1e2ec4:	a204008c 	sb	a0,140(s0)
1e2ec8:	9244008d 	lbu	a0,141(s2)
1e2ecc:	a204008d 	sb	a0,141(s0)
1e2ed0:	8fb00000 	lw	s0,0(sp)
1e2ed4:	8fb10004 	lw	s1,4(sp)
1e2ed8:	8fb20008 	lw	s2,8(sp)
1e2edc:	8fb3000c 	lw	s3,12(sp)
1e2ee0:	8fb40010 	lw	s4,16(sp)
1e2ee4:	8fb50014 	lw	s5,20(sp)
1e2ee8:	8fbf0018 	lw	ra,24(sp)
1e2eec:	03e00008 	jr	ra
1e2ef0:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eCameraEffectGeom::AssignCopy(const cBase *)
// Address: 0x001e2d00, Size: 500B
// Obj: eAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

void eCameraEffectGeom__AssignCopy_constcBaseptr(void *arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x41004 == 0) {
            if (*(s32 *)0x40FF4 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                }
                *(s32 *)0x40FF4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x16, *(s32 *)0x385DC);
            }
            *(s32 *)0x41004 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x1B9, *(s32 *)0x40FF4);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x41004;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a2(arg1 + temp_a1, temp_a1, temp_a2);
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
    __asm__ volatile("lv.q C120, 0x50($s2)");
    __asm__ volatile("sv.q C120, 0x50($s0)");
    arg0->unk5C = (f32) var_s2->unk5C;
    arg0->unk60 = (s32) var_s2->unk60;
    arg0->unk64 = (s32) var_s2->unk64;
    arg0->unk68 = (s32) var_s2->unk68;
    arg0->unk6C = (s32) var_s2->unk6C;
    arg0->unk70 = (s32) var_s2->unk70;
    arg0->unk74 = (f32) var_s2->unk74;
    arg0->unk78 = (f32) var_s2->unk78;
    arg0->unk7C = (s32) var_s2->unk7C;
    arg0->unk80 = (s32) var_s2->unk80;
    arg0->unk84 = (s32) var_s2->unk84;
    arg0->unk88 = (s32) var_s2->unk88;
    arg0->unk8C = (u8) var_s2->unk8C;
    arg0->unk8D = (u8) var_s2->unk8D;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
