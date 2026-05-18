# Battle Packet: `eBipedController::AssignCopy(const cBase *)`

## Target

- Address: `0x00208b48`
- Size: `660` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 5B; codegen-specific diagnosis; AssignCopy retry family`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
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
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x00208938` `untried` 528B `cArrayBase<cArrayBase> & cArrayBase<eBipedState>::operator=(const cArrayBase<cArrayBase> &)`

## Placement

- Canonical source: `src/eBipedController.cpp`
- Header(s): `include/eBipedController.h`
- Allowed source(s): `src/eBipedController.cpp`
- Split-TU prefix: `src/eBipedController_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eBipedController.h`

```cpp
#ifndef EBIPEDCONTROLLER_H
#define EBIPEDCONTROLLER_H

class mVec3;
class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eContact;
class eContactCollector;

class eBipedController {
public:
    static cBase *New(cMemPool *, cBase *);

    int Read(cFile &, cMemPool *);

    void ApplyImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    int GetCollisionMask(void) const;
    void OnSnappedTo(void);
    void GetVelocity(int, mVec3 *, mVec3 *) const;
    void InvalidateCacheEntries(eGeom *);
    void EnableWallWalk(bool);
    const cType *GetType(void) const;

    void CollectContact(eContactCollector *, eContact *, int);
    void ProcessContact(eContactCollector *, eContact *, int);
    void ProcessGroundOnlyContact(eContactCollector *, eContact *, int);

    static void CollectContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessGroundOnlyContactCallBack(eContactCollector *, void *, eContact *, int);

    char _pad0[0x10];
    int bodyIndex;          // 0x10
    char _pad1[0x1C];       // 0x14-0x2F
    void *bodyEntries;      // 0x30
    char _pad2[0xE0];       // 0x34-0x113
    int collisionMask;      // 0x114
};

#endif
```

## Matched Same-Class Neighbors

- `0x0006487c` 76B `eBipedController::Write(cFile &) const` — `src/eMovie.cpp`
- `0x000648c8` 188B `eBipedController::Read(cFile &, cMemPool *)` — `src/eBipedController.cpp`
- `0x00064b1c` 276B `eBipedController::~eBipedController(void)` — `src/eBipedController.cpp`
- `0x00065ddc` 100B `eBipedController::SetPosition(int, const mVec3 &)` — `src/eBipedController.cpp`
- `0x00065e40` 84B `eBipedController::ApplyImpulse(int, const mVec3 &, const mVec3 &)` — `src/eBipedController.cpp`

## Matched Method Exemplars

### Exemplar 1: `eDynamicFluid::AssignCopy(const cBase *)`

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

### Exemplar 2: `eGeomTrail::AssignCopy(const cBase *)`

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

### Exemplar 3: `eGeomWake::AssignCopy(const cBase *)`

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

### Exemplar 4: `eOnePassModelMtl::AssignCopy(const cBase *)`

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


## Prior Failure Notes

### Note 1

- session `4122d3c6`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/4122d3c6/00208b48__agent_self_reported_failure__src_eBipedController.cpp`

Implemented eBipedController::AssignCopy(const cBase *) in src/eBipedController.cpp, despite the prompt's write-to typo, to satisfy the class-method file rule. The source reconstructs the lazy type initialization, dcast gate, byte-order field copies, VFPU v4sf copies, the cArrayBase<eBipedState> assignment call at 0x30, and the 8-entry 0x30-stride saved-contact copy loop. Barriers and typed v4sf structs brought the body to a very close compare_func result: 5/660 bytes different under the normal file build.

The remaining difference is isolated to the dcast vtable call register allocation: current C emits the slot pointer/function pointer through a0/a1/a2, while the target uses a1/a0/a1 for addiu/lh/lw/jalr. Attempts with register-bound entry variables and a small noreorder inline-asm vtable call either did not affect the register choice or shifted the following branch delay slot/regalloc and made the function worse. A 120s permuter run generated 2328 candidates / 1206 compiled and found no improvement. This is likely another source-leverage limit around SNC register allocation for dynamic-cast dispatch.


## Disassembly

```asm
208b48:	27bdffe0 	addiu	sp,sp,-32
208b4c:	afb00000 	sw	s0,0(sp)
208b50:	afb10004 	sw	s1,4(sp)
208b54:	afb20008 	sw	s2,8(sp)
208b58:	afb3000c 	sw	s3,12(sp)
208b5c:	afb40010 	sw	s4,16(sp)
208b60:	afb50014 	sw	s5,20(sp)
208b64:	afbf0018 	sw	ra,24(sp)
208b68:	34120000 	li	s2,0x0
208b6c:	00808025 	move	s0,a0
208b70:	10a00042 	beqz	a1,0x208c7c
208b74:	00a08825 	move	s1,a1
208b78:	3c130004 	lui	s3,0x4
208b7c:	8e646bb4 	lw	a0,27572(s3)
208b80:	14800027 	bnez	a0,0x208c20
208b84:	3c140004 	lui	s4,0x4
208b88:	8e8469d8 	lw	a0,27096(s4)
208b8c:	14800019 	bnez	a0,0x208bf4
208b90:	3c150004 	lui	s5,0x4
208b94:	8ea485dc 	lw	a0,-31268(s5)
208b98:	1480000c 	bnez	a0,0x208bcc
208b9c:	3c040037 	lui	a0,0x37
208ba0:	2484cd74 	addiu	a0,a0,-12940
208ba4:	3c050037 	lui	a1,0x37
208ba8:	24a5cd7c 	addiu	a1,a1,-12932
208bac:	34060001 	li	a2,0x1
208bb0:	00003825 	move	a3,zero
208bb4:	00004025 	move	t0,zero
208bb8:	00004825 	move	t1,zero
208bbc:	00005025 	move	t2,zero
208bc0:	0c001d4d 	jal	0x7534
208bc4:	00005825 	move	t3,zero
208bc8:	aea285dc 	sw	v0,-31268(s5)
208bcc:	8ea785dc 	lw	a3,-31268(s5)
208bd0:	00002025 	move	a0,zero
208bd4:	00002825 	move	a1,zero
208bd8:	34060232 	li	a2,0x232
208bdc:	00004025 	move	t0,zero
208be0:	00004825 	move	t1,zero
208be4:	00005025 	move	t2,zero
208be8:	0c001d4d 	jal	0x7534
208bec:	00005825 	move	t3,zero
208bf0:	ae8269d8 	sw	v0,27096(s4)
208bf4:	8e8769d8 	lw	a3,27096(s4)
208bf8:	3c080021 	lui	t0,0x21
208bfc:	25088ddc 	addiu	t0,t0,-29220
208c00:	00002025 	move	a0,zero
208c04:	00002825 	move	a1,zero
208c08:	34060238 	li	a2,0x238
208c0c:	00004825 	move	t1,zero
208c10:	00005025 	move	t2,zero
208c14:	0c001d4d 	jal	0x7534
208c18:	00005825 	move	t3,zero
208c1c:	ae626bb4 	sw	v0,27572(s3)
208c20:	8e736bb4 	lw	s3,27572(s3)
208c24:	8e240004 	lw	a0,4(s1)
208c28:	24850008 	addiu	a1,a0,8
208c2c:	84a40000 	lh	a0,0(a1)
208c30:	8ca50004 	lw	a1,4(a1)
208c34:	00a0f809 	jalr	a1
208c38:	02242021 	addu	a0,s1,a0
208c3c:	16600003 	bnez	s3,0x208c4c
208c40:	00402025 	move	a0,v0
208c44:	1000000a 	b	0x208c70
208c48:	34040000 	li	a0,0x0
208c4c:	50800008 	beqzl	a0,0x208c70
208c50:	34040000 	li	a0,0x0
208c54:	54930003 	bnel	a0,s3,0x208c64
208c58:	8c84001c 	lw	a0,28(a0)
208c5c:	10000004 	b	0x208c70
208c60:	34040001 	li	a0,0x1
208c64:	1480fffb 	bnez	a0,0x208c54
208c68:	00000000 	nop
208c6c:	34040000 	li	a0,0x0
208c70:	50800003 	beqzl	a0,0x208c80
208c74:	8e440008 	lw	a0,8(s2)
208c78:	02209025 	move	s2,s1
208c7c:	8e440008 	lw	a0,8(s2)
208c80:	ae040008 	sw	a0,8(s0)
208c84:	8e44000c 	lw	a0,12(s2)
208c88:	ae04000c 	sw	a0,12(s0)
208c8c:	8e440010 	lw	a0,16(s2)
208c90:	ae040010 	sw	a0,16(s0)
208c94:	92440014 	lbu	a0,20(s2)
208c98:	a2040014 	sb	a0,20(s0)
208c9c:	92440015 	lbu	a0,21(s2)
208ca0:	a2040015 	sb	a0,21(s0)
208ca4:	da460020 	lv.q        C120, 0x20($s2)
208ca8:	fa060020 	sv.q        C120, 0x20($s0)
208cac:	26040030 	addiu	a0,s0,48
208cb0:	0c08224e 	jal	0x208938
208cb4:	26450030 	addiu	a1,s2,48
208cb8:	92440034 	lbu	a0,52(s2)
208cbc:	a2040034 	sb	a0,52(s0)
208cc0:	92440035 	lbu	a0,53(s2)
208cc4:	a2040035 	sb	a0,53(s0)
208cc8:	8e440038 	lw	a0,56(s2)
208ccc:	ae040038 	sw	a0,56(s0)
208cd0:	9244003c 	lbu	a0,60(s2)
208cd4:	a204003c 	sb	a0,60(s0)
208cd8:	c64c0040 	lwc1	$f12,64(s2)
208cdc:	e60c0040 	swc1	$f12,64(s0)
208ce0:	8e440044 	lw	a0,68(s2)
208ce4:	ae040044 	sw	a0,68(s0)
208ce8:	8e440048 	lw	a0,72(s2)
208cec:	ae040048 	sw	a0,72(s0)
208cf0:	da460080 	lv.q        C120, 0x80($s2)
208cf4:	fa060080 	sv.q        C120, 0x80($s0)
208cf8:	da460050 	lv.q        C120, 0x50($s2)
208cfc:	fa060050 	sv.q        C120, 0x50($s0)
208d00:	da460060 	lv.q        C120, 0x60($s2)
208d04:	fa060060 	sv.q        C120, 0x60($s0)
208d08:	da460070 	lv.q        C120, 0x70($s2)
208d0c:	fa060070 	sv.q        C120, 0x70($s0)
208d10:	c64c0090 	lwc1	$f12,144(s2)
208d14:	e60c0090 	swc1	$f12,144(s0)
208d18:	da4600a0 	lv.q        C120, 0xA0($s2)
208d1c:	fa0600a0 	sv.q        C120, 0xA0($s0)
208d20:	da4600b0 	lv.q        C120, 0xB0($s2)
208d24:	fa0600b0 	sv.q        C120, 0xB0($s0)
208d28:	da4600c0 	lv.q        C120, 0xC0($s2)
208d2c:	fa0600c0 	sv.q        C120, 0xC0($s0)
208d30:	da4600d0 	lv.q        C120, 0xD0($s2)
208d34:	fa0600d0 	sv.q        C120, 0xD0($s0)
208d38:	924400e0 	lbu	a0,224(s2)
208d3c:	a20400e0 	sb	a0,224(s0)
208d40:	da4600f0 	lv.q        C120, 0xF0($s2)
208d44:	fa0600f0 	sv.q        C120, 0xF0($s0)
208d48:	da460100 	lv.q        C120, 0x100($s2)
208d4c:	fa060100 	sv.q        C120, 0x100($s0)
208d50:	c64c0110 	lwc1	$f12,272(s2)
208d54:	e60c0110 	swc1	$f12,272(s0)
208d58:	8e440114 	lw	a0,276(s2)
208d5c:	ae040114 	sw	a0,276(s0)
208d60:	8e440120 	lw	a0,288(s2)
208d64:	ae040120 	sw	a0,288(s0)
208d68:	34040000 	li	a0,0x0
208d6c:	26050130 	addiu	a1,s0,304
208d70:	26460130 	addiu	a2,s2,304
208d74:	d8c60000 	lv.q        C120, 0x0($a2)
208d78:	f8a60000 	sv.q        C120, 0x0($a1)
208d7c:	d8c60010 	lv.q        C120, 0x10($a2)
208d80:	f8a60010 	sv.q        C120, 0x10($a1)
208d84:	8cc70020 	lw	a3,32(a2)
208d88:	aca70020 	sw	a3,32(a1)
208d8c:	8cc70024 	lw	a3,36(a2)
208d90:	aca70024 	sw	a3,36(a1)
208d94:	24840001 	addiu	a0,a0,1
208d98:	24a50030 	addiu	a1,a1,48
208d9c:	2c870008 	sltiu	a3,a0,8
208da0:	14e0fff4 	bnez	a3,0x208d74
208da4:	24c60030 	addiu	a2,a2,48
208da8:	8e4402b0 	lw	a0,688(s2)
208dac:	ae0402b0 	sw	a0,688(s0)
208db0:	8e4402b4 	lw	a0,692(s2)
208db4:	ae0402b4 	sw	a0,692(s0)
208db8:	8fb00000 	lw	s0,0(sp)
208dbc:	8fb10004 	lw	s1,4(sp)
208dc0:	8fb20008 	lw	s2,8(sp)
208dc4:	8fb3000c 	lw	s3,12(sp)
208dc8:	8fb40010 	lw	s4,16(sp)
208dcc:	8fb50014 	lw	s5,20(sp)
208dd0:	8fbf0018 	lw	ra,24(sp)
208dd4:	03e00008 	jr	ra
208dd8:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eBipedController::AssignCopy(const cBase *)
// Address: 0x00208b48, Size: 660B
// Obj: eAll_psp.obj

? cArrayBase_cArrayBase_refcArrayBase_eBipedState___operator_eq_constcArrayBase_cArrayBase_ref(void *, void *); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

void eBipedController__AssignCopy_constcBaseptr(void *arg0, void *arg1) {
    s32 temp_s3;
    s32 var_a0_2;
    u32 var_a0_3;
    void *(*temp_a1)(void *, ?);
    void *temp_a0;
    void *var_a0;
    void *var_a1;
    void *var_a2;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x46BB4 == 0) {
            if (*(s32 *)0x469D8 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                }
                *(s32 *)0x469D8 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x232, *(s32 *)0x385DC);
            }
            *(s32 *)0x46BB4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x238, *(s32 *)0x469D8);
        }
        temp_s3 = *(s32 *)0x46BB4;
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
    arg0->unk8 = (s32) var_s2->unk8;
    arg0->unkC = (s32) var_s2->unkC;
    arg0->unk10 = (s32) var_s2->unk10;
    arg0->unk14 = (u8) var_s2->unk14;
    arg0->unk15 = (u8) var_s2->unk15;
    __asm__ volatile("lv.q C120, 0x20($s2)");
    __asm__ volatile("sv.q C120, 0x20($s0)");
    cArrayBase_cArrayBase_refcArrayBase_eBipedState___operator_eq_constcArrayBase_cArrayBase_ref(arg0 + 0x30, var_s2 + 0x30);
    arg0->unk34 = (u8) var_s2->unk34;
    arg0->unk35 = (u8) var_s2->unk35;
    arg0->unk38 = (s32) var_s2->unk38;
    arg0->unk3C = (u8) var_s2->unk3C;
    arg0->unk40 = (f32) var_s2->unk40;
    arg0->unk44 = (s32) var_s2->unk44;
    arg0->unk48 = (s32) var_s2->unk48;
    __asm__ volatile("lv.q C120, 0x80($s2)");
    __asm__ volatile("sv.q C120, 0x80($s0)");
    __asm__ volatile("lv.q C120, 0x50($s2)");
    __asm__ volatile("sv.q C120, 0x50($s0)");
    __asm__ volatile("lv.q C120, 0x60($s2)");
    __asm__ volatile("sv.q C120, 0x60($s0)");
    __asm__ volatile("lv.q C120, 0x70($s2)");
    __asm__ volatile("sv.q C120, 0x70($s0)");
    arg0->unk90 = (f32) var_s2->unk90;
    __asm__ volatile("lv.q C120, 0xa0($s2)");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    __asm__ volatile("lv.q C120, 0xb0($s2)");
    __asm__ volatile("sv.q C120, 0xb0($s0)");
    __asm__ volatile("lv.q C120, 0xc0($s2)");
    __asm__ volatile("sv.q C120, 0xc0($s0)");
    __asm__ volatile("lv.q C120, 0xd0($s2)");
    __asm__ volatile("sv.q C120, 0xd0($s0)");
    arg0->unkE0 = (u8) var_s2->unkE0;
    __asm__ volatile("lv.q C120, 0xf0($s2)");
    __asm__ volatile("sv.q C120, 0xf0($s0)");
    __asm__ volatile("lv.q C120, 0x100($s2)");
    __asm__ volatile("sv.q C120, 0x100($s0)");
    arg0->unk110 = (f32) var_s2->unk110;
    arg0->unk114 = (s32) var_s2->unk114;
    arg0->unk120 = (s32) var_s2->unk120;
    var_a0_3 = 0;
    var_a1 = arg0 + 0x130;
    var_a2 = var_s2 + 0x130;
    do {
        __asm__ volatile("lv.q C120, ($a2)");
        __asm__ volatile("sv.q C120, ($a1)");
        __asm__ volatile("lv.q C120, 0x10($a2)");
        __asm__ volatile("sv.q C120, 0x10($a1)");
        var_a1->unk20 = (s32) var_a2->unk20;
        var_a1->unk24 = (s32) var_a2->unk24;
        var_a0_3 += 1;
        var_a1 += 0x30;
        var_a2 += 0x30;
    } while (var_a0_3 < 8U);
    arg0->unk2B0 = (s32) var_s2->unk2B0;
    arg0->unk2B4 = (s32) var_s2->unk2B4;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
