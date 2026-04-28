class cBase;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class eConfigBase {
public:
    eConfigBase(cBase *);
    ~eConfigBase();
};

class eConfigPSP : public eConfigBase {
public:
    eConfigPSP(cBase *);
    ~eConfigPSP();

    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern "C" {
    void eConfigPSP__eConfigPSP_cBaseptr(void *self, cBase *parent);
}

// eConfigPSP::New(cMemPool *, cBase *) static @ 0x001df84c
cBase *eConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigPSP *result = 0;
    eConfigPSP *obj = (eConfigPSP *)rec->fn(base, 0x94, 4, 0, 0);
    if (obj != 0) {
        eConfigPSP__eConfigPSP_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// eConfigPSP::~eConfigPSP(void) @ 0x001dfc00
eConfigPSP::~eConfigPSP() {
    *(void **)((char *)this + 4) = (void *)0x37F930;
}
