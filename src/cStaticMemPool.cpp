extern "C" void free(void *);

class cMemAllocator;

class cMemPool {
public:
    char mPad[0x98];

    cMemPool(cMemAllocator *, const char *);
    ~cMemPool();

    static cMemPool *GetPoolFromPtr(const void *);
};

class cFastMemAllocator {
public:
    cFastMemAllocator(const char *, unsigned int, unsigned int, void *);
    ~cFastMemAllocator();
};

extern char cStaticMemPoolvirtualtable[];
extern char cStaticMemPoolBaseVtable[];

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cStaticMemPool : public cMemPool {
public:
    void *mVtable;
    cFastMemAllocator mAllocator;

    cStaticMemPool(const char *, unsigned int, unsigned int);
    ~cStaticMemPool();

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

cStaticMemPool::cStaticMemPool(const char *name, unsigned int allocSize,
                               unsigned int poolSize)
    : cMemPool((cMemAllocator *)&mAllocator, name),
      mVtable(cStaticMemPoolvirtualtable),
      mAllocator((const char *)((char *)this + 4), allocSize, poolSize, 0) {}

cStaticMemPool::~cStaticMemPool() {
    mVtable = cStaticMemPoolBaseVtable;
}
