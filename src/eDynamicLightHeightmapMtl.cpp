#include "eTextureMap.h"

// ── Forward class declarations ──

class eHeightmapMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

#pragma control sched=1

void eDynamicLightHeightmapMtl::Apply(const eCamera *, const eWorld *) const {
}

// ── Write ──

void eDynamicLightHeightmapMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eHeightmapMtl *)this)->Write(file);
    wb.End();
}

// ── New ──

extern "C" void eDynamicLightHeightmapMtl_eDynamicLightHeightmapMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eDynamicLightHeightmapMtl *eDynamicLightHeightmapMtl::New(cMemPool *pool, cBase *parent) {
    eDynamicLightHeightmapMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eDynamicLightHeightmapMtl *obj = (eDynamicLightHeightmapMtl *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eDynamicLightHeightmapMtl_eDynamicLightHeightmapMtl(obj, parent);
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

struct cHandleVal {
    int mIndex;
};

void eDynamicLightHeightmapMtl::AssignCopy(const cBase *src) {
    eDynamicLightHeightmapMtl &other = *dcast<eDynamicLightHeightmapMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((unsigned char *)&other)[0x5D];
    ((unsigned char *)this)[0x5E] = ((unsigned char *)&other)[0x5E];
    ((signed char *)this)[0x5F] = ((signed char *)&other)[0x5F];
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x64));
    __asm__ volatile("" ::: "memory");
    *(cHandleVal *)((char *)this + 0x68) = *(cHandleVal *)((char *)&other + 0x68);
}
