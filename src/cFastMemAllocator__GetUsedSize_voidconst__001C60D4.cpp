// ODR-WARNING: this TU locally redeclares cFastMemAllocator with only the
// members and methods needed by GetUsedSize(). Do not add `#include
// "include/cFastMemAllocator.h"` and do not edit that header; sibling matched
// methods in src/cFastMemAllocator.cpp can drift if this TU's class shape
// leaks into the canonical one. Keep this declaration minimal. Mirrors the
// sibling RAII lock idiom used by cFastMemAllocator::Reset.

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

class cFastMemAllocator {
public:
    char _pad0[8];
    unsigned int mPoolBase;     // 0x08
    unsigned int mFlags;        // 0x0C
    char _pad10[8];
    int mMutex;                 // 0x18
    char _pad1C[4];
    unsigned int mCurrent;      // 0x20

    int GetUsedSize(void) const;
};

int cFastMemAllocator::GetUsedSize(void) const {
    cMemAllocatorLock lock((mFlags & 8) != 0, ((cFastMemAllocator *)this)->mMutex);
    return mCurrent - mPoolBase;
}
