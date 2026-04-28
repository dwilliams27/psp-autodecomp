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

class cConfigBase {
public:
    cConfigBase(cBase *);
    ~cConfigBase();
};

class cConfigPSP : public cConfigBase {
public:
    cConfigPSP(cBase *);
    ~cConfigPSP();

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

extern char cConfigBasevirtualtable[];

extern "C" {
    void cConfigPSP__cConfigPSP_cBaseptr(void *self, cBase *parent);
}

// cConfigPSP::New(cMemPool *, cBase *) static @ 0x001c6bf8
cBase *cConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    cConfigPSP *result = 0;
    cConfigPSP *obj = (cConfigPSP *)rec->fn(base, 0x2BC, 4, 0, 0);
    if (obj != 0) {
        cConfigPSP__cConfigPSP_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// cConfigPSP::~cConfigPSP(void) @ 0x001c6d58
cConfigPSP::~cConfigPSP() {
    *(void **)((char *)this + 4) = cConfigBasevirtualtable;
}
