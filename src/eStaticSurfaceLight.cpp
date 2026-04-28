#include "eStaticLight.h"

class cFile;
class cMemPool;

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
    void Write(cFile &) const;
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

extern char eStaticSurfaceLightvirtualtable[];

#pragma control sched=1
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
#pragma control sched=2
