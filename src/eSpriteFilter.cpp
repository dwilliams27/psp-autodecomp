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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    int Read(cFile &, cMemPool *);
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
    int Read(cFile &, cMemPool *);
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

template <class T> T *dcast(const cBase *);

extern char eSpriteFiltervirtualtable[];
extern cType *D_000385DC;
extern cType *D_00046C60;
extern cType *D_00046CBC;

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

extern "C" {
    void eSpriteFilter__eSpriteFilter_cBaseptr(void *self, cBase *parent);
}

#pragma control sched=1
// ── 0x0008bfb8 — eSpriteFilter(cBase *), 72B ──
eSpriteFilter::eSpriteFilter(cBase *parent) : eTextureFilter(parent) {
    mClassDesc = eSpriteFiltervirtualtable;
    mField10.mId = 0;
    mField14 = false;
    mField18 = 0xFF000000;
    mField1C = false;
}

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

// ── 0x0008be48 — Read(cFile &, cMemPool *), 368B ──
int eSpriteFilter::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);
    unsigned int version = (unsigned int)rb._data[3];
    if (version >= 5 || version < 1) goto fail;
    if (!eTextureFilter::Read(file, pool)) goto fail;
    *(int *)((char *)this + 0x10) = 0;
    __asm__ volatile("" ::: "memory");
    goto read_handle;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

read_handle:
    {
        cHandle *h = (cHandle *)((char *)this + 0x10);
        cMemPool *handlePool = cMemPool::GetPoolFromPtr(h);
        h->Read(rb, handlePool);
    }
    version = (unsigned int)rb._data[3];
    if (version >= 2) {
        char flag;
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &flag, 1);
        *(unsigned char *)((char *)this + 0x14) = flag != 0;
        version = (unsigned int)rb._data[3];
    }
    if (version >= 3) {
        char flag;
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &flag, 1);
        *(unsigned char *)((char *)this + 0x1C) = flag != 0;
        version = (unsigned int)rb._data[3];
    }
    if (version >= 4) {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x18, 4);
    }
    return result;
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

// ── 0x0021ce00 — GetType(void) const, 216B ──
const cType *eSpriteFilter::GetType(void) const {
    if (D_00046CBC == 0) {
        if (D_00046C60 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00046C60 = cType::InitializeType(
                0, 0, 0x13B, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00046C60;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x21CD84;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CBC = cType::InitializeType(
            0, 0, 0x63, parentType, factory, 0, 0, 0);
    }
    return D_00046CBC;
}
#pragma control sched=2
