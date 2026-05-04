// cDynamicMemPool — gcAll_psp.obj
//   0x00247164 cDynamicMemPool::~cDynamicMemPool(void)   (160B, deleting)
//
// cDynamicMemPool inherits from cMemPool (size 0x9C) and embeds a
// cDynamicMemAllocator at offset 0x9C.  The deleting destructor sets the
// cDynamicMemPool vtable, destructs the embedded allocator, invokes
// ~cMemPool(), then dispatches via cMemPool::operator delete (pool-aware).

extern "C" void free(void *);

class cMemAllocator;

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    unsigned int mHash;            // 0x00
    char mName[0x20];               // 0x04
    cMemAllocator *mAllocator;      // 0x24
    int mField28;                   // 0x28
    cMemPool *mPrev;                // 0x2C
    cMemPool *mNext;                // 0x30
    unsigned char mGUIDs[0x40];    // 0x34
    char mPad74[0x20];              // 0x74
    int mGUIDCount;                 // 0x94
    void *mClassDesc;               // 0x98
    ~cMemPool();
    static cMemPool *GetPoolFromPtr(const void *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            PoolDeleteSlot *slot = (PoolDeleteSlot *)(((char **)block)[7] + 0x30);
            short off = slot->offset;
            void (*fn)(void *, void *) = slot->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

class cDynamicMemAllocator {
public:
    char _pad[0x1BC];               // size large enough; only the dtor matters
    ~cDynamicMemAllocator();
};

class cDynamicMemPool : public cMemPool {
public:
    cDynamicMemAllocator mAllocator;   // 0x9C
    ~cDynamicMemPool();
};

extern char __0dPcDynamicMemPoolG__vtbl[];

cDynamicMemPool::~cDynamicMemPool() {
    mClassDesc = (void *)__0dPcDynamicMemPoolG__vtbl;
    // implicit ~mAllocator(2) and implicit ~cMemPool(0) emitted by SNC
}
