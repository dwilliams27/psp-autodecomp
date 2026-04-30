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
extern cType *D_00046B28;
extern cType *D_00046C98;

class eModelMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct eShadowVolumeModelMtl_Fields {
    char pad[0x74];
    bool mField74;
    bool mField75;
};

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);
extern char eShadowVolumeModelMtlvirtualtable[];

// ── PlatformFree (trivial stub — sched-agnostic) ──

void eShadowVolumeModelMtl::PlatformFree(void) {
}

#pragma control sched=1

// ── GetType ──

const cType *eShadowVolumeModelMtl::GetType(void) const {
    if (D_00046C98 == 0) {
        if (D_00046B28 == 0) {
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
            D_00046B28 = cType::InitializeType(0, 0, 0x13, D_00040FF8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046B28;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x21A8C4;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C98 = cType::InitializeType(0, 0, 0x213, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C98;
}

// ── Constructor @ 0x0008627c ──

eShadowVolumeModelMtl::eShadowVolumeModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    *(void **)((char *)this + 4) = eShadowVolumeModelMtlvirtualtable;
    register unsigned int enabled __asm__("$4");
    __asm__ volatile("ori %0, $0, 1" : "=r"(enabled));
    ((unsigned char *)this)[0x74] = enabled;
    ((unsigned char *)this)[0x75] = enabled;
    CreateData();
    ((unsigned char *)this)[0x5C] = 0x4A;
    signed char mode = 5;
    __asm__ volatile("" : "+r"(mode));
    if (((unsigned char *)this)[0x75] != 0) {
        mode = 4;
    }
    ((signed char *)this)[0x5F] = mode;
}

// ── Write @ 0x00086010 ──

void eShadowVolumeModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);
    const eShadowVolumeModelMtl_Fields *self = (const eShadowVolumeModelMtl_Fields *)this;
    wb.Write(self->mField74);
    wb.Write(self->mField75);
    wb.End();
}

// ── PlatformRead @ 0x000861cc ──

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

void eShadowVolumeModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── New @ 0x0021a8c4 ──

extern "C" void eShadowVolumeModelMtl_eShadowVolumeModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eShadowVolumeModelMtl *eShadowVolumeModelMtl::New(cMemPool *pool, cBase *parent) {
    eShadowVolumeModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowVolumeModelMtl *obj = (eShadowVolumeModelMtl *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        eShadowVolumeModelMtl_eShadowVolumeModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
