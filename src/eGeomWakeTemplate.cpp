// eGeomWakeTemplate methods. In sched=1 eAll_psp engine cluster.

#pragma control sched=1

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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

class eGeomWakeTemplate : public cObject {
public:
    eGeomWakeTemplate(cBase *);
    ~eGeomWakeTemplate(void);
    void AssignCopy(const cBase *);
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

extern char eGeomTemplatevirtualtable[];

struct eGeomWakeTemplateWord {
    int value;
};

// ── eGeomWakeTemplate::AssignCopy(const cBase *) @ 0x0021207c ──
#pragma control sched=1
void eGeomWakeTemplate::AssignCopy(const cBase *src) {
    eGeomWakeTemplate *other = dcast<eGeomWakeTemplate *>(src);
    cObject::operator=(*other);
    *(float *)((char *)this + 0x44) = *(float *)((char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 0x48) = *(int *)((char *)other + 0x48);
    eGeomWakeTemplateWord *dst4C = (eGeomWakeTemplateWord *)((char *)this + 0x4C);
    eGeomWakeTemplateWord *src4C = (eGeomWakeTemplateWord *)((char *)other + 0x4C);
    *dst4C = *src4C;
    *(int *)((char *)this + 0x50) = *(int *)((char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(int *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)other + 0x68);
    *(float *)((char *)this + 0x6C) = *(float *)((char *)other + 0x6C);
    __asm__ volatile("" ::: "memory");
    eGeomWakeTemplateWord *dst70 = (eGeomWakeTemplateWord *)((char *)this + 0x70);
    eGeomWakeTemplateWord *src70 = (eGeomWakeTemplateWord *)((char *)other + 0x70);
    *dst70 = *src70;
}

// ── eGeomWakeTemplate::New(cMemPool *, cBase *) static @ 0x0021212c ──
#pragma control sched=1
cBase *eGeomWakeTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomWakeTemplate *result = 0;
    __asm__ volatile("" : "+r"(parent), "+r"(result) :: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eGeomWakeTemplate *obj = (eGeomWakeTemplate *)rec->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        new (obj) eGeomWakeTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eGeomWakeTemplate::~eGeomWakeTemplate(void) @ 0x00212340 ──
eGeomWakeTemplate::~eGeomWakeTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

#pragma control sched=2
