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

class cFastMemAllocator {
public:
    cFastMemAllocator(const char *, unsigned int, unsigned int, void *);
    ~cFastMemAllocator();
    float GetSizeScale(void) const;
    void Free(void *);

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

cFastMemAllocator::cFastMemAllocator(const char *name,
                                     unsigned int allocSize,
                                     unsigned int poolSize,
                                     void *memory) {
    cMemAllocator_construct((cMemAllocator *)this, name, poolSize, 1.0f);
    *(void **)((char *)this + 0x1C) = (void *)0x37E5E8;
    *(int *)((char *)this + 0x20) = 0;
    if (allocSize != 0) {
        cMemAllocator_AllocPoolMemory((cMemAllocator *)this, allocSize, memory);
    }
}

cFastMemAllocator::~cFastMemAllocator() {
    *(void **)((char *)this + 0x1C) = (void *)0x37E5E8;
    cMemAllocator_destruct((cMemAllocator *)this, 0);
}

float cFastMemAllocator::GetSizeScale(void) const {
    return 1.0f;
}
