// eSprite.cpp -- decompiled from eAll_psp.obj.
//
// Functions:
//   0x001e6f84 eSprite::New(cMemPool *, cBase *) static
//   0x001e7428 eSprite::~eSprite(void)

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) {
    return p;
}

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
    short _pad;
    void (*fn)(void *, void *);
};

class cObject {
public:
    char _pad[0x44];

    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
};

class eSprite : public cObject {
public:
    int mField44;
    int mField48;
    float mField4C;
    float mField50;
    float mField54;
    float mField58;
    int mField5C;
    int mField60;
    int mField64;
    int mField68;

    eSprite(cBase *);
    ~eSprite();
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

template <class T> T *dcast(const cBase *);

struct CopyWord {
    int value;
};

// -- eSprite::AssignCopy(const cBase *) @ 0x001e6eec --
void eSprite::AssignCopy(const cBase *src) {
    eSprite *other = dcast<eSprite>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(const int *)((char *)other + 0x44);
    *(CopyWord *)((char *)this + 0x48) = *(CopyWord *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(const float *)((char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(const float *)((char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(const float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(const float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(const float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(const float *)((char *)other + 0x68);
}

// -- eSprite::New(cMemPool *, cBase *) static @ 0x001e6f84 --
cBase *eSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eSprite *result = 0;
    eSprite *obj = (eSprite *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eSprite(parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- eSprite::~eSprite(void) @ 0x001e7428 --
eSprite::~eSprite() {
    *(void **)((char *)this + 4) = (void *)0x3806A8;
}
