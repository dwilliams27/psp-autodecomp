# Battle Packet: `eRoom::AssignCopy(const cBase *)`

## Target

- Address: `0x001e9920`
- Size: `720` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eRoom`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 5B; codegen-specific diagnosis; AssignCopy retry family`
- failure_action: `quarantine`
- near_miss_bytes: `5`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `5`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006e7c` `matched` 228B `cBaseArray::operator=(const cBaseArray &)`
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x00009d70` `matched` 84B `cObject::operator=(const cObject &)`
- `0x001e1ba4` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<cHandleT<eMaterial> >::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e8eb0` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<cHandleT<eRoom> >::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e90b0` `untried` 624B `cArrayBase<cArrayBase> & cArrayBase<eRoomTreeNode>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e9320` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<cHandleT<eSurface> >::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e9520` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<unsigned short>::operator=(const cArrayBase<cArrayBase> &)`
- `0x001e9720` `failed` 512B `cArrayBase<cArrayBase> & cArrayBase<cHandleT<ePortal> >::operator=(const cArrayBase<cArrayBase> &)`

## Placement

- Canonical source: `src/eRoom.cpp`
- Header(s): `include/eRoom.h`
- Allowed source(s): `src/eRoom.cpp`
- Split-TU prefix: `src/eRoom_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eRoom.h`

Not found.

## Matched Same-Class Neighbors

- `0x0003d2e8` 748B `eRoom::Write(cFile &) const` — `src/eRoom.cpp`
- `0x0003d5d4` 228B `eRoom::eRoom(cBase *)` — `src/eRoom.cpp`
- `0x0003ee70` 40B `eRoom::RemoveGeom(eGeom *)` — `src/gcStaticInstanceGroup.cpp`
- `0x0003f474` 40B `eRoom::RemoveVolume(eVolume *)` — `src/eRoom.cpp`
- `0x001e9bf0` 124B `eRoom::New(cMemPool *, cBase *) static` — `src/eRoom.cpp`

## Matched Method Exemplars

### Exemplar 1: `eGeomTrail::AssignCopy(const cBase *)`

- Address: `0x002119f4`
- Size: `704` bytes
- Source: `src/eGeomTrail_AssignCopy.cpp`

```cpp
// -- eGeomTrail::AssignCopy(const cBase *) @ 0x002119f4 --
void eGeomTrail::AssignCopy(const cBase *base) {
    const eGeomTrail *other = 0;

    if (base != 0) {
        if (D_00046C2C == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(
                            name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            const cType *parentType = D_000469C0;
            cBase *(*factory)(cMemPool *, cBase *) = &eGeomTrail::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C2C = cType::InitializeType(
                0, 0, 0x34, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C2C;
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
                type = type->mParent;
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
            other = (const eGeomTrail *)base;
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
    float field5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = field5C;

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((const char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) =
        *(const v4sf_t *)((const char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) =
        *(const v4sf_t *)((const char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) =
        *(const v4sf_t *)((const char *)other + 0xB0);

    *(short *)((char *)this + 0xD0) =
        *(const short *)((const char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((const char *)other + 0xD2);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xD4) =
        *(const float *)((const char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    const int *srcD8 = (const int *)((const char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) =
        *(const int *)((const char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) =
        *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) =
        *(const int *)((const char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) =
        *(const int *)((const char *)other + 0xE8);
    *(int *)((char *)this + 0xF0) =
        *(const int *)((const char *)other + 0xF0);
    *(int *)((char *)this + 0xF4) =
        *(const int *)((const char *)other + 0xF4);

    ((cArrayBase<eGeomTrailParticle> *)((char *)this + 0xF8))->operator=(
        *(const cArrayBase<eGeomTrailParticle> *)((const char *)other + 0xF8));

    int *dstFC = (int *)((char *)this + 0xFC);
    const int *srcFC = (const int *)((const char *)other + 0xFC);
    *dstFC = *srcFC;
}
```

### Exemplar 2: `eGeomWake::AssignCopy(const cBase *)`

- Address: `0x00212e74`
- Size: `704` bytes
- Source: `src/eGeomWake.cpp`

```cpp
// -- eGeomWake::AssignCopy(const cBase *) @ 0x00212e74 --
void eGeomWake::AssignCopy(const cBase *base) {
    const eGeomWake *other = 0;

    if (base != 0) {
        if (D_00046C34 == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(
                            name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            const cType *parentType = D_000469C0;
            cBase *(*factory)(cMemPool *, cBase *) = &eGeomWake::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C34 = cType::InitializeType(
                0, 0, 0x257, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C34;
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
                type = type->mParent;
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
            other = (const eGeomWake *)base;
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
    float field5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = field5C;

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((const char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) =
        *(const v4sf_t *)((const char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) =
        *(const v4sf_t *)((const char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) =
        *(const v4sf_t *)((const char *)other + 0xB0);

    *(short *)((char *)this + 0xD0) =
        *(const short *)((const char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((const char *)other + 0xD2);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xD4) =
        *(const float *)((const char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    const int *srcD8 = (const int *)((const char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) =
        *(const int *)((const char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) =
        *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) =
        *(const int *)((const char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) =
        *(const int *)((const char *)other + 0xE8);
    *(int *)((char *)this + 0xF0) =
        *(const int *)((const char *)other + 0xF0);

    ((cArrayBase<eGeomWakeSegment> *)((char *)this + 0xF4))->operator=(
        *(const cArrayBase<eGeomWakeSegment> *)((const char *)other + 0xF4));

    *(float *)((char *)this + 0xF8) =
        *(const float *)((const char *)other + 0xF8);
    *(v4sf_t *)((char *)this + 0x100) =
        *(const v4sf_t *)((const char *)other + 0x100);
    *(unsigned char *)((char *)this + 0x110) =
        *(const unsigned char *)((const char *)other + 0x110);
}
```

### Exemplar 3: `eParticleSystem::AssignCopy(const cBase *)`

- Address: `0x002149a0`
- Size: `744` bytes
- Source: `src/eParticleSystem.cpp`

```cpp
// ── eParticleSystem::AssignCopy(const cBase *) @ 0x002149a0 ──
void eParticleSystem::AssignCopy(const cBase *base) {
    const eParticleSystem *other = 0;

    if (base != 0) {
        if (D_00046C44 == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(
                            name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            const cType *parentType = D_000469C0;
            cBase *(*factory)(cMemPool *, cBase *) = &eParticleSystem::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C44 = cType::InitializeType(
                0, 0, 0x30, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C44;
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
                type = type->mParent;
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
            other = (const eParticleSystem *)base;
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
    float field5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = field5C;

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((const char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) =
        *(const v4sf_t *)((const char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) =
        *(const v4sf_t *)((const char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) =
        *(const v4sf_t *)((const char *)other + 0xB0);

    *(short *)((char *)this + 0xD0) =
        *(const short *)((const char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((const char *)other + 0xD2);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xD4) =
        *(const float *)((const char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    const int *srcD8 = (const int *)((const char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) =
        *(const int *)((const char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) =
        *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) =
        *(const int *)((const char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) =
        *(const int *)((const char *)other + 0xE8);

    ((cArrayBase<eParticle> *)((char *)this + 0xF0))->operator=(
        *(const cArrayBase<eParticle> *)((const char *)other + 0xF0));

    *(int *)((char *)this + 0x100) =
        *(const int *)((const char *)other + 0x100);
    int *dst104 = (int *)((char *)this + 0x104);
    const int *src104 = (const int *)((const char *)other + 0x104);
    *dst104 = *src104;
    int *dst108 = (int *)((char *)this + 0x108);
    const int *src108 = (const int *)((const char *)other + 0x108);
    *dst108 = *src108;
    int *dst10C = (int *)((char *)this + 0x10C);
    const int *src10C = (const int *)((const char *)other + 0x10C);
    *dst10C = *src10C;
    *(int *)((char *)this + 0x110) =
        *(const int *)((const char *)other + 0x110);

    *(v4sf_t *)((char *)this + 0x120) =
        *(const v4sf_t *)((const char *)other + 0x120);
}
```

### Exemplar 4: `eDynamicFluid::AssignCopy(const cBase *)`

- Address: `0x00205508`
- Size: `668` bytes
- Source: `src/eDynamicFluid_AssignCopy.cpp`

```cpp
void eDynamicFluid::AssignCopy(const cBase *base) {
    const eDynamicFluid *other = 0;

    if (base != 0) {
        if (D_00046B24 == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(
                            name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            const cType *parentType = D_000469C0;
            cBase *(*factory)(cMemPool *, cBase *) = &eDynamicFluid::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046B24 = cType::InitializeType(
                0, 0, 0x259, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046B24;
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
                type = type->mParent;
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
            other = (const eDynamicFluid *)base;
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
    float field5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = field5C;

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((const char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) =
        *(const v4sf_t *)((const char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) =
        *(const v4sf_t *)((const char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) =
        *(const v4sf_t *)((const char *)other + 0xB0);

    *(short *)((char *)this + 0xD0) =
        *(const short *)((const char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((const char *)other + 0xD2);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xD4) =
        *(const float *)((const char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    const int *srcD8 = (const int *)((const char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) =
        *(const int *)((const char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) =
        *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) =
        *(const int *)((const char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) =
        *(const int *)((const char *)other + 0xE8);
    *(int *)((char *)this + 0xF0) =
        *(const int *)((const char *)other + 0xF0);
}
```


## Prior Failure Notes

### Note 1

- session `3060ad5d`; src `src/eRoom.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/3060ad5d/001e9920__agent_self_reported_failure__src_eRoom.cpp`

Implemented eRoom::AssignCopy canonically in src/eRoom.cpp with the same lazy init/dynamic-cast pattern as eAll exemplars, cObject::operator=, native V4SF copies for the VFPU blocks, and the array/base-array assignment calls in target byte order. Size is exact at 720B and all existing eRoom.cpp matched siblings remain matched except the pre-existing eRoom::Free DB size mismatch. The remaining AssignCopy diff is 5/720 bytes: only register encodings in scalar field-copy instructions around offsets +0xCC and +0xEC (target keeps +0xCC in a2 for the following eRoomTreeNode array assignment and uses a1/a0 for the +0xEC copy). Tried pointer-copy reshaping, temp splitting, register-bound locals, minimal inline asm experiments, and a 120s permuter run (4032 generated / 1806 compiled, no improvement; best stayed 17 raw bytes/5 masked bytes). This appears to be local register allocation drift rather than control-flow or scheduling.


## Disassembly

```asm
1e9920:	27bdffe0 	addiu	sp,sp,-32
1e9924:	afb00000 	sw	s0,0(sp)
1e9928:	afb20008 	sw	s2,8(sp)
1e992c:	34120000 	li	s2,0x0
1e9930:	00808025 	move	s0,a0
1e9934:	afb10004 	sw	s1,4(sp)
1e9938:	afb3000c 	sw	s3,12(sp)
1e993c:	afb40010 	sw	s4,16(sp)
1e9940:	afb50014 	sw	s5,20(sp)
1e9944:	afb60018 	sw	s6,24(sp)
1e9948:	afbf001c 	sw	ra,28(sp)
1e994c:	10a00054 	beqz	a1,0x1e9aa0
1e9950:	00a08825 	move	s1,a1
1e9954:	3c130004 	lui	s3,0x4
1e9958:	8e6468d4 	lw	a0,26836(s3)
1e995c:	14800039 	bnez	a0,0x1e9a44
1e9960:	3c140004 	lui	s4,0x4
1e9964:	8e8485e4 	lw	a0,-31260(s4)
1e9968:	1480002a 	bnez	a0,0x1e9a14
1e996c:	3c08001f 	lui	t0,0x1f
1e9970:	3c150004 	lui	s5,0x4
1e9974:	8ea485e0 	lw	a0,-31264(s5)
1e9978:	1480001b 	bnez	a0,0x1e99e8
1e997c:	3c160004 	lui	s6,0x4
1e9980:	8ec485dc 	lw	a0,-31268(s6)
1e9984:	1480000e 	bnez	a0,0x1e99c0
1e9988:	3c08001c 	lui	t0,0x1c
1e998c:	3c040037 	lui	a0,0x37
1e9990:	3c050037 	lui	a1,0x37
1e9994:	34060001 	li	a2,0x1
1e9998:	00003825 	move	a3,zero
1e999c:	00004025 	move	t0,zero
1e99a0:	00004825 	move	t1,zero
1e99a4:	00005025 	move	t2,zero
1e99a8:	00005825 	move	t3,zero
1e99ac:	2484cd74 	addiu	a0,a0,-12940
1e99b0:	0c001d4d 	jal	0x7534
1e99b4:	24a5cd7c 	addiu	a1,a1,-12932
1e99b8:	aec285dc 	sw	v0,-31268(s6)
1e99bc:	3c08001c 	lui	t0,0x1c
1e99c0:	8ec785dc 	lw	a3,-31268(s6)
1e99c4:	00002025 	move	a0,zero
1e99c8:	00002825 	move	a1,zero
1e99cc:	34060002 	li	a2,0x2
1e99d0:	00004825 	move	t1,zero
1e99d4:	00005025 	move	t2,zero
1e99d8:	00005825 	move	t3,zero
1e99dc:	0c001d4d 	jal	0x7534
1e99e0:	25083c58 	addiu	t0,t0,15448
1e99e4:	aea285e0 	sw	v0,-31264(s5)
1e99e8:	8ea785e0 	lw	a3,-31264(s5)
1e99ec:	00002025 	move	a0,zero
1e99f0:	00002825 	move	a1,zero
1e99f4:	34060003 	li	a2,0x3
1e99f8:	00004025 	move	t0,zero
1e99fc:	00004825 	move	t1,zero
1e9a00:	00005025 	move	t2,zero
1e9a04:	0c001d4d 	jal	0x7534
1e9a08:	00005825 	move	t3,zero
1e9a0c:	ae8285e4 	sw	v0,-31260(s4)
1e9a10:	3c08001f 	lui	t0,0x1f
1e9a14:	3c090037 	lui	t1,0x37
1e9a18:	3c0a0037 	lui	t2,0x37
1e9a1c:	8e8785e4 	lw	a3,-31260(s4)
1e9a20:	00002025 	move	a0,zero
1e9a24:	00002825 	move	a1,zero
1e9a28:	3406021c 	li	a2,0x21c
1e9a2c:	340b0003 	li	t3,0x3
1e9a30:	25089bf0 	addiu	t0,t0,-25616
1e9a34:	2529ce04 	addiu	t1,t1,-12796
1e9a38:	0c001d4d 	jal	0x7534
1e9a3c:	254ace0c 	addiu	t2,t2,-12788
1e9a40:	ae6268d4 	sw	v0,26836(s3)
1e9a44:	8e240004 	lw	a0,4(s1)
1e9a48:	8e7368d4 	lw	s3,26836(s3)
1e9a4c:	24840008 	addiu	a0,a0,8
1e9a50:	84850000 	lh	a1,0(a0)
1e9a54:	8c860004 	lw	a2,4(a0)
1e9a58:	00c0f809 	jalr	a2
1e9a5c:	02252021 	addu	a0,s1,a1
1e9a60:	16600003 	bnez	s3,0x1e9a70
1e9a64:	00402025 	move	a0,v0
1e9a68:	1000000a 	b	0x1e9a94
1e9a6c:	34040000 	li	a0,0x0
1e9a70:	50800008 	beqzl	a0,0x1e9a94
1e9a74:	34040000 	li	a0,0x0
1e9a78:	54930003 	bnel	a0,s3,0x1e9a88
1e9a7c:	8c84001c 	lw	a0,28(a0)
1e9a80:	10000004 	b	0x1e9a94
1e9a84:	34040001 	li	a0,0x1
1e9a88:	1480fffb 	bnez	a0,0x1e9a78
1e9a8c:	00000000 	nop
1e9a90:	34040000 	li	a0,0x0
1e9a94:	10800003 	beqz	a0,0x1e9aa4
1e9a98:	02002025 	move	a0,s0
1e9a9c:	02209025 	move	s2,s1
1e9aa0:	02002025 	move	a0,s0
1e9aa4:	0c00275c 	jal	0x9d70
1e9aa8:	02402825 	move	a1,s2
1e9aac:	da460080 	lv.q        C120, 0x80($s2)
1e9ab0:	fa060080 	sv.q        C120, 0x80($s0)
1e9ab4:	da460050 	lv.q        C120, 0x50($s2)
1e9ab8:	fa060050 	sv.q        C120, 0x50($s0)
1e9abc:	da460060 	lv.q        C120, 0x60($s2)
1e9ac0:	fa060060 	sv.q        C120, 0x60($s0)
1e9ac4:	da460070 	lv.q        C120, 0x70($s2)
1e9ac8:	fa060070 	sv.q        C120, 0x70($s0)
1e9acc:	8e440090 	lw	a0,144(s2)
1e9ad0:	ae040090 	sw	a0,144(s0)
1e9ad4:	da4600a0 	lv.q        C120, 0xA0($s2)
1e9ad8:	fa0600a0 	sv.q        C120, 0xA0($s0)
1e9adc:	260400c4 	addiu	a0,s0,196
1e9ae0:	da4600b0 	lv.q        C120, 0xB0($s2)
1e9ae4:	fa0600b0 	sv.q        C120, 0xB0($s0)
1e9ae8:	264500c4 	addiu	a1,s2,196
1e9aec:	8e4600c0 	lw	a2,192(s2)
1e9af0:	0c07a5c8 	jal	0x1e9720
1e9af4:	ae0600c0 	sw	a2,192(s0)
1e9af8:	260400c8 	addiu	a0,s0,200
1e9afc:	0c07a548 	jal	0x1e9520
1e9b00:	264500c8 	addiu	a1,s2,200
1e9b04:	264400cc 	addiu	a0,s2,204
1e9b08:	8c860000 	lw	a2,0(a0)
1e9b0c:	260500cc 	addiu	a1,s0,204
1e9b10:	aca60000 	sw	a2,0(a1)
1e9b14:	260400d0 	addiu	a0,s0,208
1e9b18:	0c07a42c 	jal	0x1e90b0
1e9b1c:	264500d0 	addiu	a1,s2,208
1e9b20:	260400d4 	addiu	a0,s0,212
1e9b24:	0c07a3ac 	jal	0x1e8eb0
1e9b28:	264500d4 	addiu	a1,s2,212
1e9b2c:	260400d8 	addiu	a0,s0,216
1e9b30:	0c001b9f 	jal	0x6e7c
1e9b34:	264500d8 	addiu	a1,s2,216
1e9b38:	260400e0 	addiu	a0,s0,224
1e9b3c:	0c0786e9 	jal	0x1e1ba4
1e9b40:	264500e0 	addiu	a1,s2,224
1e9b44:	260400e4 	addiu	a0,s0,228
1e9b48:	0c07a4c8 	jal	0x1e9320
1e9b4c:	264500e4 	addiu	a1,s2,228
1e9b50:	264400e8 	addiu	a0,s2,232
1e9b54:	8c840000 	lw	a0,0(a0)
1e9b58:	260500e8 	addiu	a1,s0,232
1e9b5c:	264600ec 	addiu	a2,s2,236
1e9b60:	aca40000 	sw	a0,0(a1)
1e9b64:	8cc50000 	lw	a1,0(a2)
1e9b68:	260400ec 	addiu	a0,s0,236
1e9b6c:	ac850000 	sw	a1,0(a0)
1e9b70:	8e4400f0 	lw	a0,240(s2)
1e9b74:	ae0400f0 	sw	a0,240(s0)
1e9b78:	924400f4 	lbu	a0,244(s2)
1e9b7c:	a20400f4 	sb	a0,244(s0)
1e9b80:	8e4400f8 	lw	a0,248(s2)
1e9b84:	ae0400f8 	sw	a0,248(s0)
1e9b88:	8e4400fc 	lw	a0,252(s2)
1e9b8c:	ae0400fc 	sw	a0,252(s0)
1e9b90:	8e440100 	lw	a0,256(s2)
1e9b94:	ae040100 	sw	a0,256(s0)
1e9b98:	8e440104 	lw	a0,260(s2)
1e9b9c:	ae040104 	sw	a0,260(s0)
1e9ba0:	8e440108 	lw	a0,264(s2)
1e9ba4:	ae040108 	sw	a0,264(s0)
1e9ba8:	8e44010c 	lw	a0,268(s2)
1e9bac:	ae04010c 	sw	a0,268(s0)
1e9bb0:	8e440110 	lw	a0,272(s2)
1e9bb4:	ae040110 	sw	a0,272(s0)
1e9bb8:	8e440114 	lw	a0,276(s2)
1e9bbc:	ae040114 	sw	a0,276(s0)
1e9bc0:	8e440118 	lw	a0,280(s2)
1e9bc4:	ae040118 	sw	a0,280(s0)
1e9bc8:	8fb00000 	lw	s0,0(sp)
1e9bcc:	8fb10004 	lw	s1,4(sp)
1e9bd0:	8fb20008 	lw	s2,8(sp)
1e9bd4:	8fb3000c 	lw	s3,12(sp)
1e9bd8:	8fb40010 	lw	s4,16(sp)
1e9bdc:	8fb50014 	lw	s5,20(sp)
1e9be0:	8fb60018 	lw	s6,24(sp)
1e9be4:	8fbf001c 	lw	ra,28(sp)
1e9be8:	03e00008 	jr	ra
1e9bec:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eRoom::AssignCopy(const cBase *)
// Address: 0x001e9920, Size: 720B
// Obj: eAll_psp.obj

? cArrayBase_cArrayBase_refcArrayBase_cHandleT_eMaterial____operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_cHandleT_ePortal____operator_eq_constcArrayBase_cArrayBase_ref(void *, void *, s32); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_cHandleT_eRoom____operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_cHandleT_eSurface____operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_eRoomTreeNode___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *, s32); /* extern */
? cArrayBase_cArrayBase_refcArrayBase_unsignedshort___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
? cBaseArray__operator_eq_constcBaseArrayref(void *, void *); /* extern */
? cObject__operator_eq_constcObjectref(void *, void *); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

void eRoom__AssignCopy_constcBaseptr(void *arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x468D4 == 0) {
            if (*(s32 *)0x385E4 == 0) {
                if (*(s32 *)0x385E0 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                    }
                    *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
                }
                *(s32 *)0x385E4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 3, *(s32 *)0x385E0);
            }
            *(s32 *)0x468D4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x21C, *(s32 *)0x385E4);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x468D4;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a2(arg1 + temp_a1, temp_a1, temp_a2);
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
    cObject__operator_eq_constcObjectref(arg0, var_s2);
    __asm__ volatile("lv.q C120, 0x80($s2)");
    __asm__ volatile("sv.q C120, 0x80($s0)");
    __asm__ volatile("lv.q C120, 0x50($s2)");
    __asm__ volatile("sv.q C120, 0x50($s0)");
    __asm__ volatile("lv.q C120, 0x60($s2)");
    __asm__ volatile("sv.q C120, 0x60($s0)");
    __asm__ volatile("lv.q C120, 0x70($s2)");
    __asm__ volatile("sv.q C120, 0x70($s0)");
    arg0->unk90 = (s32) var_s2->unk90;
    __asm__ volatile("lv.q C120, 0xa0($s2)");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    __asm__ volatile("lv.q C120, 0xb0($s2)");
    __asm__ volatile("sv.q C120, 0xb0($s0)");
    temp_a2_2 = var_s2->unkC0;
    arg0->unkC0 = temp_a2_2;
    cArrayBase_cArrayBase_refcArrayBase_cHandleT_ePortal____operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xC4, var_s2 + 0xC4, temp_a2_2);
    cArrayBase_cArrayBase_refcArrayBase_unsignedshort___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xC8, var_s2 + 0xC8);
    temp_a2_3 = var_s2->unkCC;
    arg0->unkCC = temp_a2_3;
    cArrayBase_cArrayBase_refcArrayBase_eRoomTreeNode___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xD0, var_s2 + 0xD0, temp_a2_3);
    cArrayBase_cArrayBase_refcArrayBase_cHandleT_eRoom____operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xD4, var_s2 + 0xD4);
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0xD8, var_s2 + 0xD8);
    cArrayBase_cArrayBase_refcArrayBase_cHandleT_eMaterial____operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xE0, var_s2 + 0xE0);
    cArrayBase_cArrayBase_refcArrayBase_cHandleT_eSurface____operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0xE4, var_s2 + 0xE4);
    arg0->unkE8 = (s32) var_s2->unkE8;
    arg0->unkEC = (s32) var_s2->unkEC;
    arg0->unkF0 = (s32) var_s2->unkF0;
    arg0->unkF4 = (u8) var_s2->unkF4;
    arg0->unkF8 = (s32) var_s2->unkF8;
    arg0->unkFC = (s32) var_s2->unkFC;
    arg0->unk100 = (s32) var_s2->unk100;
    arg0->unk104 = (s32) var_s2->unk104;
    arg0->unk108 = (s32) var_s2->unk108;
    arg0->unk10C = (s32) var_s2->unk10C;
    arg0->unk110 = (s32) var_s2->unk110;
    arg0->unk114 = (s32) var_s2->unk114;
    arg0->unk118 = (s32) var_s2->unk118;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
