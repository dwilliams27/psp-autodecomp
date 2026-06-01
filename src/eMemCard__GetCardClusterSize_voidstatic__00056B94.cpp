// ODR-WARNING: split-TU isolated definition of eMemCard::GetCardClusterSize.
// Sibling of eMemCard::GetCardFreeSpace (0x56b0c). Same RAII scoped-lock idiom,
// two-symbol global split (gMemCardMutex 0x98670, gMemCardPool 0x98678).
// Difference: cluster size = present ? (pool.unkC * pool.unk10) : 0, and the
// card-present byte must stay live across the status store (it gates the
// multiply). Do not edit the shared TU here.

class eMemCard {
public:
    static int GetCardClusterSize(void);
};

extern "C" int sceKernelLockMutex(int mutexid, int count, unsigned int *timeout);
extern "C" int sceKernelUnlockMutex(int mutexid, int count);

extern int gMemCardMutex;          // 0x98670

struct sMemCardPool {
    int field_0;                   // 0x98678  (+0)
    int field_4;                   // +4  (freeBlocks in sibling)
    int field_8;                   // +8
    int clusterCount;              // +12 (0xC)
    int clusterBlocks;             // +16 (0x10)
};
extern sMemCardPool gMemCardPool;  // 0x98678

extern unsigned char gMemCardPresent;  // 0x37d3bc
extern int gMemCardStatus;             // 0x37d2c8

class cMemCardScopedLock {
    int *mMutex;
    bool mActive;
public:
    cMemCardScopedLock(bool active) {
        mMutex = &gMemCardMutex;
        __asm__ volatile("" : : "r"(mMutex));
        mActive = active;
        __asm__ volatile("" ::: "memory");
        sceKernelLockMutex(gMemCardMutex, 1, 0);
    }
    ~cMemCardScopedLock() {
        if (mActive) {
            int mutex = *mMutex;
            __asm__ volatile("" : : "r"(mutex));
            sceKernelUnlockMutex(mutex, 1);
        }
    }
};

#pragma control sched=1

int eMemCard::GetCardClusterSize(void) {
    cMemCardScopedLock lock(true);
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    int result = 0;
    if (gMemCardPresent) {
        result = gMemCardPool.clusterBlocks * gMemCardPool.clusterCount;
    }
    return result;
}
