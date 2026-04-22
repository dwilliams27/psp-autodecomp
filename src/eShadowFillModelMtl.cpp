#include "eTextureMap.h"

// ── Forward class declarations ──

class eDrawInfo;

class eModelMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// ── CreateData / PlatformFree (trivial stubs — sched-agnostic) ──

void eShadowFillModelMtl::CreateData(void) {
}

void eShadowFillModelMtl::PlatformFree(void) {
}

#pragma control sched=1

// ── Unapply ──

struct RenderState {
    char pad[0x20];
    int field_20;
};

extern RenderState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eShadowFillModelMtl::Unapply(void) const {
    int f = D_00098428.field_20;
    if (f != 0) {
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = 0x24000000;
        f = 0;
        D_00098428.field_20 = f;
    }
}

// ── Write ──

void eShadowFillModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eModelMtl *)this)->Write(file);
    wb.End();
}

// ── Constructor ──

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

extern char eShadowFillModelMtlclassdesc[];

eShadowFillModelMtl::eShadowFillModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x4 / 4] = eShadowFillModelMtlclassdesc;
    CreateData();
}

// ── New ──

extern "C" void eShadowFillModelMtl_eShadowFillModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eShadowFillModelMtl *eShadowFillModelMtl::New(cMemPool *pool, cBase *parent) {
    eShadowFillModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowFillModelMtl *obj = (eShadowFillModelMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eShadowFillModelMtl_eShadowFillModelMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── AssignCopy ──

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT &operator=(const cHandleT &rhs) { mIndex = rhs.mIndex; return *this; }
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

struct cHandle {
    int mIndex;
};

void eShadowFillModelMtl::AssignCopy(const cBase *src) {
    eShadowFillModelMtl &other = *dcast<eShadowFillModelMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((unsigned char *)&other)[0x5D];
    ((unsigned char *)this)[0x5E] = ((unsigned char *)&other)[0x5E];
    ((signed char *)this)[0x5F] = ((signed char *)&other)[0x5F];
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x64));
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x68) = *(cHandle *)((char *)&other + 0x68);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x6C) = *(cHandle *)((char *)&other + 0x6C);
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x70 / 4] = ((int *)&other)[0x70 / 4];
}
