// eReflectionTexture — sched=1 zone (eAll_psp.obj unity build)
//
// Functions:
//   0x00084eb8  eReflectionTexture::~eReflectionTexture(void)
//   0x00219da8  eReflectionTexture::New(cMemPool *, cBase *) static

class cBase;
class cMemPool;

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

#pragma control sched=1

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
    ~eVirtualTexture();
};

class eReflectionTexture : public eVirtualTexture {
public:
    eReflectionTexture(cBase *);
    ~eReflectionTexture();
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ── eReflectionTexture::~eReflectionTexture(void) @ 0x00084eb8 ──
eReflectionTexture::~eReflectionTexture() {
    *(void **)((char *)this + 4) = (void *)0x3857C8;
}

// ── eReflectionTexture::New(cMemPool *, cBase *) static @ 0x00219da8 ──
cBase *eReflectionTexture::New(cMemPool *pool, cBase *parent) {
    eReflectionTexture *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eReflectionTexture *obj = (eReflectionTexture *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eReflectionTexture(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
