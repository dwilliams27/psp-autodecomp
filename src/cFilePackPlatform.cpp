#include "cFileSystem.h"

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cFilePackPlatform {
public:
    void *mPackData;         // 0x00
    char _pad4[0x14];        // 0x04
    cFileHandle *mHandle;    // 0x18

    ~cFilePackPlatform();
    void PackClose(void);
    void PackFree(void *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

void cFilePackPlatform::PackClose(void) {
    if (mHandle != 0) {
        cFileSystem::Close(mHandle);
        mHandle = 0;
    }
}

void cFilePackPlatform::PackFree(void *ptr) {
    if (ptr != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, ptr);
    }
}

cFilePackPlatform::~cFilePackPlatform() {
    if (mPackData != 0) {
        PackFree(mPackData);
        mPackData = 0;
    }
    PackClose();
}
