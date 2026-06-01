// ODR-WARNING: split-TU local redeclaration of cMemPool and friends.
// Target: cMemPool::AllocGlobalPool(void) static @ 0x00004aa4
// Emits: __0fIcMemPoolPAllocGlobalPoolvT
// Do NOT add these declarations to include/*.h.

class cMemAllocator;

class cMemPool {
public:
    cMemPool(cMemAllocator *allocator, const char *name);
    static cMemPool *AllocGlobalPool(void);
};

// cFastMemAllocator constructor (this, name, size, flags, ptr)
extern void cFastMemAllocator_ctor(void *self, const char *name,
                                   unsigned int size, int flags, void *ptr);

extern void __record_needed_destruction(void *);

// The lazily-constructed global pool wrapper: a cMemPool base immediately
// followed by an embedded cFastMemAllocator at member offset 0x9C.
struct GlobalPoolStorage {
    char poolBytes[0x9C];   // cMemPool subobject (offset 0x98 = field below)
    char allocBytes[1];     // cFastMemAllocator subobject begins here
};

extern GlobalPoolStorage gGlobalPoolStorage;       // at 0xADB4
extern cMemPool *gGlobalPoolPtr;                    // at 0x38778

extern void cMemPool_ctor(cMemPool *self, cMemAllocator *allocator, const char *name);
extern void *gPoolClassDesc;   // global at address 0x2B8

static int gGlobalPoolGuard;

cMemPool *cMemPool::AllocGlobalPool(void) {
    cMemPool *result = (cMemPool *)&gGlobalPoolStorage;
    if (gGlobalPoolGuard == 0) {
        gGlobalPoolGuard = 1;
        cMemPool_ctor(result, (cMemAllocator *)((char *)result + 0x9C), "Global");
        *(void **)((char *)result + 0x98) = &gPoolClassDesc;
        cFastMemAllocator_ctor((char *)result + 0x9C,
                               (const char *)((char *)result + 4), 0x4000, 0, 0);
        __record_needed_destruction((void *)0x37C0C4);
    }
    gGlobalPoolPtr = result;
    return result;
}
