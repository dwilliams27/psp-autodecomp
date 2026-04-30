#include "eTextureMap.h"

// ── Forward class declarations ──

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eColor {
public:
    unsigned int mColor;
};

class eDrawInfo;

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046C8C;

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);
extern "C" void eTexCoordGen_eTexCoordGen(void *, cBase *);

extern char eReflectionModelMtlclassdesc[];

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

// ── Unapply (existing stub) ──

void eReflectionModelMtl::Unapply(void) const {
}

#pragma control sched=1

// ── GetType ──

const cType *eReflectionModelMtl::GetType(void) const {
    if (D_00046C8C == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x2198BC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C8C = cType::InitializeType(0, 0, 0x11F, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C8C;
}

// ── ApplyDynamic ──

void eReflectionModelMtl::ApplyDynamic(const eDrawInfo &di, const mOCS &ocs, float f, unsigned int u, eColor c) const {
    ((const eModelMtl *)this)->ApplyCommonDynamic(di, ocs, f, u, c);
}

// ── Constructor ──

eReflectionModelMtl::eReflectionModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x04 / 4] = eReflectionModelMtlclassdesc;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x74 / 4] = 1;
    ((int *)this)[0x78 / 4] = 0;
    eTexCoordGen_eTexCoordGen((char *)this + 0x7C, (cBase *)this);
    ((int *)this)[0xF0 / 4] = -1;
    CreateData();
}

// ── PlatformRead ──

void eReflectionModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── Write ──

void eReflectionModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    void *desc = *(void **)((char *)this + 0x80);
    ClassDescEntry *entry = (ClassDescEntry *)((char *)desc + 0x28);
    short off = entry->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg_base = (char *)this + 0x7C;
    void *target = tcg_base + off;
    entry->fn(target, (void *)wb._data[0]);

    wb.Write(((int *)this)[0x74 / 4]);
    wb.Write(((int *)this)[0x78 / 4]);
    wb.Write((unsigned int)((int *)this)[0xF0 / 4]);
    wb.End();
}

// ── New ──

extern "C" void eReflectionModelMtl_eReflectionModelMtl(void *, cBase *);

eReflectionModelMtl *eReflectionModelMtl::New(cMemPool *pool, cBase *parent) {
    eReflectionModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eReflectionModelMtl *obj = (eReflectionModelMtl *)entry->fn(base, 0xF4, 4, 0, 0);
    if (obj != 0) {
        eReflectionModelMtl_eReflectionModelMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── AssignCopy ──

void eReflectionModelMtl::AssignCopy(const cBase *base) {
    eReflectionModelMtl *other = dcast<eReflectionModelMtl>(base);
    ((eMaterial *)this)->operator=(*(const eMaterial *)other);
    *(unsigned char *)((char *)this + 0x5C) =
        *(const unsigned char *)((char *)other + 0x5C);
    *(unsigned char *)((char *)this + 0x5D) =
        *(const unsigned char *)((char *)other + 0x5D);
    *(unsigned char *)((char *)this + 0x5E) =
        *(const unsigned char *)((char *)other + 0x5E);
    *(signed char *)((char *)this + 0x5F) =
        *(const signed char *)((char *)other + 0x5F);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x64));
    int *dstWord = (int *)((char *)this + 0x68);
    int *srcWord = (int *)((char *)other + 0x68);
    int word = *srcWord;
    *dstWord = word;
    dstWord = (int *)((char *)this + 0x6C);
    srcWord = (int *)((char *)other + 0x6C);
    word = *srcWord;
    *dstWord = word;
    *(int *)((char *)this + 0x70) = *(const int *)((char *)other + 0x70);
    *(int *)((char *)this + 0x74) = *(const int *)((char *)other + 0x74);
    *(int *)((char *)this + 0x78) = *(const int *)((char *)other + 0x78);
    *(signed char *)((char *)this + 0x84) =
        *(const signed char *)((char *)other + 0x84);
    *(unsigned char *)((char *)this + 0x85) =
        *(const unsigned char *)((char *)other + 0x85);
    *(unsigned char *)((char *)this + 0x86) =
        *(const unsigned char *)((char *)other + 0x86);
    *(unsigned char *)((char *)this + 0x87) =
        *(const unsigned char *)((char *)other + 0x87);
    *(unsigned char *)((char *)this + 0x88) =
        *(const unsigned char *)((char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x89) =
        *(const unsigned char *)((char *)other + 0x89);
    __asm__ volatile("" ::: "memory");
    int i = 0;
    int *dst = (int *)((char *)this + 0x8C);
    int *src = (int *)((char *)other + 0x8C);
    do {
        int word0 = src[0];
        int word1 = src[1];
        int word2 = src[2];
        dst[0] = word0;
        dst[1] = word1;
        dst[2] = word2;
        int word3 = src[3];
        int word4 = src[4];
        dst[3] = word3;
        dst[4] = word4;
        i += 1;
        dst += 5;
        src += 5;
    } while (i < 5);
    {
        int *dstWord2 = (int *)((char *)this + 0xF0);
        int *srcWord2 = (int *)((char *)other + 0xF0);
        __asm__ volatile("" : "+r"(dstWord2), "+r"(srcWord2));
        int word2 = *srcWord2;
        *dstWord2 = word2;
    }
}
