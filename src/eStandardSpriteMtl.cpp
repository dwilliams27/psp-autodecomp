#include "eStandardSpriteMtl.h"

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
extern cType *D_00041038;
extern cType *D_00041044;

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

class eSpriteMtl {
public:
    eSpriteMtl(cBase *);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void WriteBase(const cBase *);
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

template <class T> T *dcast(const cBase *);

void cFile_SetCurrentPos(void *, unsigned int);

struct WordCopy {
    int value;
};

#pragma control sched=1

const cType *eStandardSpriteMtl::GetType(void) const {
    if (D_00041044 == 0) {
        register cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eStandardSpriteMtl::New;
        if (D_00041038 == 0) {
            if (D_00040FEC == 0) {
                register const char *kindName = (const char *)0x36CDCC;
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        register cBase *(*namedFactory)(cMemPool *, cBase *) =
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        __asm__ volatile(
                            "" : "+r"(parentType), "+r"(namedFactory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, namedFactory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                const char *kindDesc = (const char *)0x36CDD8;
                __asm__ volatile(
                    "" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                D_00040FEC = cType::InitializeType(
                    0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
            }
            D_00041038 = cType::InitializeType(0, 0, 0x3F, D_00040FEC,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00041038;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00041044 = cType::InitializeType(0, 0, 0x42, parentType, factory,
                                           0, 0, 0);
    }
    return D_00041044;
}

#pragma control sched=2

void eStandardSpriteMtl::PlatformFree(void) {
}

void eStandardSpriteMtl::Unapply(void) const {
}

void eStandardSpriteMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const eSpriteMtl *)this)->Write(file);

    ((const cHandle *)((const char *)this + 0x48))->Write(wb);
    wb.Write(*(int *)((const char *)this + 0x64));
    wb.WriteBase(*(const cBase **)((const char *)this + 0x68));
    wb.Write(*(bool *)((const char *)this + 0x6C));
    wb.Write(*(float *)((const char *)this + 0x70));
    wb.Write(*(float *)((const char *)this + 0x74));
    wb.Write(*(float *)((const char *)this + 0x78));
    wb.Write(*(float *)((const char *)this + 0x7C));
    wb.End();
}

void eStandardSpriteMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
}

void eStandardSpriteMtl::AssignCopy(const cBase *src) {
    eStandardSpriteMtl &other = *dcast<eStandardSpriteMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    int v0 = *(int *)((char *)&other + 0x5C);
    *(int *)((char *)this + 0x5C) = v0;
    *(WordCopy *)((char *)this + 0x60) =
        *(WordCopy *)((char *)&other + 0x60);
    *(int *)((char *)this + 0x64) = *(int *)((char *)&other + 0x64);
    *(int *)((char *)this + 0x68) = *(int *)((char *)&other + 0x68);
    *(unsigned char *)((char *)this + 0x6C) =
        *(unsigned char *)((char *)&other + 0x6C);
    *(float *)((char *)this + 0x70) = *(float *)((char *)&other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)&other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)&other + 0x78);
    *(float *)((char *)this + 0x7C) = *(float *)((char *)&other + 0x7C);
}

extern "C" void eSpriteMtl_eSpriteMtl(void *, cBase *);

extern char eStandardSpriteMtlclassdesc[];

eStandardSpriteMtl::eStandardSpriteMtl(cBase *parent) {
    eSpriteMtl_eSpriteMtl(this, parent);
    *(void **)((char *)this + 4) = eStandardSpriteMtlclassdesc;
    *(int *)((char *)this + 0x64) = 0;
    *(int *)((char *)this + 0x68) = 0;
    *(unsigned char *)((char *)this + 0x6C) = 0;
    *(float *)((char *)this + 0x70) = 0.0f;
    *(float *)((char *)this + 0x74) = 0.0f;
    *(float *)((char *)this + 0x78) = 256.0f;
    *(float *)((char *)this + 0x7C) = 256.0f;
    CreateData();
}

extern "C" void eStandardSpriteMtl_eStandardSpriteMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// ── eStandardSpriteMtl::New(cMemPool *, cBase *) static @ 0x001e44c4 ──
eStandardSpriteMtl *eStandardSpriteMtl::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eStandardSpriteMtl *result = 0;
    eStandardSpriteMtl *obj = (eStandardSpriteMtl *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        eStandardSpriteMtl_eStandardSpriteMtl(obj, parent);
        result = obj;
    }
    return result;
}
