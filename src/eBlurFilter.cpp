// Functions:
//   0x0007fc10 eBlurFilter::Write(cFile &) const               eAll_psp.obj
//   0x0007fdcc eBlurFilter::~eBlurFilter(void)                  eAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eTextureFilter {
public:
    eTextureFilter(cBase *);
    ~eTextureFilter();
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

inline void *operator new(unsigned int, void *p) { return p; }

class eBlurFilter : public eTextureFilter {
public:
    eBlurFilter(cBase *);
    ~eBlurFilter();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

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

extern char eBlurFiltervirtualtable[];

static cType *type_cBase;
static cType *type_eTextureFilter;
static cType *type_eBlurFilter;

#pragma control sched=1

eBlurFilter::eBlurFilter(cBase *parent) : eTextureFilter(parent) {
    *(void **)((char *)this + 4) = eBlurFiltervirtualtable;
    *(float *)((char *)this + 0x10) = 0.5f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x14) = 1.0f;
    *(int *)((char *)this + 0x18) = -1;
}

#pragma control sched=2

#pragma control sched=1

cBase *eBlurFilter::New(cMemPool *pool, cBase *parent) {
    eBlurFilter *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBlurFilter *obj = (eBlurFilter *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBlurFilter(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2

#pragma control sched=1

const cType *eBlurFilter::GetType(void) const {
    if (!type_eBlurFilter) {
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
        const cType *parentType = type_eTextureFilter;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x217580;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eBlurFilter =
            cType::InitializeType(0, 0, 0x13D, parentType, factory, 0, 0, 0);
    }
    return type_eBlurFilter;
}

#pragma control sched=2

#pragma control sched=1
void eBlurFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eTextureFilter::Write(file);
    wb.Write(*(float *)((char *)this + 0x10));
    wb.Write(*(float *)((char *)this + 0x14));
    wb.Write(*(unsigned int *)((char *)this + 0x18));
    wb.End();
}

eBlurFilter::~eBlurFilter() {
    *(void **)((char *)this + 4) = eBlurFiltervirtualtable;
}
#pragma control sched=2
