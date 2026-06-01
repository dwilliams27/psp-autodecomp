// ODR-WARNING: split-TU isolated definition of eMemCard::GetCardFreeSpace.
// Mirrors src/eMemCard.cpp which ALSO defines this symbol (shared TU produces a
// 128B near-miss). This isolated TU uses the two-symbol split (gMemCardMutex at
// 0x98670, sMemCardPool at 0x98678 with freeBlocks at +4) to reach the expected
// 136B lui+addiu(-31112)+lw 4(base) sequence. Do not edit the shared TU here.

class eMemCard {
public:
    static int GetCardFreeSpace(void);
};

extern "C" int sceKernelLockMutex(int mutexid, int count, unsigned int *timeout);
extern "C" int sceKernelUnlockMutex(int mutexid, int count);

extern int gMemCardMutex;          // 0x98670

struct sMemCardPool {
    int field_0;                   // 0x98678
    int freeBlocks;                // 0x9867c
};
extern sMemCardPool gMemCardPool;  // 0x98678

extern unsigned char gMemCardPresent;
extern int gMemCardStatus;

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

int eMemCard::GetCardFreeSpace(void) {
    cMemCardScopedLock lock(true);
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    return gMemCardPool.freeBlocks << 5;
}
