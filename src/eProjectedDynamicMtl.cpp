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
extern cType *D_00046C84;

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

// ── GetType @ 0x00219314 ──

const cType *eProjectedDynamicMtl::GetType(void) const {
    if (D_00046C84 == 0) {
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
        const cType *parentType = D_00040FF8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eProjectedDynamicMtl::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C84 = cType::InitializeType(0, 0, 0x24C, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C84;
}

// ── eProjectedDynamicMtl::Write @ 0x00082ec4 ──

void eProjectedDynamicMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomMtl *)this)->Write(file);
    ((const cHandle *)((char *)this + 0x48))->Write(wb);
    wb.Write(*(unsigned int *)((char *)this + 0x68));
    wb.End();
}

// ── eProjectedDynamicMtl::PlatformRead @ 0x0008305c ──

void eProjectedDynamicMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── eProjectedModelMtl::PlatformRead @ 0x0007de54 ──

void eProjectedModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── eSilhouetteModelMtl::PlatformRead @ 0x00086f34 ──

void eSilhouetteModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
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
