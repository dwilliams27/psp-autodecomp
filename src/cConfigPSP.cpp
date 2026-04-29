class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
    static cBase *New(cMemPool *, cBase *);
};

class cConfigPSP : public cConfigBase {
public:
    cConfigPSP(cBase *);
    ~cConfigPSP();

    const cType *GetType(void) const;
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
extern cType *D_000385DC;
extern cType *D_00038888;
extern cType *D_0003888C;

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

// cConfigPSP::GetType(void) const @ 0x001c6c74
const cType *cConfigPSP::GetType(void) const {
    if (D_0003888C == 0) {
        if (D_00038888 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                   (const char *)0x36C838,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00038888 = cType::InitializeType(0, 0, 0x28A, D_000385DC,
                                               &cConfigBase::New, 0, 0, 0);
        }
        D_0003888C = cType::InitializeType(0, 0, 0x291, D_00038888,
                                           &cConfigPSP::New, 0, 0, 0);
    }
    return D_0003888C;
}

// cConfigPSP::~cConfigPSP(void) @ 0x001c6d58
cConfigPSP::~cConfigPSP() {
    *(void **)((char *)this + 4) = cConfigBasevirtualtable;
}
