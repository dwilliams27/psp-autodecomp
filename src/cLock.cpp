// cLock::cLock(void) @ 0x001c5fac
// Wraps a Sony kernel mutex.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cLock {
public:
    int mMutex;
    cLock();
    ~cLock();
};

extern char cLockMutexName[];
extern "C" int sceKernelCreateMutex(const char *, int, int, int);
extern "C" int sceKernelDeleteMutex(int);
extern "C" void free(void *);

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    } else {
        free(p);
    }
}

cLock::cLock() {
    mMutex = sceKernelCreateMutex(cLockMutexName, 0, 0, 0);
}

cLock::~cLock() {
    sceKernelDeleteMutex(mMutex);
}
