// cMemBlockAllocation::~cMemBlockAllocation(void)
// Address: 0x0000262c, Size: 176B
// Obj: cAll_psp.obj
// Symbol: __0oTcMemBlockAllocationdtv
//
// Split-TU: class declared locally here (the shared TU src/cMemBlockAllocation.cpp
// holds the two ctors only; this file owns the destructor).
//
// Deleting-destructor idiom (matches sibling src/cFastMemAllocator.cpp): the
// `static operator delete` body supplies the GetPoolFromPtr-or-free teardown that
// SNC auto-emits under the hidden `flag & 1` test.

extern "C" void free(void *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cMemBlockAllocatorEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemBlockAllocation {
public:
    void *mPtr;
    unsigned int mSize;
    unsigned int mPrev;
    bool mActive;
    bool mOwns;
    char mPad[2];
    cMemPool *mPool;
    void *mBlock;

    ~cMemBlockAllocation();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            cMemBlockAllocatorEntry *rec =
                (cMemBlockAllocatorEntry *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

cMemBlockAllocation::~cMemBlockAllocation() {
    cMemPool *pool = mPool;
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        cMemBlockAllocatorEntry *rec =
            (cMemBlockAllocatorEntry *)(((char **)block)[7] + 0x40);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        ((void (*)(void *, void *, void *, short))fn)(block + off, this, mBlock, off);
    }
}
