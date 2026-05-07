// eLightEmitModelMtl.cpp — decompiled from eAll_psp.obj.
// Parent: eOnePassModelMtl. Vtable at 0x3854E8.
//
// Functions:
//   0x00082ca8 eLightEmitModelMtl::Write(cFile &) const             100B
//   0x00082e48 eLightEmitModelMtl::~eLightEmitModelMtl(void)        124B (deleting dtor auto-gen)
//   0x00218f80 eLightEmitModelMtl::New(cMemPool *, cBase *) static  124B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class eOnePassModelMtl {
public:
    char _pad[0x80];
    eOnePassModelMtl(cBase *);
    ~eOnePassModelMtl(void);
    eOnePassModelMtl &operator=(const eOnePassModelMtl &);
    void CreateData(void);
    void Write(cFile &) const;
};

class eLightEmitModelMtl : public eOnePassModelMtl {
public:
    float mField80;          // 0x80
    unsigned int mField84;   // 0x84

    eLightEmitModelMtl(cBase *);
    ~eLightEmitModelMtl(void);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eLightEmitModelMtlvirtualtable[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046C7C;
extern cType *D_00046C80;

template <class T>
T dcast(const cBase *);

// ── eLightEmitModelMtl::eLightEmitModelMtl @ 0x00082df8 ──
#pragma control sched=1
eLightEmitModelMtl::eLightEmitModelMtl(cBase *parent)
    : eOnePassModelMtl(parent) {
    *(void **)((char *)this + 4) = eLightEmitModelMtlvirtualtable;
    mField80 = 1.0f;
    __asm__ volatile("" ::: "memory");
    mField84 = 0xFFFFFFFF;
    eOnePassModelMtl::CreateData();
}

// ── eLightEmitModelMtl::Write @ 0x00082ca8 ──
void eLightEmitModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eOnePassModelMtl::Write(file);
    wb.Write(mField80);
    wb.Write(mField84);
    wb.End();
}

// ── eLightEmitModelMtl::~eLightEmitModelMtl @ 0x00082e48 ──
eLightEmitModelMtl::~eLightEmitModelMtl(void) {
    *(void **)((char *)this + 4) = eLightEmitModelMtlvirtualtable;
}

// ── eLightEmitModelMtl::AssignCopy @ 0x00218f28 ──
void eLightEmitModelMtl::AssignCopy(const cBase *base) {
    eLightEmitModelMtl *src = dcast<eLightEmitModelMtl *>(base);
    ((eOnePassModelMtl *)this)->operator=(*(eOnePassModelMtl *)src);
    mField80 = src->mField80;
    __asm__ volatile("" ::: "memory");
    unsigned int *dst84 = &mField84;
    unsigned int *src84 = &src->mField84;
    __asm__ volatile("" : "+r"(dst84), "+r"(src84));
    *dst84 = *src84;
}

// ── eLightEmitModelMtl::New @ 0x00218f80 ──
cBase *eLightEmitModelMtl::New(cMemPool *pool, cBase *parent) {
    eLightEmitModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eLightEmitModelMtl *obj = (eLightEmitModelMtl *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) eLightEmitModelMtl(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eLightEmitModelMtl::GetType @ 0x00218ffc ──
#pragma control sched=1
const cType *eLightEmitModelMtl::GetType(void) const {
    if (D_00046C80 == 0) {
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
        const cType *parentType = D_00046C7C;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x218F80;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C80 = cType::InitializeType(0, 0, 0x53, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C80;
}
#pragma control sched=2
