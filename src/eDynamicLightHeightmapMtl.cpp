#include "eTextureMap.h"

// ── Forward class declarations ──

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
extern cType *D_00046C38;
extern cType *D_00046C6C;

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

// ── GetType ──

const cType *eDynamicLightHeightmapMtl::GetType(void) const {
    if (D_00046C6C == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x2179D0;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C6C = cType::InitializeType(0, 0, 9, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C6C;
}

void eDynamicLightHeightmapMtl::Apply(const eCamera *, const eWorld *) const {
}

// ── Write ──

void eDynamicLightHeightmapMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eHeightmapMtl *)this)->Write(file);
    wb.End();
}

// ── Constructor ──

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

eDynamicLightHeightmapMtl::eDynamicLightHeightmapMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    register int value __asm__("a0");
    value = -1;
    ((int *)this)[0x68 / 4] = value;
    value = 0x385160;
    ((void **)this)[0x04 / 4] = (void *)value;
    __asm__ volatile("" ::: "memory");
    value = 2;
    ((int *)this)[0x44 / 4] = value;
    __asm__ volatile("" ::: "memory");
    value = ((unsigned char *)this)[0x5C];
    value |= 8;
    ((unsigned char *)this)[0x5C] = (unsigned char)value;
    CreateData();
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
