// eDynamicFluidTemplate.cpp — decompiled from eAll_psp.obj
// Parent: cObject.
//
// Functions:
//   0x002051ec  eDynamicFluidTemplate::AssignCopy(const cBase *)        (112B)
//   0x0020525c  eDynamicFluidTemplate::New(cMemPool *, cBase *) static  (124B)

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

class eDynamicFluid {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class eDynamicFluidTemplate : public cObject {
public:
    eDynamicFluidTemplate(cBase *);
    ~eDynamicFluidTemplate();
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void *target = block + off;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(target, p);
    }
};

extern "C" {
    void eDynamicFluidTemplate__eDynamicFluidTemplate_cBaseptr(void *self, cBase *parent);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046B20;
extern cType *D_00046B24;

// ── eDynamicFluidTemplate::AssignCopy(const cBase *) @ 0x002051ec ──
#pragma control sched=1
void eDynamicFluidTemplate::AssignCopy(const cBase *base) {
    eDynamicFluidTemplate *src = dcast<eDynamicFluidTemplate>(base);
    cObject::operator=(*src);
    *(float *)((char *)this + 0x44) = *(float *)((char *)src + 0x44);
    *(float *)((char *)this + 0x48) = *(float *)((char *)src + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)src + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)src + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)src + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)src + 0x58);
}
#pragma control sched=2

// ── eDynamicFluidTemplate::New(cMemPool *, cBase *) static @ 0x0020525c ──
#pragma control sched=1
cBase *eDynamicFluidTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicFluidTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicFluidTemplate *obj = (eDynamicFluidTemplate *)rec->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        eDynamicFluidTemplate__eDynamicFluidTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── eDynamicFluidTemplate::GetType(void) const @ 0x002052d8 ──
#pragma control sched=1
const cType *eDynamicFluidTemplate::GetType(void) const {
    if (D_00046B20 == 0) {
        if (D_000469E0 == 0) {
            if (D_000469A8 == 0) {
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
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CE2C;
                const char *kindDesc = (const char *)0x36CE3C;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, parentType, 0, kindName, kindDesc, 5);
            }
            D_000469E0 = cType::InitializeType(0, 0, 0x22, D_000469A8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469E0;
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicFluidTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B20 = cType::InitializeType(
            0, 0, 0x258, parentType, factory, 0, 0, 0);
    }
    return D_00046B20;
}
#pragma control sched=2

// ── eDynamicFluidTemplate::~eDynamicFluidTemplate(void) @ 0x00205470 ──
#pragma control sched=1
eDynamicFluidTemplate::~eDynamicFluidTemplate() {
    *(void **)((char *)this + 4) = (void *)0x00380C18;
}
#pragma control sched=2

// ── eDynamicFluidTemplate::Write(cFile &) const @ 0x0005d954 ──
#pragma control sched=1
void eDynamicFluidTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    wb.Write(*(const float *)((const char *)this + 0x48));
    wb.Write(*(const float *)((const char *)this + 0x50));
    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.Write(*(const float *)((const char *)this + 0x54));
    wb.Write(*(const float *)((const char *)this + 0x58));
    wb.End();
}
#pragma control sched=2

// ── eDynamicFluidTemplate::eDynamicFluidTemplate(cBase *) @ 0x0005dafc ──
#pragma control sched=1
eDynamicFluidTemplate::eDynamicFluidTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x003822D0;
    *(float *)((char *)this + 0x48) = 4.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x4C) = 3.0f;
    *(float *)((char *)this + 0x50) = 4.0f;
    *(float *)((char *)this + 0x54) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x58) = 0.0f;
    *(float *)((char *)this + 0x44) = 50.0f;
}
#pragma control sched=2

// ── eDynamicFluidTemplate::GetInstanceType(void) const @ 0x0005db6c ──
#pragma control sched=1
const cType *eDynamicFluidTemplate::GetInstanceType(void) const {
    if (D_00046B24 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicFluid::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B24 = cType::InitializeType(0, 0, 0x259, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B24;
}
#pragma control sched=2
