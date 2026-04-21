#include "eTextureMap.h"

// ── CreateData ──

void eShadowFillModelMtl::CreateData(void) {
}

// ── Unapply ──

struct ShadowFillState {
    char pad[0x20];
    int shadowFlag;
};

extern ShadowFillState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eShadowFillModelMtl::Unapply(void) const {
    int flag = D_00098428.shadowFlag;
    if (flag != 0) {
        int cmd = 0x24000000;
        __asm__ volatile("" ::: "memory");
        int *p;
        p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = cmd;
        flag = 0;
        D_00098428.shadowFlag = flag;
    }
}

#pragma control sched=1

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eModelMtl {
public:
    void Write(cFile &) const;
};

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

#pragma control sched=2

// ── New ──

inline void *operator new(unsigned int, void *p) { return p; }

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
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eShadowFillModelMtl *obj = (eShadowFillModelMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        new (obj) eShadowFillModelMtl(parent);
        result = obj;
    }
    return result;
}

#pragma control sched=1

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

// ── Other class stubs ──

void eDynamicLightHeightmapMtl::PlatformFree(void) {
}
