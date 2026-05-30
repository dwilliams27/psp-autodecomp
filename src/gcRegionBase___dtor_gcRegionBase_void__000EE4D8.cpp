// gcRegionBase::~gcRegionBase(void) @ 0x000ee4d8  (deleting destructor variant)
// Emits __0oMgcRegionBasedtv.
//
// Split TU per SPLIT-TU rule. The main gcRegionBase definition is not used
// here; only the destructor, the cBaseArray sub-array RemoveAll, the cObject
// base destructor, and the cMemPool delete path are exposed via local
// redeclarations.
//
// ODR-WARNING: local redeclarations expose only what this dtor touches.

class cBaseArray {
public:
    void RemoveAll(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(void *, int) asm("__0oHcObjectdtv");

class gcRegionBase {
public:
    static void operator delete(void *);
    ~gcRegionBase(void);
};

#pragma control sched=2

inline void gcRegionBase::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    char *block = ((char **)pool)[9];
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
    slot->fn(block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oMgcRegionBasedtv, 0xa4\n");

gcRegionBase::~gcRegionBase(void) {
    *(void **)((char *)this + 4) = (void *)0x387838;

    cBaseArray *sub = (cBaseArray *)((char *)this + 0x5C);
    if (sub != 0) {
        sub->RemoveAll();
    }

    *(void **)((char *)this + 4) = (void *)0x37E9C0;
    cObject_dtor(this, 0);
}
