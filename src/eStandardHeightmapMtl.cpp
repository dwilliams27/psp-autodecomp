#include "eStandardHeightmapMtl.h"
#include "eStandardParticleSystemMtl.h"
#include "eStandardWeatherEffectMtl.h"

void eStandardHeightmapMtl::CreateData(void) {
}

void eStandardParticleSystemMtl::PlatformFree(void) {
}

void eStandardParticleSystemMtl::Unapply(void) const {
}

void eStandardParticleSystemMtl::CreateData(void) {
}

void eStandardWeatherEffectMtl::PlatformFree(void) {
}

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

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// ── Write ──

void eStandardHeightmapMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eHeightmapMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);

    wb.End();
}

// ── PlatformRead ──

void eStandardHeightmapMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
}

// ── Constructor ──

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

extern char eStandardHeightmapMtlclassdesc[];

eStandardHeightmapMtl::eStandardHeightmapMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((void **)this)[0x04 / 4] = eStandardHeightmapMtlclassdesc;
    CreateData();
}

// ── New ──

extern "C" void eStandardHeightmapMtl_eStandardHeightmapMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eStandardHeightmapMtl *eStandardHeightmapMtl::New(cMemPool *pool, cBase *parent) {
    eStandardHeightmapMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStandardHeightmapMtl *obj = (eStandardHeightmapMtl *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eStandardHeightmapMtl_eStandardHeightmapMtl(obj, parent);
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

void eStandardHeightmapMtl::AssignCopy(const cBase *src) {
    eStandardHeightmapMtl &other = *dcast<eStandardHeightmapMtl>(src);
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
