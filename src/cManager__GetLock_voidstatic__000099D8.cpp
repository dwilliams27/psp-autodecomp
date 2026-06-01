// cManager::GetLock(void) static @ 0x000099d8
// Returns a pointer to the lazily-initialised manager mutex.
// Sibling of byte-exact cMemPool::GetLock; same lazy-static-local idiom.

extern char cManagerLockName[];
extern "C" int sceKernelCreateMutex(const char *, int, int, int);
extern "C" int sceKernelDeleteMutex(int);
extern void __record_needed_destruction(void *);

class cLock {
public:
    int mMutex;
    cLock() { mMutex = sceKernelCreateMutex(cManagerLockName, 0, 0, 0); }
    ~cLock() { sceKernelDeleteMutex(mMutex); }
};

class cManager {
public:
    static cLock *GetLock();
};

cLock *cManager::GetLock() {
    static cLock theLock;
    return &theLock;
}
