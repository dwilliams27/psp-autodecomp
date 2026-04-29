#include "eStandardHeightmapMtl.h"
#include "eStandardParticleSystemMtl.h"
#include "eStandardWeatherEffectMtl.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_0004104C;
extern cType *D_00046C38;
extern cType *D_00046CAC;
extern cType *D_00046CB0;

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

// ── GetType ──

const cType *eStandardHeightmapMtl::GetType(void) const {
    if (D_00046CAC == 0) {
        if (D_00046C38 == 0) {
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
                D_00040FF8 = cType::InitializeType(0, 0, 0x12, D_00040FEC,
                                                   0, 0, 0, 0);
            }
            D_00046C38 = cType::InitializeType(0, 0, 0x51, D_00040FF8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046C38;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eStandardHeightmapMtl::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CAC = cType::InitializeType(0, 0, 0x52, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046CAC;
}

const cType *eStandardParticleSystemMtl::GetType(void) const {
    if (D_00046CB0 == 0) {
        if (D_0004104C == 0) {
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
                D_00040FF8 = cType::InitializeType(0, 0, 0x12, D_00040FEC,
                                                   0, 0, 0, 0);
            }
            D_0004104C = cType::InitializeType(0, 0, 0x32, D_00040FF8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_0004104C;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eStandardParticleSystemMtl::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CB0 = cType::InitializeType(0, 0, 0x33, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046CB0;
}

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
