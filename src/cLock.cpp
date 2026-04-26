// cLock::cLock(void) @ 0x001c5fac
// Wraps a Sony kernel mutex.

class cLock {
public:
    int mMutex;
    cLock();
};

extern char cLockMutexName[];
extern "C" int sceKernelCreateMutex(const char *, int, int, int);

cLock::cLock() {
    mMutex = sceKernelCreateMutex(cLockMutexName, 0, 0, 0);
}
