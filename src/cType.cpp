class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern char D_000001F8[];

class cType {
public:
    char _pad[0x2C];
    void *mDeleteDesc;

    ~cType();

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

cType::~cType() {
    mDeleteDesc = D_000001F8;
}
