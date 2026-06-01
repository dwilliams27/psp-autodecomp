// cMemPool::GetLock(void) static @ 0x00004a38
// Returns a pointer to the lazily-initialised pool mutex.

extern char cMemPoolLockName[];
extern "C" int sceKernelCreateMutex(const char *, int, int, int);
extern "C" int sceKernelDeleteMutex(int);
extern void __record_needed_destruction(void *);

class cLock {
public:
    int mMutex;
    cLock() { mMutex = sceKernelCreateMutex(cMemPoolLockName, 0, 0, 0); }
    ~cLock() { sceKernelDeleteMutex(mMutex); }
};

class cMemPool {
public:
    static cLock *GetLock();
};

cLock *cMemPool::GetLock() {
    static cLock theLock;
    return &theLock;
}
