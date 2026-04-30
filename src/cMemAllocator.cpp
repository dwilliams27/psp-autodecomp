#include "cThread.h"
#include "cFastMemAllocator.h"
#include "cFile.h"

extern "C" void free(void *);
extern "C" int sceKernelDeleteMutex(int);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cDynamicMemAllocator {
public:
    int GetUsedSize(void) const;
};

class cNamed {
public:
    void OnNameChanged(void);
};

inline void cMemAllocator::operator delete(void *p) {
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

cMemAllocator::~cMemAllocator(void) {
    *(void **)((char *)this + 0x1C) = (void *)0x37E590;
    if ((char *)this + 0x18 != 0) {
        sceKernelDeleteMutex(*(int *)((char *)this + 0x18));
    }
}

void cMemAllocator::StopBlock(bool) {
}

void cFastMemAllocator::Free(void *) {
}

int cDynamicMemAllocator::GetUsedSize(void) const {
    return *(int *)((char *)this + 0x1AC);
}

bool cListSubscriber::IsModifiable(void) const {
    return false;
}

void cNamed::OnNameChanged(void) {
}
