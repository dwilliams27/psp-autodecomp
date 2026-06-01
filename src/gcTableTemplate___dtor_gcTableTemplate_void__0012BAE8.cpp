// gcTableTemplate::~gcTableTemplate(void) — 0x0012bae8, 304B, gcAll_psp.obj
// Symbol: __0oPgcTableTemplatedtv
//
// SNC virtual/deleting destructor. Sets the vtable literal at this+4, releases
// the cBaseArray storage at this+0x50 through the pool-resident destructor
// record (GetPoolFromPtr(ptr) -> [9]=block -> block->allocTable + 0x30), runs an
// empty index-count loop (an inlined cBaseArray length scan whose body was
// optimized away), calls cBaseArray::RemoveAll on the embedded array at
// this+0x48, chains to the base cObject::~cObject, then — in the SNC
// deleting-destructor tail (flag&1) — frees `this` via the in-class
// operator delete.
//
// Standalone-class form modeled on the matched gcDesiredEntityTemplate /
// gcStatsTrackingStrings dtor exemplars: a non-inheriting class with a
// non-virtual ~Class() plus a static inline operator delete makes SNC emit the
// `dtv` symbol with the literal vtable write and the flag&1 pool-delete tail
// (no auto base-vtable management). The trailing trap word + .size recreate the
// terminal `b $; nop`.
//
// Split-TU local redeclaration: no new declarations added to include/*.h.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;       // 0x1C
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

// cObject::~cObject(void) base destructor — real jal, second arg = non-deleting.
extern "C" void __0oHcObjectdtv(void *, int);

class gcTableTemplate {
public:
    ~gcTableTemplate(void);
    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            DeleteEntry *rec =
                (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
            short off = rec->offset;
            rec->fn(block + off, p);
        }
    }
};

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcTableTemplatedtv, 0x130\n");

gcTableTemplate::~gcTableTemplate(void) {
    *(char **)((char *)this + 4) = (char *)0x389F60;
    cBaseArray *arr48 = (cBaseArray *)((char *)this + 0x48);
    char *slot50 = (char *)this + 0x50;
    if (slot50 != 0) {
        char *data = *(char **)((char *)this + 0x50);
        int count = 0;
        char * dup56468 = data;
        if (data != 0) {
            count = ((int *)dup56468)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DeleteEntry *rec =
                    (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
                void (*fn)(void *, void *) = rec->fn;
                short off = rec->offset;
                fn(block + off, data);
            }
            *(int *)((char *)this + 0x50) = 0;
        }
    }
    if (arr48 != 0) {
        arr48->RemoveAll();
    }
    __0oHcObjectdtv(this, 0);
}
