extern "C" void free(void *);
extern "C" int sceKernelDeleteFpl(int);
extern "C" int sceKernelFreeFpl(int, void *);
extern int cMemPoolPlatform_totalAllocatedBytes;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cMemPoolPlatformDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolPlatform {
public:
    int mSize;
    int mAllocHandle;
    void *mBlock;

    ~cMemPoolPlatform(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            cMemPoolPlatformDeleteRecord *rec =
                (cMemPoolPlatformDeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

cMemPoolPlatform::~cMemPoolPlatform(void) {
    if (mAllocHandle >= 0) {
        int total = cMemPoolPlatform_totalAllocatedBytes;
        cMemPoolPlatform_totalAllocatedBytes = total - mSize;
        sceKernelFreeFpl(mAllocHandle, mBlock);
        sceKernelDeleteFpl(mAllocHandle);
        mSize = 0;
        mAllocHandle = -1;
        mBlock = 0;
    }
}
