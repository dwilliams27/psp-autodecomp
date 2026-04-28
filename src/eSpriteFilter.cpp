// src/eSpriteFilter.cpp
// Functions:
//   0x0008bdcc  eSpriteFilter::Write(cFile &) const            eAll_psp.obj  (sched=1 zone)
//   0x0008c000  eSpriteFilter::~eSpriteFilter(void)            eAll_psp.obj  (sched=1 zone)
//   0x0021cd0c  eSpriteFilter::AssignCopy(const cBase *)        eAll_psp.obj
//   0x0021cd84  eSpriteFilter::New(cMemPool *, cBase *) static  eAll_psp.obj
//
// eSpriteFilter derives from eTextureFilter. Layout (relative to this):
//   +0x00 vtable / cBase header
//   +0x08 int (eTextureFilter base field)
//   +0x0C int (eTextureFilter base field)
//   +0x10 cHandle  (texture / image handle)
//   +0x14 bool
//   +0x18 unsigned int
//   +0x1C bool

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class eTextureFilter {
public:
    eTextureFilter(cBase *);
    ~eTextureFilter();
    void Write(cFile &) const;
};

class eSpriteFilter : public eTextureFilter {
public:
    cBase   *mOwner;          // +0x00
    void    *mClassDesc;      // +0x04
    int      mField8;         // +0x08
    int      mFieldC;         // +0x0C
    cHandle  mField10;        // +0x10
    bool     mField14;        // +0x14
    unsigned int mField18;    // +0x18
    bool     mField1C;        // +0x1C

    eSpriteFilter(cBase *);
    ~eSpriteFilter();
    void AssignCopy(const cBase *);
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

template <class T> T *dcast(const cBase *);

extern char eSpriteFiltervirtualtable[];

extern "C" {
    void eSpriteFilter__eSpriteFilter_cBaseptr(void *self, cBase *parent);
}

#pragma control sched=1
// ── 0x0008bdcc — Write(cFile &) const, 124B ──
void eSpriteFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    eTextureFilter::Write(file);
    mField10.Write(wb);
    wb.Write(mField14);
    wb.Write(mField1C);
    wb.Write(mField18);
    wb.End();
}

// ── 0x0008c000 — ~eSpriteFilter(), 124B ──
eSpriteFilter::~eSpriteFilter() {
    *(void **)((char *)this + 4) = eSpriteFiltervirtualtable;
}

// ── 0x0021cd0c — AssignCopy(const cBase *), 120B ──
void eSpriteFilter::AssignCopy(const cBase *base) {
    eSpriteFilter *other = dcast<eSpriteFilter>(base);
    *(cHandle *)((char *)this + 8) = *(const cHandle *)((char *)other + 8);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 20) = *(const unsigned char *)((char *)other + 20);
    *(cHandle *)((char *)this + 24) = *(const cHandle *)((char *)other + 24);
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 28) = *(const unsigned char *)((char *)other + 28);
}

// ── 0x0021cd84 — New(cMemPool *, cBase *) static, 124B ──
cBase *eSpriteFilter::New(cMemPool *pool, cBase *parent) {
    eSpriteFilter *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eSpriteFilter *obj = (eSpriteFilter *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        eSpriteFilter__eSpriteFilter_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
