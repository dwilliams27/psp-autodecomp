#include "eStaticLight.h"

class cFile;
class cMemPool;
class cBase;
class cType;

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

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

class cObject {
public:
    cObject &operator=(const cObject &);
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

class eStaticSurfaceLight : public eStaticLight {
public:
    eStaticSurfaceLight(cBase *);
    ~eStaticSurfaceLight();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
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

class eBipedControllerTemplate {
public:
    void AssignCopy(const cBase *);
};

class eDynamicModelLookAtTemplate {
public:
    void AssignCopy(const cBase *);
};

template <class T> T *dcast(const cBase *);

struct WordCopy {
    int value;
};

struct cHandle {
    int mIndex;
};

extern char eStaticSurfaceLightvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B30;
extern cType *D_00046B44;

#pragma control sched=1
// eStaticSurfaceLight::eStaticSurfaceLight(cBase *) — 0x00060080
eStaticSurfaceLight::eStaticSurfaceLight(cBase *parent) : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticSurfaceLightvirtualtable;
    *(float *)((char *)this + 0x90) = 1.0f;
    *(float *)((char *)this + 0x94) = 1.0f;
    __asm__ volatile("" ::: "memory");
}

// eStaticSurfaceLight::Write(cFile &) const — 0x0005ff54
void eStaticSurfaceLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eStaticLight *)this)->Write(file);
    wb.Write(2, (const float *)((const char *)this + 0x90));
    wb.End();
}

// eStaticSurfaceLight::~eStaticSurfaceLight(void) — 0x000600c4
eStaticSurfaceLight::~eStaticSurfaceLight() {
    *(void **)((char *)this + 4) = eStaticSurfaceLightvirtualtable;
}

// eStaticSurfaceLight::AssignCopy(const cBase *) — 0x00206a68
void eStaticSurfaceLight::AssignCopy(const cBase *src) {
    eStaticSurfaceLight *other = dcast<eStaticSurfaceLight>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) =
        *(const float *)((const char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    cHandle *dstHandle = (cHandle *)((char *)this + 0x48);
    const cHandle *srcHandle = (const cHandle *)((const char *)other + 0x48);
    *dstHandle = *srcHandle;
    *(v4sf_t *)((char *)this + 0x80) =
        *(const v4sf_t *)((const char *)other + 0x80);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(v4sf_t *)((char *)this + 0x60) =
        *(const v4sf_t *)((const char *)other + 0x60);
    *(v4sf_t *)((char *)this + 0x70) =
        *(const v4sf_t *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x90) =
        *(const float *)((const char *)other + 0x90);
    *(float *)((char *)this + 0x94) =
        *(const float *)((const char *)other + 0x94);
}

// eStaticSurfaceLight::New(cMemPool *, cBase *) static — 0x00206af0
cBase *eStaticSurfaceLight::New(cMemPool *pool, cBase *parent) {
    eStaticSurfaceLight *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStaticSurfaceLight *obj = (eStaticSurfaceLight *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eStaticSurfaceLight(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eStaticSurfaceLight::GetType(void) const {
    if (D_00046B44 == 0) {
        if (D_00046B30 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *kindName = (const char *)0x36CEE0;
            const char *kindDesc = (const char *)0x36CEEC;
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_00046B30 = cType::InitializeType(0, 0, 0x4A, parentType,
                                               0, kindName, kindDesc, 0);
        }
        const cType *parentType = D_00046B30;
        cBase *(*factory)(cMemPool *, cBase *) = &eStaticSurfaceLight::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B44 = cType::InitializeType(0, 0, 0x4D, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B44;
}

// eBipedControllerTemplate::AssignCopy(const cBase *) — 0x002085a8
void eBipedControllerTemplate::AssignCopy(const cBase *src) {
    eBipedControllerTemplate *other = dcast<eBipedControllerTemplate>(src);
    const cBaseArray *other8 = (const cBaseArray *)((const char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(*other8);
    ((cBaseArray *)((char *)this + 0x10))->operator=(
        *(const cBaseArray *)((const char *)other + 0x10));
    ((cBaseArray *)((char *)this + 0x18))->operator=(
        *(const cBaseArray *)((const char *)other + 0x18));
    __asm__ volatile("" ::: "memory");
    cHandle *dst20 = (cHandle *)((char *)this + 0x20);
    const cHandle *src20 = (const cHandle *)((const char *)other + 0x20);
    *dst20 = *src20;
    __asm__ volatile("" ::: "memory");
    cHandle *dst24 = (cHandle *)((char *)this + 0x24);
    const cHandle *src24 = (const cHandle *)((const char *)other + 0x24);
    *dst24 = *src24;
    *(float *)((char *)this + 0x28) =
        *(const float *)((const char *)other + 0x28);
    *(float *)((char *)this + 0x2C) =
        *(const float *)((const char *)other + 0x2C);
    *(float *)((char *)this + 0x30) =
        *(const float *)((const char *)other + 0x30);
}

// eDynamicModelLookAtTemplate::AssignCopy(const cBase *) — 0x001f0e34
void eDynamicModelLookAtTemplate::AssignCopy(const cBase *src) {
    eDynamicModelLookAtTemplate *other =
        dcast<eDynamicModelLookAtTemplate>(src);
    *(unsigned char *)((char *)this + 8) =
        *(const unsigned char *)((const char *)other + 8);
    *(float *)((char *)this + 0xC) =
        *(const float *)((const char *)other + 0xC);
    *(float *)((char *)this + 0x10) =
        *(const float *)((const char *)other + 0x10);
    *(float *)((char *)this + 0x14) =
        *(const float *)((const char *)other + 0x14);
    *(float *)((char *)this + 0x18) =
        *(const float *)((const char *)other + 0x18);
    *(float *)((char *)this + 0x1C) =
        *(const float *)((const char *)other + 0x1C);
    __asm__ volatile("" ::: "memory");
    int *dst = (int *)((char *)this + 0x20);
    const int *srcWords = (const int *)((const char *)other + 0x20);
    int w0 = srcWords[0];
    int w1 = srcWords[1];
    int w2 = srcWords[2];
    dst[0] = w0;
    dst[1] = w1;
    dst[2] = w2;
    int w3 = srcWords[3];
    int w4 = srcWords[4];
    int w5 = srcWords[5];
    dst[3] = w3;
    dst[4] = w4;
    dst[5] = w5;
}
#pragma control sched=2
