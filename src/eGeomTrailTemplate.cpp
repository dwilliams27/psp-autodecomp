// eGeomTrailTemplate.cpp - decompiled from eAll_psp.obj.

#pragma control sched=1

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    unsigned int mKey;
    void Write(cWriteBlock &) const;
};

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

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    cObject(cBase *);
    char _pad[0x44];
    ~cObject(void);
    cObject &operator=(const cObject &);
};

class eDynamicGeomTemplate {
public:
    float mDynamicField44;
    void Write(cFile &) const;
};

class eGeomTrail {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eGeomTrailTemplate : public cObject {
public:
    float mDynamicField44;
    cHandle mHandle;
    int mField4C;
    int mField50;
    int mField54;
    float mField58;
    float mField5C;
    bool mField60;
    char _pad61[3];
    int mField64;

    eGeomTrailTemplate(cBase *);
    ~eGeomTrailTemplate(void);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *target = block + off;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(target, p);
    }
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" {
    void eGeomTrailTemplate__eGeomTrailTemplate_cBaseptr(void *self, cBase *parent);
}

extern char eGeomTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C28;
extern cType *D_00046C2C;

template <class T> T *dcast(const cBase *);

// -- eGeomTrailTemplate::Write(cFile &) const @ 0x00078f4c --
void eGeomTrailTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    mHandle.Write(wb);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField5C);
    wb.Write(mField60);
    wb.Write(mField64);
    wb.End();
}

// -- eGeomTrailTemplate::New(cMemPool *, cBase *) static @ 0x00211224 --
cBase *eGeomTrailTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomTrailTemplate *result = 0;
    __asm__ volatile("" : "+r"(parent), "+r"(result) :: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eGeomTrailTemplate *obj = (eGeomTrailTemplate *)rec->fn(base, 0x68, 4, 0, 0);
    if (obj != 0) {
        eGeomTrailTemplate__eGeomTrailTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- eGeomTrailTemplate::GetInstanceType(void) const @ 0x00079244 --
const cType *eGeomTrailTemplate::GetInstanceType(void) const {
    if (D_00046C2C == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eGeomTrail::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C2C = cType::InitializeType(0, 0, 0x34, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C2C;
}

const cType *eGeomTrailTemplate::GetType(void) const {
    if (D_00046C28 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eGeomTrailTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C28 = cType::InitializeType(0, 0, 0x35, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C28;
}

// -- eGeomTrailTemplate::~eGeomTrailTemplate(void) @ 0x00211438 --
eGeomTrailTemplate::~eGeomTrailTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

eGeomTrailTemplate::eGeomTrailTemplate(cBase *parent) : cObject(parent) {
    *(volatile float *)((char *)this + 0x44) = 1000.0f;
    *(void **)((char *)this + 4) = (void *)0x003845A8;
    *(volatile int *)((char *)this + 0x48) = 0;
    *(volatile int *)((char *)this + 0x4C) = 0;
    *(volatile int *)((char *)this + 0x50) = 8;
    *(volatile int *)((char *)this + 0x54) = 3;
    *(volatile float *)((char *)this + 0x58) = 0.25f;
    *(volatile float *)((char *)this + 0x5C) = 0.8f;
    *(volatile unsigned char *)((char *)this + 0x60) = 1;
    *(volatile int *)((char *)this + 0x64) = 0;
}

// -- eGeomTrailTemplate::AssignCopy(const cBase *) @ 0x0021118c --
void eGeomTrailTemplate::AssignCopy(const cBase *src) {
    eGeomTrailTemplate *other = dcast<eGeomTrailTemplate>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    mDynamicField44 = other->mDynamicField44;
    __asm__ volatile("" ::: "memory");
    cHandle *dstHandle = &mHandle;
    cHandle *srcHandle = &other->mHandle;
    *(int *)dstHandle = *(int *)srcHandle;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mField54 = other->mField54;
    mField58 = other->mField58;
    mField5C = other->mField5C;
    mField60 = other->mField60;
    int *dstField64 = &mField64;
    int *srcField64 = &other->mField64;
    *dstField64 = *srcField64;
}
