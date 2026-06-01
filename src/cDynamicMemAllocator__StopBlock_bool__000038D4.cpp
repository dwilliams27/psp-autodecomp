// Split-TU: locally redeclares cDynamicMemAllocator with only the members and
// methods needed by StopBlock(bool). Do NOT add an include of a shared header
// and do NOT edit include/*.h. Mirrors the RAII lock idiom used by the matched
// sibling cFastMemAllocator::GetUsedSize / Reset (lock on construct, unlock on
// scope exit -- both return paths converge on the destructor).

extern "C" int sceKernelLockMutex(int, int, unsigned int *);
extern "C" int sceKernelUnlockMutex(int, int);

class cMemAllocatorLock {
    int *mMutex;
    bool mActive;
public:
    cMemAllocatorLock(bool active, int &mutex)
        : mMutex(&mutex), mActive(active) {
        if (active) {
            sceKernelLockMutex(mutex, 1, 0);
        }
    }
    ~cMemAllocatorLock() {
        if (mActive) {
            sceKernelUnlockMutex(*mMutex, 1);
        }
    }
};

struct cMemBlock {
    char _pad0[8];
    int mCount;                 // 0x08
};

class cDynamicMemAllocator {
public:
    char _pad0[12];
    unsigned int mFlags;        // 0x0C
    char _pad10[8];
    int mMutex;                 // 0x18

    cMemBlock *GetBlock(void);
    void StopBlock(bool);
};

extern "C" void *cDynamicMemAllocator_GetBlock(cDynamicMemAllocator *)
    asm("__0fUcDynamicMemAllocatorIGetBlockv");

void cDynamicMemAllocator::StopBlock(bool start) {
    cMemAllocatorLock lock((mFlags & 8) != 0, mMutex);
    cMemBlock *block = (cMemBlock *)cDynamicMemAllocator_GetBlock(this);
    if (block == 0) {
        return;
    }
    block->mCount += start ? 1 : -1;
}
