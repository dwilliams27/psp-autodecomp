class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcNamedSet {
public:
    cBase *mOwner;
    void *mClassDesc;

    const cType *GetType(void) const;
    ~gcNamedSet();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

extern cType *D_000385DC;
extern cType *D_000998F4;

gcNamedSet::~gcNamedSet() {
    mClassDesc = (void *)0x37E6A8;
}

const cType *gcNamedSet::GetType(void) const {
    if (D_000998F4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC, 0, 0, 0, 0);
    }
    return D_000998F4;
}
