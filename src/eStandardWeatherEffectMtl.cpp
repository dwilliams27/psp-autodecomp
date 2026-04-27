#include "eStandardWeatherEffectMtl.h"
#include "eTwoPassModelMtl.h"

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eWeatherEffectMtl {
public:
    void Write(cFile &) const;
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

void eStandardWeatherEffectMtl::Unapply(void) const {
}

void eStandardWeatherEffectMtl::CreateData(void) {
}

void eTwoPassModelMtl::PlatformFree(void) {
}

void eTwoPassModelMtl::Unapply(void) const {
}

void eTwoPassModelMtl::CreateData(void) {
}

// ── eStandardWeatherEffectMtl::Write @ 0x0008a260 ──
void eStandardWeatherEffectMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eWeatherEffectMtl *)this)->Write(file);
    ((const cHandle *)((char *)this + 0x48))->Write(wb);
    wb.End();
}

// ── eStandardWeatherEffectMtl::New @ 0x0021c0b4 ──
extern "C" void eStandardWeatherEffectMtl_eStandardWeatherEffectMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eStandardWeatherEffectMtl *eStandardWeatherEffectMtl::New(cMemPool *pool, cBase *parent) {
    eStandardWeatherEffectMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStandardWeatherEffectMtl *obj = (eStandardWeatherEffectMtl *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eStandardWeatherEffectMtl_eStandardWeatherEffectMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── eStandardWeatherEffectMtl::AssignCopy @ 0x0021c02c ──
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

void eStandardWeatherEffectMtl::AssignCopy(const cBase *src) {
    eStandardWeatherEffectMtl &other = *dcast<eStandardWeatherEffectMtl>(src);
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
