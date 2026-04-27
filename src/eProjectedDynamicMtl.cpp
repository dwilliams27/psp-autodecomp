#include "eTextureMap.h"

// ── Forward class declarations ──

class eGeomMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

// ── eProjectedDynamicMtl::Write @ 0x00082ec4 ──

void eProjectedDynamicMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomMtl *)this)->Write(file);
    ((const cHandle *)((char *)this + 0x48))->Write(wb);
    wb.Write(*(unsigned int *)((char *)this + 0x68));
    wb.End();
}

// ── eProjectedDynamicMtl::New @ 0x00219298 ──

extern "C" void eProjectedDynamicMtl_eProjectedDynamicMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eProjectedDynamicMtl *eProjectedDynamicMtl::New(cMemPool *pool, cBase *parent) {
    eProjectedDynamicMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eProjectedDynamicMtl *obj = (eProjectedDynamicMtl *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eProjectedDynamicMtl_eProjectedDynamicMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── eProjectedDynamicMtl::AssignCopy @ 0x00219210 ──

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

void eProjectedDynamicMtl::AssignCopy(const cBase *src) {
    eProjectedDynamicMtl &other = *dcast<eProjectedDynamicMtl>(src);
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
