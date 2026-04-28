// eGeomCurveTemplate.cpp - decompiled from eAll_psp.obj
// Parent codegen matches the eGeomTemplate/cObject destructor and copy chain.

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
};

template <class T> T dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eGeomCurveTemplate : public cObject {
public:
    eGeomCurveTemplate(cBase *);
    ~eGeomCurveTemplate(void);
    void AssignCopy(const cBase *);
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

extern char eGeomTemplatevirtualtable[];

struct eGeomCurveTemplateWord {
    int value;
};

// eGeomCurveTemplate::AssignCopy(const cBase *) @ 0x00210814
#pragma control sched=1
void eGeomCurveTemplate::AssignCopy(const cBase *base) {
    eGeomCurveTemplate *src = dcast<eGeomCurveTemplate *>(base);
    cObject::operator=(*src);
    *(float *)((char *)this + 0x44) = *(float *)((char *)src + 0x44);
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplateWord *dst48 = (eGeomCurveTemplateWord *)((char *)this + 0x48);
    eGeomCurveTemplateWord *src48 = (eGeomCurveTemplateWord *)((char *)src + 0x48);
    *dst48 = *src48;
    *(int *)((char *)this + 0x4C) = *(int *)((char *)src + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)src + 0x50);
    *(unsigned char *)((char *)this + 0x54) = *(unsigned char *)((char *)src + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)src + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)src + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)src + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)src + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)src + 0x68);
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplateWord *dst6c = (eGeomCurveTemplateWord *)((char *)this + 0x6C);
    eGeomCurveTemplateWord *src6c = (eGeomCurveTemplateWord *)((char *)src + 0x6C);
    *dst6c = *src6c;
    *(unsigned char *)((char *)this + 0x70) = *(unsigned char *)((char *)src + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)src + 0x74);
    *(int *)((char *)this + 0x78) = *(int *)((char *)src + 0x78);
}
#pragma control sched=2

// eGeomCurveTemplate::New(cMemPool *, cBase *) static @ 0x002108d4
#pragma control sched=1
cBase *eGeomCurveTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomCurveTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplate *obj = (eGeomCurveTemplate *)rec->fn(base, 0x7C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eGeomCurveTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// eGeomCurveTemplate::~eGeomCurveTemplate(void) @ 0x00210ae8
#pragma control sched=1
eGeomCurveTemplate::~eGeomCurveTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}
#pragma control sched=2
