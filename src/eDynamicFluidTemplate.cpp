// eDynamicFluidTemplate.cpp — decompiled from eAll_psp.obj
// Parent: cObject.
//
// Functions:
//   0x002051ec  eDynamicFluidTemplate::AssignCopy(const cBase *)        (112B)
//   0x0020525c  eDynamicFluidTemplate::New(cMemPool *, cBase *) static  (124B)

class cBase;
class cFile;
class cMemPool;

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

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
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
