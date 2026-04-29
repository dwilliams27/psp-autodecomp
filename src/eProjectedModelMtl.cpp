#include "eTextureMap.h"

// ── Forward class declarations ──

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

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

// ── Write ──

void eProjectedModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    wb.End();
}

// ── New ──

extern "C" void eProjectedModelMtl_eProjectedModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eProjectedModelMtl *eProjectedModelMtl::New(cMemPool *pool, cBase *parent) {
    eProjectedModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eProjectedModelMtl *obj = (eProjectedModelMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eProjectedModelMtl_eProjectedModelMtl(obj, parent);
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

void eProjectedModelMtl::AssignCopy(const cBase *src) {
    eProjectedModelMtl &other = *dcast<eProjectedModelMtl>(src);
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
    __asm__ volatile("" ::: "memory");
    *(cHandleVal *)((char *)this + 0x6C) = *(cHandleVal *)((char *)&other + 0x6C);
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x70 / 4] = ((int *)&other)[0x70 / 4];
}

// ── Header extensions ──
// eProjectedModelMtl declares Write/New via out-of-class declarations below
// because eTextureMap.h has only the trivial stubs.
