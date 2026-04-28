// gcStateHandlerBase — base class for state-machine handlers.

class cBase;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

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

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcStateHandlerBase {
public:
    gcStateHandlerBase(cBase *);
    ~gcStateHandlerBase();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009A3D0;

// ── gcStateHandlerBase::gcStateHandlerBase(cBase *) @ 0x00109D84 ──
gcStateHandlerBase::gcStateHandlerBase(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = (void *)0x388280;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x10) = 4;
    *(int *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
    *(int *)((char *)this + 0x1C) = (int)this | 1;
    *(int *)((char *)this + 0x20) = 0;
    *(int *)((char *)this + 0x24) = 0;
    *(int *)((char *)this + 0x28) = (int)this | 1;
}

// ── gcStateHandlerBase::New(cMemPool *, cBase *) static @ 0x0025708c ──
cBase *gcStateHandlerBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStateHandlerBase *result = 0;
    gcStateHandlerBase *obj = (gcStateHandlerBase *)rec->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcStateHandlerBase(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStateHandlerBase::GetType(void) const @ 0x00257108 ──
const cType *gcStateHandlerBase::GetType(void) const {
    if (D_0009A3D0 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009A3D0 = cType::InitializeType(0, 0, 0xD9, D_000385DC,
                                           &gcStateHandlerBase::New, 0, 0, 0);
    }
    return D_0009A3D0;
}

// ── gcStateHandlerBase::~gcStateHandlerBase(void) @ 0x0010a808 ──
gcStateHandlerBase::~gcStateHandlerBase() {
    *(void **)((char *)this + 4) = (void *)0x388280;

    void *prevSlot = (char *)this + 0x1C;
    if ((char *)this + 0x28 != 0) {
        int owned;
        int next = *(int *)((char *)this + 0x28);
        owned = 1;
        if (next & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (next != 0) {
                DeleteRec *rec = (DeleteRec *)(*(char **)(next + 4) + 0x50);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn((void *)(next + off), (void *)3);
                *(int *)((char *)this + 0x28) = 0;
            }
        }
    }

    if (prevSlot != 0) {
        int owned;
        int prev = *(int *)((char *)this + 0x1C);
        owned = 1;
        if (prev & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (prev != 0) {
                DeleteRec *rec = (DeleteRec *)(*(char **)(prev + 4) + 0x50);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn((void *)(prev + off), (void *)3);
                *(int *)((char *)this + 0x1C) = 0;
            }
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
