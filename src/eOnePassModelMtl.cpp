#include "eTextureMap.h"

// ── Forward class declarations ──

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046C7C;

struct eOnePassModelMtl_Fields {
    char pad[0x74];
    int mField74;
    const cBase *mField78;
};

struct VCallEntry {
    short offset;
    short pad;
    void *fn;
};

struct DestroyEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct CloneEntry {
    short offset;
    short pad;
    void *(*fn)(void *, cMemPool *, void *);
};

struct RefCountedBase {
    void *pad0;
    char *vt;
};

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

#pragma control sched=1

// ── GetType @ 0x0021899c ──

const cType *eOnePassModelMtl::GetType(void) const {
    if (D_00046C7C == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x218920;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C7C = cType::InitializeType(0, 0, 0x1B, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C7C;
}

// ── Constructor @ 0x00081f1c ──

eOnePassModelMtl::eOnePassModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    *(int *)((char *)this + 0x68) = -1;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(void **)((char *)this + 0x04) = (void *)0x385430;
    *(int *)((char *)this + 0x74) = 0;
    *(int *)((char *)this + 0x78) = 0;
    *(int *)((char *)this + 0x7C) = 0;
    CreateData();
}

// ── operator= @ 0x00082154 ──

eOnePassModelMtl &eOnePassModelMtl::operator=(const eOnePassModelMtl &other) {
    int *dstWord;
    const int *srcWord;

    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    *(unsigned char *)((char *)this + 0x5C) =
        *(const unsigned char *)((const char *)&other + 0x5C);
    *(unsigned char *)((char *)this + 0x5D) =
        *(const unsigned char *)((const char *)&other + 0x5D);
    *(unsigned char *)((char *)this + 0x5E) =
        *(const unsigned char *)((const char *)&other + 0x5E);
    *(signed char *)((char *)this + 0x5F) =
        *(const signed char *)((const char *)&other + 0x5F);
    __asm__ volatile("" ::: "memory");

    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x64));

    dstWord = (int *)((char *)this + 0x68);
    srcWord = (const int *)((const char *)&other + 0x68);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    dstWord = (int *)((char *)this + 0x6C);
    srcWord = (const int *)((const char *)&other + 0x6C);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    *(int *)((char *)this + 0x70) = *(const int *)((const char *)&other + 0x70);
    *(int *)((char *)this + 0x74) = *(const int *)((const char *)&other + 0x74);
    __asm__ volatile("" ::: "memory");

    char *live78 = *(char **)((char *)this + 0x78);
    if (!live78) goto skip_destroy78;
    {
        DestroyEntry *entry = (DestroyEntry *)(((RefCountedBase *)live78)->vt + 0x50);
        short offset = entry->offset;
        live78 = live78 + offset;
        __asm__ volatile("" : "+r"(live78), "+r"(entry));
        void (*fn)(void *, int) = entry->fn;
        fn(live78, 3);
        *(void **)((char *)this + 0x78) = 0;
    }
skip_destroy78:

    char *other78 = *(char *const *)((const char *)&other + 0x78);
    if (!other78) goto skip_clone78;
    {
        char *vt = ((RefCountedBase *)other78)->vt;
        CloneEntry *entry = (CloneEntry *)(vt + 0x10);
        other78 = other78 + entry->offset;
        __asm__ volatile("" : "+r"(other78), "+r"(entry));
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        *(void **)((char *)this + 0x78) = entry->fn(other78, pool, this);
    }
skip_clone78:

    char *live7C = *(char **)((char *)this + 0x7C);
    if (!live7C) goto skip_destroy7C;
    {
        DestroyEntry *entry = (DestroyEntry *)(((RefCountedBase *)live7C)->vt + 0x50);
        short offset = entry->offset;
        live7C = live7C + offset;
        __asm__ volatile("" : "+r"(live7C), "+r"(entry));
        void (*fn)(void *, int) = entry->fn;
        fn(live7C, 3);
        *(void **)((char *)this + 0x7C) = 0;
    }
skip_destroy7C:

    char *other7C = *(char *const *)((const char *)&other + 0x7C);
    if (!other7C) goto skip_clone7C;
    {
        char *vt = ((RefCountedBase *)other7C)->vt;
        CloneEntry *entry = (CloneEntry *)(vt + 0x10);
        other7C = other7C + entry->offset;
        __asm__ volatile("" : "+r"(other7C), "+r"(entry));
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        *(void **)((char *)this + 0x7C) = entry->fn(other7C, pool, this);
    }
skip_clone7C:

    return *this;
}

// ── Write @ 0x00081cc4 ──

void eOnePassModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eModelMtl *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x48))->Write(wb);
    const eOnePassModelMtl_Fields *self = (const eOnePassModelMtl_Fields *)this;
    wb.Write(self->mField74);
    wb.WriteBase(self->mField78);
    wb.End();
}

// ── New @ 0x00218920 ──

extern "C" void eOnePassModelMtl_eOnePassModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eOnePassModelMtl *eOnePassModelMtl::New(cMemPool *pool, cBase *parent) {
    eOnePassModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eOnePassModelMtl *obj = (eOnePassModelMtl *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        eOnePassModelMtl_eOnePassModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
