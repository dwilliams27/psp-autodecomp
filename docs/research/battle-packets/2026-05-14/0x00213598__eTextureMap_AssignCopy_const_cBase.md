# Battle Packet: `eTextureMap::AssignCopy(const cBase *)`

## Target

- Address: `0x00213598`
- Size: `488` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eTextureMap`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `182`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 5B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- failure_action: `research`
- near_miss_bytes: `5`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `5`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0007b164` `matched` 124B `eTextureMap::operator=(const eTextureMap &)`

## Placement

- Canonical source: `src/eTextureMap.cpp`
- Header(s): `include/eTextureMap.h`
- Allowed source(s): `src/eTextureMap.cpp`
- Split-TU prefix: `src/eTextureMap_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eTextureMap.h`

```cpp
#ifndef ETEXTUREMAP_H
#define ETEXTUREMAP_H

#include "eRenderTarget.h"

class cFilename;

class cMemPool {
public:
    static float GetSizeScale(void);
    static cMemPool *GetPoolFromPtr(const void *);
};

class eTextureMap : public eTexture {
public:
    bool field_4C;
    unsigned char field_4D;
    char _pad_4E[2];
    int field_50;

    int GetNumExternalDependencies(void) const;
    eTextureMap(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    eTextureMap &operator=(const eTextureMap &);
    void CreateData(void);
    void GetExternalDependency(int, cFilename *) const;

    static eTextureMap *New(cMemPool *, cBase *);
};

class eProjectedModelMtl {
public:
    eProjectedModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eProjectedModelMtl *New(cMemPool *, cBase *);
};

class eCamera;
class eWorld;
class mOCS;
class mVec3;
class eTexture;
class cType;
class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cMemPool;

class eShadowFillModelMtl {
public:
    eShadowFillModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    static eShadowFillModelMtl *New(cMemPool *, cBase *);
};

class eBumpOffsetMap : public eTexture {
public:
    bool field_4C;
    char _pad_4D[3];
    float field_50;
    int field_54;

    eBumpOffsetMap(cBase *);
    ~eBumpOffsetMap(void);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void CreateData(void);
    void Apply(int, const eCamera *, const eWorld *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    int GetNumExternalDependencies(void) const;
    void GetExternalDependency(int, cFilename *) const;

    static void operator delete(void *p) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        char *allocTable = ((char **)block)[7];
        DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }

    static eBumpOffsetMap *New(cMemPool *, cBase *);
};

class eDynamicLightHeightmapMtl {
public:
    eDynamicLightHeightmapMtl(cBase *);
    ~eDynamicLightHeightmapMtl(void);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static void operator delete(void *p) __attribute__((always_inline)) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            char *allocTable = ((char **)block)[7];
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
    static eDynamicLightHeightmapMtl *New(cMemPool *, cBase *);
};

class eDynamicLightModelMtl {
public:
    eDynamicLightModelMtl(cBase *);
    ~eDynamicLightModelMtl(void);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static void operator delete(void *p) __attribute__((always_inline)) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            char *allocTable = ((char **)block)[7];
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
    static eDynamicLightModelMtl *New(cMemPool *, cBase *);
};

class eOnePassModelMtl {
public:
    eOnePassModelMtl(cBase *);
    const cType *GetType(void) const;
    eOnePassModelMtl &operator=(const eOnePassModelMtl &);
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void Write(cFile &) const;
    static eOnePassModelMtl *New(cMemPool *, cBase *);
};

class eProjectedDynamicMtl {
public:
    eProjectedDynamicMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eProjectedDynamicMtl *New(cMemPool *, cBase *);
};

class eProjectedHeightmapMtl {
public:
    eProjectedHeightmapMtl(cBase *);
    ~eProjectedHeightmapMtl(void);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Apply(const eCamera *, const eWorld *) const;
    void ApplyDynamic(const eCamera &, const eWorld *, const mOCS &, float, float, const mVec3 &, float, int, const eTexture *, unsigned int) const;
    void Unapply(void) const;
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static void operator delete(void *p) __attribute__((always_inline)) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            char *allocTable = ((char **)block)[7];
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
    static eProjectedHeightmapMtl *New(cMemPool *, cBase *);
};

class eDrawInfo;
class eColor;
class cBase;
class cFile;
class cMemPool;

class eReflectionModelMtl {
public:
    eReflectionModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eReflectionModelMtl *New(cMemPool *, cBase *);
};

class eRefractionModelMtl {
public:
    eRefractionModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void Unapply(void) const;
    void CreateData(void);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    eRefractionModelMtl &operator=(const eRefractionModelMtl &);
    static eRefractionModelMtl *New(cMemPool *, cBase *);
};

class eShadowVolumeModelMtl {
public:
    eShadowVolumeModelMtl(cBase *);
    ~eShadowVolumeModelMtl();
    const cType *GetType(void) const;
    void PlatformFree(void);
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void PlatformRead(cFile &, cMemPool *);
    static eShadowVolumeModelMtl *New(cMemPool *, cBase *);
};

class eExtrudedShadowVolumeModelMtl : public eShadowVolumeModelMtl {
public:
    eExtrudedShadowVolumeModelMtl(cBase *);
    ~eExtrudedShadowVolumeModelMtl();
    static void operator delete(void *p) {
        struct DeleteRecord {
            short offset;
            short pad;
            void (*fn)(void *, void *);
        };
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = ((void **)pool)[9];
        char *allocTable = *(char **)((char *)block + 0x1C);
        DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = (char *)block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
    void CreateData(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eExtrudedShadowVolumeModelMtl *New(cMemPool *, cBase *);
};

class eSilhouetteModelMtl {
public:
    eSilhouetteModelMtl(cBase *);
    const cType *GetType(void) const;
    void PlatformFree(void);
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
    void ApplyDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Unapply(void) const;
    float GetSilhouetteOffset(const eDrawInfo &) const;
    void Write(cFile &) const;
    eSilhouetteModelMtl &operator=(const eSilhouetteModelMtl &);
    static eSilhouetteModelMtl *New(cMemPool *, cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0007ae00` 100B `eTextureMap::Write(cFile &) const` — `src/eTextureMap.cpp`
- `0x0007ae64` 264B `eTextureMap::Read(cFile &, cMemPool *)` — `src/eTextureMap_Read.cpp`
- `0x0007b098` 72B `eTextureMap::eTextureMap(cBase *)` — `src/eTextureMap.cpp`
- `0x0007b164` 124B `eTextureMap::operator=(const eTextureMap &)` — `src/eTextureMap.cpp`
- `0x0007b1e0` 8B `eTextureMap::CreateData(void)` — `src/eTextureMap.cpp`

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

- session `b7ace5a8`; src `src/eTextureMap.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/b7ace5a8/00213598__agent_self_reported_failure__src_eTextureMap.cpp`

Closest attempt is a 5/488-byte mismatch. I replaced the prior loose vtable-slot cast with the matched inline-dcast shape: local cType layout with parent at 0x1C, DispatchEntry at classDesc+8, explicit target != 0 branch, equality-first parent walk, and a no-code dependency on D_00046C3C. This made the lazy type-init ladder and the bnel parent-chain loop match; a 300s permuter pass generated 10104 candidates, compiled 4669, and found no improvement.

The only remaining difference is the virtual GetType dispatch setup. Target emits `lw s3,D_00046C3C; lw a0,4(s1); addiu a1,a0,8; lh a0,0(a1); lw a1,4(a1); jalr a1; addu a0,s1,a0`. Current best emits the same operations but keeps the entry pointer in a0 and uses a1/a2 for offset/function pointer. Register-binding and alternate two-argument fn-pointer call shapes either had no effect or regressed. Root cause appears to be a tiny local register-selection/scheduling divergence in the dispatch setup.

### Note 2

- session `e59aac37`

Best result was an 81/488-byte mismatch with the inlined type-init ladder fully matching and the remaining diff isolated to the dynamic cast/type-walk block before the tail call to eTextureMap::operator=. I tried multiple source shapes for that block: a gcLobbyScoreboardStrings-style goto/fail loop, a do/while equality-break loop, and a raw-pointer version that passed the vtable slot function pointer as an explicit second argument to mimic the jalr register usage. I also reordered locals so D_00046C3C loads before the vtable access.

The persistent mismatch is in two places: SNC still emits the virtual GetType call setup as `lw a0,4(s1); lw s3,...; addiu a0,8; lw a1,4(a0); lh a0,0(a0)` instead of the target's `lw s3,...; lw a0,4(s1); addiu a1,a0,8; lh a0,0(a1); lw a1,4(a1)`, and the parent-chain walk compiles to `beq`/`bnez` instead of the target's `bnel`-based shape. The next retry should focus only on that cast block; the rest of AssignCopy is already aligned.


## Disassembly

```asm
213598:	27bdffd0 	addiu	sp,sp,-48
21359c:	afb00000 	sw	s0,0(sp)
2135a0:	afb10004 	sw	s1,4(sp)
2135a4:	afb20008 	sw	s2,8(sp)
2135a8:	afb3000c 	sw	s3,12(sp)
2135ac:	afb40010 	sw	s4,16(sp)
2135b0:	afb50014 	sw	s5,20(sp)
2135b4:	afb60018 	sw	s6,24(sp)
2135b8:	afb7001c 	sw	s7,28(sp)
2135bc:	afbf0020 	sw	ra,32(sp)
2135c0:	34120000 	li	s2,0x0
2135c4:	00808025 	move	s0,a0
2135c8:	10a0005f 	beqz	a1,0x213748
2135cc:	00a08825 	move	s1,a1
2135d0:	3c130004 	lui	s3,0x4
2135d4:	8e646c3c 	lw	a0,27708(s3)
2135d8:	14800044 	bnez	a0,0x2136ec
2135dc:	3c140004 	lui	s4,0x4
2135e0:	8e840fe8 	lw	a0,4072(s4)
2135e4:	14800036 	bnez	a0,0x2136c0
2135e8:	3c150004 	lui	s5,0x4
2135ec:	8ea485e4 	lw	a0,-31260(s5)
2135f0:	14800027 	bnez	a0,0x213690
2135f4:	3c160004 	lui	s6,0x4
2135f8:	8ec485e0 	lw	a0,-31264(s6)
2135fc:	1480001a 	bnez	a0,0x213668
213600:	3c170004 	lui	s7,0x4
213604:	8ee485dc 	lw	a0,-31268(s7)
213608:	1480000c 	bnez	a0,0x21363c
21360c:	3c040037 	lui	a0,0x37
213610:	2484cd74 	addiu	a0,a0,-12940
213614:	3c050037 	lui	a1,0x37
213618:	24a5cd7c 	addiu	a1,a1,-12932
21361c:	34060001 	li	a2,0x1
213620:	00003825 	move	a3,zero
213624:	00004025 	move	t0,zero
213628:	00004825 	move	t1,zero
21362c:	00005025 	move	t2,zero
213630:	0c001d4d 	jal	0x7534
213634:	00005825 	move	t3,zero
213638:	aee285dc 	sw	v0,-31268(s7)
21363c:	8ee785dc 	lw	a3,-31268(s7)
213640:	3c08001c 	lui	t0,0x1c
213644:	25083c58 	addiu	t0,t0,15448
213648:	00002025 	move	a0,zero
21364c:	00002825 	move	a1,zero
213650:	34060002 	li	a2,0x2
213654:	00004825 	move	t1,zero
213658:	00005025 	move	t2,zero
21365c:	0c001d4d 	jal	0x7534
213660:	00005825 	move	t3,zero
213664:	aec285e0 	sw	v0,-31264(s6)
213668:	8ec785e0 	lw	a3,-31264(s6)
21366c:	00002025 	move	a0,zero
213670:	00002825 	move	a1,zero
213674:	34060003 	li	a2,0x3
213678:	00004025 	move	t0,zero
21367c:	00004825 	move	t1,zero
213680:	00005025 	move	t2,zero
213684:	0c001d4d 	jal	0x7534
213688:	00005825 	move	t3,zero
21368c:	aea285e4 	sw	v0,-31260(s5)
213690:	8ea785e4 	lw	a3,-31260(s5)
213694:	3c090037 	lui	t1,0x37
213698:	2529cda8 	addiu	t1,t1,-12888
21369c:	3c0a0037 	lui	t2,0x37
2136a0:	254acdb4 	addiu	t2,t2,-12876
2136a4:	00002025 	move	a0,zero
2136a8:	00002825 	move	a1,zero
2136ac:	3406000a 	li	a2,0xa
2136b0:	00004025 	move	t0,zero
2136b4:	0c001d4d 	jal	0x7534
2136b8:	340b0005 	li	t3,0x5
2136bc:	ae820fe8 	sw	v0,4072(s4)
2136c0:	8e870fe8 	lw	a3,4072(s4)
2136c4:	3c080021 	lui	t0,0x21
2136c8:	25083780 	addiu	t0,t0,14208
2136cc:	00002025 	move	a0,zero
2136d0:	00002825 	move	a1,zero
2136d4:	3406000c 	li	a2,0xc
2136d8:	00004825 	move	t1,zero
2136dc:	00005025 	move	t2,zero
2136e0:	0c001d4d 	jal	0x7534
2136e4:	00005825 	move	t3,zero
2136e8:	ae626c3c 	sw	v0,27708(s3)
2136ec:	8e736c3c 	lw	s3,27708(s3)
2136f0:	8e240004 	lw	a0,4(s1)
2136f4:	24850008 	addiu	a1,a0,8
2136f8:	84a40000 	lh	a0,0(a1)
2136fc:	8ca50004 	lw	a1,4(a1)
213700:	00a0f809 	jalr	a1
213704:	02242021 	addu	a0,s1,a0
213708:	16600003 	bnez	s3,0x213718
21370c:	00402025 	move	a0,v0
213710:	1000000a 	b	0x21373c
213714:	34040000 	li	a0,0x0
213718:	50800008 	beqzl	a0,0x21373c
21371c:	34040000 	li	a0,0x0
213720:	54930003 	bnel	a0,s3,0x213730
213724:	8c84001c 	lw	a0,28(a0)
213728:	10000004 	b	0x21373c
21372c:	34040001 	li	a0,0x1
213730:	1480fffb 	bnez	a0,0x213720
213734:	00000000 	nop
213738:	34040000 	li	a0,0x0
21373c:	10800003 	beqz	a0,0x21374c
213740:	02002025 	move	a0,s0
213744:	02209025 	move	s2,s1
213748:	02002025 	move	a0,s0
21374c:	0c01ec59 	jal	0x7b164
213750:	02402825 	move	a1,s2
213754:	8fb00000 	lw	s0,0(sp)
213758:	8fb10004 	lw	s1,4(sp)
21375c:	8fb20008 	lw	s2,8(sp)
213760:	8fb3000c 	lw	s3,12(sp)
213764:	8fb40010 	lw	s4,16(sp)
213768:	8fb50014 	lw	s5,20(sp)
21376c:	8fb60018 	lw	s6,24(sp)
213770:	8fb7001c 	lw	s7,28(sp)
213774:	8fbf0020 	lw	ra,32(sp)
213778:	03e00008 	jr	ra
21377c:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: eTextureMap::AssignCopy(const cBase *)
// Address: 0x00213598, Size: 488B
// Obj: eAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? eTextureMap__operator_eq_consteTextureMapref(s32, void *); /* extern */

void eTextureMap__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a1)(void *, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x46C3C == 0) {
            if (*(s32 *)0x40FE8 == 0) {
                if (*(s32 *)0x385E4 == 0) {
                    if (*(s32 *)0x385E0 == 0) {
                        if (*(s32 *)0x385DC == 0) {
                            *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36CD74, 0x36CD7C, 1, 0);
                        }
                        *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
                    }
                    *(s32 *)0x385E4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 3, *(s32 *)0x385E0);
                }
                *(s32 *)0x40FE8 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0xA, *(s32 *)0x385E4);
            }
            *(s32 *)0x46C3C = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0xC, *(s32 *)0x40FE8);
        }
        temp_s3 = *(s32 *)0x46C3C;
        temp_a0 = arg1->unk4;
        temp_a1 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a1(arg1 + temp_a0->unk8, temp_a1);
        if (temp_s3 == 0) {
            goto block_19;
        }
        if (var_a0 != NULL) {
loop_14:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_19;
                }
                goto loop_14;
            }
            var_a0_2 = 1;
        } else {
block_19:
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            var_s2 = arg1;
        }
    }
    eTextureMap__operator_eq_consteTextureMapref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
