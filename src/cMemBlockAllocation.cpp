class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cMemBlockAllocatorEntry {
    short offset;
    short pad;
    void *(*fn)(void *, void *);
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

    cMemBlockAllocation(cMemPool *, unsigned int, bool);
    cMemBlockAllocation(void *, bool);
};

cMemBlockAllocation::cMemBlockAllocation(cMemPool *pool, unsigned int size, bool) {
    mPtr = 0;
    mSize = size;
    mPrev = 0;
    mOwns = false;
    mPool = pool;
    mActive = true;

    void *block = *(void **)((char *)pool + 0x24);
    cMemBlockAllocatorEntry *entry =
        (cMemBlockAllocatorEntry *)(*(char **)((char *)block + 0x1C) + 0x38);
    short off = entry->offset;
    void *(*fn)(void *, void *) = entry->fn;
    mBlock = fn((char *)block + off, this);
}

cMemBlockAllocation::cMemBlockAllocation(void *ptr, bool active) {
    mPtr = ptr;
    mSize = 0;
    mPrev = 0;
    mActive = active;
    mOwns = true;

    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    mPool = pool;
    mActive = true;

    void *block = *(void **)((char *)pool + 0x24);
    cMemBlockAllocatorEntry *entry =
        (cMemBlockAllocatorEntry *)(*(char **)((char *)block + 0x1C) + 0x38);
    short off = entry->offset;
    void *(*fn)(void *, void *) = entry->fn;
    mBlock = fn((char *)block + off, this);
}
