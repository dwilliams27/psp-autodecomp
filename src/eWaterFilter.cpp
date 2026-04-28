// src/eWaterFilter.cpp
// Functions:
//   0x0008c7f4  eWaterFilter::~eWaterFilter(void)            eAll_psp.obj  (sched=1 zone)
//   0x0021d250  eWaterFilter::New(cMemPool *, cBase *) static eAll_psp.obj

class cBase;
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

#pragma control sched=1
// ── 0x0008c7f4 — ~eWaterFilter(void) ──
eWaterFilter::~eWaterFilter() {
    *(void **)((char *)this + 4) = eWaterFiltervirtualtable;
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
#pragma control sched=2
