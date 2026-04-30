#include "eMemCard.h"

extern "C" int sceKernelLockMutex(int mutexid, int count, unsigned int *timeout);
extern "C" int sceKernelUnlockMutex(int mutexid, int count);

struct sMemCardState {
    int mutex;
    int field_4;
    int field_8;
    int freeBlocks;
};
extern sMemCardState gMemCardState;

extern unsigned char gMemCardPresent;
extern unsigned char gMemCardChanged;
extern unsigned char gMemCardDifferent;
extern int gMemCardStatus;

class cMemCardScopedLock {
    int *mMutex;
    bool mActive;
public:
    cMemCardScopedLock(bool active) {
        mMutex = &gMemCardState.mutex;
        __asm__ volatile("" : : "r"(mMutex));
        mActive = active;
        __asm__ volatile("" ::: "memory");
        sceKernelLockMutex(gMemCardState.mutex, 1, 0);
    }
    ~cMemCardScopedLock() {
        if (mActive) {
            int mutex = *mMutex;
            __asm__ volatile("" : : "r"(mutex));
            sceKernelUnlockMutex(mutex, 1);
        }
    }
};

void eMemCard::Initialize(void) {
    eMemCardPlatform::Initialize();
}

#pragma control sched=1

unsigned char eMemCard::CardDifferent(void) {
    cMemCardScopedLock lock(true);
    return gMemCardDifferent;
}

unsigned char eMemCard::CardPresent(void) {
    cMemCardScopedLock lock(true);
    int status = 3;
    unsigned char *presentPtr = &gMemCardPresent;
    unsigned char present = *presentPtr;
    if (present) {
        status = 0;
    }
    gMemCardStatus = status;
    __asm__ volatile("" ::: "memory");
    return present;
}

unsigned char eMemCard::CardChanged(void) {
    cMemCardScopedLock lock(true);
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    unsigned char changed = gMemCardChanged;
    gMemCardChanged = 0;
    return changed;
}

int eMemCard::GetCardFreeSpace(void) {
    cMemCardScopedLock lock(true);
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    return gMemCardState.freeBlocks << 5;
}

#pragma control sched=2

void eMemCard::SetSlot(int) {
    *(int *)0x37D2CC = 0;
}

float eMemCard::GetMinSaveTime(void) {
    return 1.0f;
}

float eMemCard::GetMinLoadTime(void) {
    return 1.0f;
}

void eMemCard::Format(void) {
    *(int *)0x37D2C8 = 0;
}

int eMemCard::GetSaveSize(int a, int b, int c) {
    int x = c;
    int y = b;
    return a + x * y;
}
