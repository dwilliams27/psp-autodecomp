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
