class cMemAllocator;

extern "C" void free(void *);
extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" int sceKernelLockMutex(int, int, unsigned int *);
extern "C" int sceKernelUnlockMutex(int, int);

int cStrLength(const char *);
void cStrCopy(char *, const char *, int);

struct cMemPoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct cMemBlockEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    unsigned int mHash;
    char mName[0x20];
    cMemAllocator *mAllocator;
    int mField28;
    cMemPool *mPrev;
    cMemPool *mNext;
    unsigned char mGUIDs[0x40];
    char mPad74[0x20];
    int mGUIDCount;
    void *mClassDesc;

    cMemPool(cMemAllocator *, const char *);
    static int *GetLock(void);
    static cMemPool *GetPoolFromPtr(const void *);
};

class cMemBlockSuspend {
public:
    cMemPool *mPool;

    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend();

    static void operator delete(void *p);
};

inline void cMemBlockSuspend::operator delete(void *p) {
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

cMemBlockSuspend::~cMemBlockSuspend() {
    cMemPoolBlock *block = *(cMemPoolBlock **)((char *)mPool + 0x24);
    cMemBlockEntry *entry = (cMemBlockEntry *)(block->allocTable + 0x48);
    short off = entry->offset;
    void (*fn)(void *, int) = entry->fn;
    fn((char *)block + off, 0);
}

cMemBlockSuspend::cMemBlockSuspend(cMemPool *pool) {
    mPool = pool;

    cMemPoolBlock *block = *(cMemPoolBlock **)((char *)pool + 0x24);
    cMemBlockEntry *entry = (cMemBlockEntry *)(block->allocTable + 0x48);
    short off = entry->offset;
    void (*fn)(void *, int) = entry->fn;
    fn((char *)block + off, 1);
}

cMemPool::cMemPool(cMemAllocator *allocator, const char *name) {
    void *classDesc = (void *)0x37E698;
    mAllocator = allocator;
    mClassDesc = classDesc;
    mField28 = 0;
    mPrev = 0;
    mNext = 0;
    __vec_new(mGUIDs, 8, 8, (void (*)(void *))0x1C607C);

    char *dst = mName;
    mGUIDCount = 0;
    cStrCopy(dst, name, 0x20);
    mName[0x1F] = 0;

    int len = cStrLength(dst);
    int var_a3;
    int var_a1 = len - 1;
    unsigned int crc = (unsigned int)-1;
    unsigned char *p = (unsigned char *)dst;
    var_a3 = var_a1;
    cMemPool **head = (cMemPool **)0x380000;

    if (len != 0) {
        unsigned int *table = (unsigned int *)0x37BC50;
        int old_a1;
        do {
            unsigned int temp = table[(crc ^ *p) & 0xFF];
            old_a1 = var_a3;
            var_a3 = var_a1 - 1;
            p++;
            crc = temp ^ (crc >> 8);
            __asm__ volatile("" ::: "memory");
            var_a1 = var_a3;
        } while (old_a1 != 0);
    }

    mHash = crc ^ (unsigned int)-1;

    sceKernelLockMutex(*GetLock(), 1, 0);
    cMemPool *first = head[-4076];
    __asm__ volatile("" ::: "memory");
    if (first != 0) {
        mNext = first->mNext;
        first = head[-4076];
        mPrev = first;
        cMemPool *next = mNext;
        next->mPrev = this;
        __asm__ volatile("" ::: "memory");
        first = mPrev;
        first->mNext = this;
    } else {
        head[-4076] = this;
        mPrev = this;
        mNext = this;
    }
    sceKernelUnlockMutex(*GetLock(), 1);
}
