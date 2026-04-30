#include "eTextureMap.h"
#include "eTwoPassModelMtl.h"

// ── Forward class declarations ──

class eColor {
public:
    unsigned int mColor;
};

class eDrawInfo;

class eModelMtl {
public:
    void ApplyCommonDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);
extern "C" void eTexCoordGen_eTexCoordGen(void *, cBase *);

extern char eTwoPassModelMtlclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046CB8;

struct ClassDescEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

#pragma control sched=1

const cType *eTwoPassModelMtl::GetType(void) const {
    if (D_00046CB8 == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x21C55C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CB8 = cType::InitializeType(0, 0, 0x49, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046CB8;
}

// ── ApplyDynamic ──

void eTwoPassModelMtl::ApplyDynamic(const eDrawInfo &di, const mOCS &ocs, float f, unsigned int u, eColor c) const {
    ((const eModelMtl *)this)->ApplyCommonDynamic(di, ocs, f, u, c);
}

// ── Constructor ──

eTwoPassModelMtl::eTwoPassModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x04 / 4] = eTwoPassModelMtlclassdesc;
    ((int *)this)[0x74 / 4] = 0;
    ((int *)this)[0x78 / 4] = 0;
    eTexCoordGen_eTexCoordGen((char *)this + 0x7C, (cBase *)this);
    eTexCoordGen_eTexCoordGen((char *)this + 0xF0, (cBase *)this);
    ((int *)this)[0x164 / 4] = -1;
    ((int *)this)[0x168 / 4] = 0;
    CreateData();
}

// ── PlatformRead ──

void eTwoPassModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── Write ──

void eTwoPassModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    void *desc1 = *(void **)((char *)this + 0x80);
    ClassDescEntry *entry1 = (ClassDescEntry *)((char *)desc1 + 0x28);
    short off1 = entry1->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg1_base = (char *)this + 0x7C;
    void *target1 = tcg1_base + off1;
    entry1->fn(target1, (void *)wb._data[0]);

    void *desc2 = *(void **)((char *)this + 0xF4);
    ClassDescEntry *entry2 = (ClassDescEntry *)((char *)desc2 + 0x28);
    short off2 = entry2->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg2_base = (char *)this + 0xF0;
    void *target2 = tcg2_base + off2;
    entry2->fn(target2, (void *)wb._data[0]);

    wb.Write(((int *)this)[0x74 / 4]);
    wb.Write(((int *)this)[0x78 / 4]);
    wb.Write((unsigned int)((int *)this)[0x164 / 4]);
    wb.End();
}

// ── New ──

extern "C" void eTwoPassModelMtl_eTwoPassModelMtl(void *, cBase *);

eTwoPassModelMtl *eTwoPassModelMtl::New(cMemPool *pool, cBase *parent) {
    eTwoPassModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eTwoPassModelMtl *obj = (eTwoPassModelMtl *)entry->fn(base, 0x16C, 4, 0, 0);
    if (obj != 0) {
        eTwoPassModelMtl_eTwoPassModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
