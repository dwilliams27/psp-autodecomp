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
    cMemCardScopedLock() {
        mMutex = &gMemCardState.mutex;
        mActive = true;
        sceKernelLockMutex(gMemCardState.mutex, 1, 0);
    }
    ~cMemCardScopedLock() {
        if (mActive) {
            sceKernelUnlockMutex(*mMutex, 1);
        }
    }
};

void eMemCard::Initialize(void) {
    eMemCardPlatform::Initialize();
}

unsigned char eMemCard::CardDifferent(void) {
    cMemCardScopedLock lock;
    return gMemCardDifferent;
}

unsigned char eMemCard::CardPresent(void) {
    cMemCardScopedLock lock;
    unsigned char present = gMemCardPresent;
    gMemCardStatus = present ? 0 : 3;
    return present;
}

unsigned char eMemCard::CardChanged(void) {
    cMemCardScopedLock lock;
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    unsigned char changed = gMemCardChanged;
    gMemCardChanged = 0;
    return changed;
}

int eMemCard::GetCardFreeSpace(void) {
    cMemCardScopedLock lock;
    gMemCardStatus = gMemCardPresent ? 0 : 3;
    return gMemCardState.freeBlocks << 5;
}

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
