// src/eWaterFilter.cpp
// Functions:
//   0x0008c7f4  eWaterFilter::~eWaterFilter(void)            eAll_psp.obj  (sched=1 zone)
//   0x0021d250  eWaterFilter::New(cMemPool *, cBase *) static eAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

struct CopyWord {
    int value;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

class eTextureFilter {
public:
    eTextureFilter(cBase *);
    ~eTextureFilter();
    void Write(cFile &) const;
};

class eWaterFilter : public eTextureFilter {
public:
    eWaterFilter(cBase *);
    ~eWaterFilter();
    void AssignCopy(const cBase *);
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

extern char eWaterFiltervirtualtable[];

static cType *type_cBase;
static cType *type_eTextureFilter;
static cType *type_eWaterFilter;

#pragma control sched=1
// ── 0x0008c7f4 — ~eWaterFilter(void) ──
eWaterFilter::~eWaterFilter() {
    *(void **)((char *)this + 4) = eWaterFiltervirtualtable;
}

// ── 0x0021d1d0 — AssignCopy(const cBase *) ──
void eWaterFilter::AssignCopy(const cBase *base) {
    eWaterFilter *other = dcast<eWaterFilter>(base);
    *(CopyWord *)((char *)this + 0x08) = *(const CopyWord *)((const char *)other + 0x08);
    __asm__ volatile("" ::: "memory");
    *(CopyWord *)((char *)this + 0x0C) = *(const CopyWord *)((const char *)other + 0x0C);
    __asm__ volatile("" ::: "memory");
    *(CopyWord *)((char *)this + 0x10) = *(const CopyWord *)((const char *)other + 0x10);
    __asm__ volatile("" ::: "memory");
    *(CopyWord *)((char *)this + 0x14) = *(const CopyWord *)((const char *)other + 0x14);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x18) = *(const float *)((const char *)other + 0x18);
    *(float *)((char *)this + 0x1C) = *(const float *)((const char *)other + 0x1C);
    *(float *)((char *)this + 0x20) = *(const float *)((const char *)other + 0x20);
}

// ── 0x0021d250 — New(cMemPool *, cBase *) static ──
cBase *eWaterFilter::New(cMemPool *pool, cBase *parent) {
    eWaterFilter *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eWaterFilter *obj = (eWaterFilter *)rec->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        new (obj) eWaterFilter(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── 0x0021d2cc — GetType(void) const ──
const cType *eWaterFilter::GetType(void) const {
    if (!type_eWaterFilter) {
        if (!type_eTextureFilter) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_eTextureFilter =
                cType::InitializeType(0, 0, 0x13B, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_eTextureFilter;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x21D250;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eWaterFilter =
            cType::InitializeType(0, 0, 0x249, parentType, factory, 0, 0, 0);
    }
    return type_eWaterFilter;
}
#pragma control sched=2
