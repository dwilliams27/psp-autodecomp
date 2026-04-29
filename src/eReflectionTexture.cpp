// eReflectionTexture — sched=1 zone (eAll_psp.obj unity build)
//
// Functions:
//   0x00084eb8  eReflectionTexture::~eReflectionTexture(void)
//   0x00219da8  eReflectionTexture::New(cMemPool *, cBase *) static

class cBase;
class cMemPool;

class cObject {
public:
    cObject &operator=(const cObject &);
};

template <class T> T *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

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
    void AssignCopy(const cBase *);
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

// ── eReflectionTexture::AssignCopy(const cBase *) @ 0x00219d08 ──
void eReflectionTexture::AssignCopy(const cBase *src) {
    eReflectionTexture *other = dcast<eReflectionTexture>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(signed char *)((char *)this + 0x44) = *(const signed char *)((char *)other + 0x44);
    *(signed char *)((char *)this + 0x45) = *(const signed char *)((char *)other + 0x45);
    *(signed char *)((char *)this + 0x46) = *(const signed char *)((char *)other + 0x46);
    *(unsigned char *)((char *)this + 0x47) = *(const unsigned char *)((char *)other + 0x47);
    *(short *)((char *)this + 0x48) = *(const short *)((char *)other + 0x48);
    *(short *)((char *)this + 0x4A) = *(const short *)((char *)other + 0x4A);
    __asm__ volatile("" ::: "memory");
    int *dst4C = (int *)((char *)this + 0x4C);
    int *src4C = (int *)((char *)other + 0x4C);
    *dst4C = *src4C;
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x60) = *(const float *)((char *)other + 0x60);
    int *dst64 = (int *)((char *)this + 0x64);
    int *src64 = (int *)((char *)other + 0x64);
    *dst64 = *src64;
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
