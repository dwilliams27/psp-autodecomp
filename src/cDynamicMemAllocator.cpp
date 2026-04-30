#include "cFileSystem.h"

extern "C" void *memset(void *, int, unsigned int);
extern "C" void free(void *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemAllocator;

extern "C" cMemAllocator *cMemAllocator_construct(cMemAllocator *, const char *,
                                                  unsigned int, float)
    asm("__0oNcMemAllocatorctPCcUif");
extern "C" void cMemAllocator_destruct(cMemAllocator *, int)
    asm("__0oNcMemAllocatordtv");
extern "C" void cMemAllocator_AllocPoolMemory(cMemAllocator *, unsigned int,
                                              void *)
    asm("__0fNcMemAllocatorPAllocPoolMemoryUiPv");

class cDynamicMemAllocator {
public:
    cDynamicMemAllocator(const char *, unsigned int, unsigned int, void *);
    ~cDynamicMemAllocator();
    void LogAllocations(void) const;
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

#pragma control sched=1

cDynamicMemAllocator::cDynamicMemAllocator(const char *name,
                                           unsigned int allocSize,
                                           unsigned int poolSize,
                                           void *memory) {
    float scale = 1.05f;
    __asm__ volatile("" ::: "memory");
    cMemAllocator_construct((cMemAllocator *)this, name, poolSize, scale);
    *(void **)((char *)this + 0x1C) = (void *)0x37E640;
    *(int *)((char *)this + 0x1AC) = 0;
    *(int *)((char *)this + 0x1B0) = 0;
    *(int *)((char *)this + 0x1B4) = 0;
    *(int *)((char *)this + 0x1B8) = -1;
    memset((char *)this + 0x20, 0, 0x180);
    if (allocSize != 0) {
        cMemAllocator_AllocPoolMemory((cMemAllocator *)this, allocSize, memory);
    }
}

#pragma control sched=2

cDynamicMemAllocator::~cDynamicMemAllocator() {
    *(void **)((char *)this + 0x1C) = (void *)0x37E640;
    LogAllocations();
    cMemAllocator_destruct((cMemAllocator *)this, 0);
}

void cDynamicMemAllocator::LogAllocations(void) const {
}

int cFileSystem::Create(const char *) {
    return 0;
}
