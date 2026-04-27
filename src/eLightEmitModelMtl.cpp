// eLightEmitModelMtl.cpp — decompiled from eAll_psp.obj.
// Parent: eOnePassModelMtl. Vtable at 0x3854E8.
//
// Functions:
//   0x00082ca8 eLightEmitModelMtl::Write(cFile &) const             100B
//   0x00082e48 eLightEmitModelMtl::~eLightEmitModelMtl(void)        124B (deleting dtor auto-gen)
//   0x00218f80 eLightEmitModelMtl::New(cMemPool *, cBase *) static  124B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
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

class eOnePassModelMtl {
public:
    char _pad[0x80];
    eOnePassModelMtl(cBase *);
    ~eOnePassModelMtl(void);
    void Write(cFile &) const;
};

class eLightEmitModelMtl : public eOnePassModelMtl {
public:
    float mField80;          // 0x80
    unsigned int mField84;   // 0x84

    eLightEmitModelMtl(cBase *);
    ~eLightEmitModelMtl(void);
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

extern char eLightEmitModelMtlvirtualtable[];

// ── eLightEmitModelMtl::Write @ 0x00082ca8 ──
#pragma control sched=1
void eLightEmitModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eOnePassModelMtl::Write(file);
    wb.Write(mField80);
    wb.Write(mField84);
    wb.End();
}

// ── eLightEmitModelMtl::~eLightEmitModelMtl @ 0x00082e48 ──
eLightEmitModelMtl::~eLightEmitModelMtl(void) {
    *(void **)((char *)this + 4) = eLightEmitModelMtlvirtualtable;
}

// ── eLightEmitModelMtl::New @ 0x00218f80 ──
cBase *eLightEmitModelMtl::New(cMemPool *pool, cBase *parent) {
    eLightEmitModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eLightEmitModelMtl *obj = (eLightEmitModelMtl *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) eLightEmitModelMtl(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
