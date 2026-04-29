#include "eStandardGeomTrailMtl.h"
#include "eStandardHeightmapMtl.h"

void eStandardGeomTrailMtl::PlatformFree(void) {
}

void eStandardGeomTrailMtl::Unapply(void) const {
}

void eStandardGeomTrailMtl::CreateData(void) {
}

void eStandardHeightmapMtl::PlatformFree(void) {
}

void eStandardHeightmapMtl::Unapply(void) const {
}

// ── Forward class declarations for Write/New ──

class eGeomTrailMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// ── eStandardGeomTrailMtl::Write(cFile &) const @ 0x00087750 ──
void eStandardGeomTrailMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomTrailMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);

    wb.Write(*(int *)((char *)this + 0x6C));
    wb.WriteBase(*(const cBase **)((char *)this + 0x70));
    wb.End();
}

// ── eStandardGeomTrailMtl::PlatformRead(cFile &, cMemPool *) @ 0x000878f8 ──
void eStandardGeomTrailMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
}

// ── eStandardGeomTrailMtl::New(cMemPool *, cBase *) static @ 0x0021b628 ──

extern "C" void eStandardGeomTrailMtl_eStandardGeomTrailMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eStandardGeomTrailMtl *eStandardGeomTrailMtl::New(cMemPool *pool, cBase *parent) {
    eStandardGeomTrailMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStandardGeomTrailMtl *obj = (eStandardGeomTrailMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eStandardGeomTrailMtl_eStandardGeomTrailMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── eStandardGeomTrailMtl::AssignCopy(const cBase *) @ 0x0021b590 ──

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

void eStandardGeomTrailMtl::AssignCopy(const cBase *src) {
    eStandardGeomTrailMtl &other = *dcast<eStandardGeomTrailMtl>(src);
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
    *(int *)((char *)this + 0x6C) = *(int *)((char *)&other + 0x6C);
    *(const cBase **)((char *)this + 0x70) = *(const cBase **)((char *)&other + 0x70);
}
