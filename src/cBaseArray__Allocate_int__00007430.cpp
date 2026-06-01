class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    int *mData;
    void *mOwner;

    int *Allocate(int);
};

typedef int *(*cBaseArray_AllocFn)(void *, int, int, int, int);

struct cBaseArray_AllocEntry {
    short offset;
    short _pad;
    cBaseArray_AllocFn func;
};

struct cBaseArray_Allocator {
    char _pad[28];
    char *table;
};

struct cBaseArray_Pool {
    char _pad[36];
    cBaseArray_Allocator *allocator;
};

int *cBaseArray::Allocate(int size) {
    if (size == 0) {
        return 0;
    }

    cBaseArray_Allocator *allocator =
        ((cBaseArray_Pool *)cMemPool::GetPoolFromPtr(this))->allocator;
    cBaseArray_AllocEntry *entry =
        (cBaseArray_AllocEntry *)(allocator->table + 0x28);
    cBaseArray_AllocFn fn = entry->func;
    short adjust = entry->offset;
    int *result = fn((char *)allocator + adjust, size * 4 + 4, 0, 0, 0);
    *result = size;
    return result + 1;
}
