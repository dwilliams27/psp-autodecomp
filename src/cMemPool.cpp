extern "C" void free(void *);
extern "C" int sceKernelLockMutex(int, int, unsigned int *);
extern "C" int sceKernelUnlockMutex(int, int);

class cMemAllocator {
public:
    void FreePoolMemory(void);
};

struct PoolDeleteSlot {
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

    ~cMemPool(void);
    static cMemPool *GetPoolFromPtr(const void *);
    static int *GetLock(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            PoolDeleteSlot *slot = (PoolDeleteSlot *)(((char **)block)[7] + 0x30);
            short off = slot->offset;
            void (*fn)(void *, void *) = slot->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

cMemPool::~cMemPool(void) {
    void *classDesc = (void *)0x37E698;
    mClassDesc = classDesc;

    sceKernelLockMutex(*GetLock(), 1, 0);
    if (mNext != 0) {
        cMemPool *prev = mPrev;
        if (prev != 0) {
            cMemPool **head = (cMemPool **)0x380000;
            if (head[-4076] == this) {
                head[-4076] = prev;
                prev = mPrev;
            }
            cMemPool *next = mNext;
            next->mPrev = prev;
            next = mNext;
            prev->mNext = next;
            mNext = 0;
            mPrev = 0;
            if (head[-4076] == this) {
                head[-4076] = 0;
            }
        }
    }
    sceKernelUnlockMutex(*GetLock(), 1);

    mAllocator->FreePoolMemory();
}
