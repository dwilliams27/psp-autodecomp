// ODR-WARNING: this TU locally redeclares cArrayBase<T> with the destructor
// only. Do not edit canonical headers — see docs/direction/005-regalloc-drift-guards.md.

class eDynamicMeshVisSkinTriList;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern void eDynamicMeshVisSkinTriList___dtor_eDynamicMeshVisSkinTriList_void(void *, int);
extern "C" void free(void *);

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
}

template <class T>
class cArrayBase {
public:
    int mData;
    ~cArrayBase();
};

#pragma control sched=1
template <class T>
cArrayBase<T>::~cArrayBase() {
    int count = 0;
    void *check = (void *)*(volatile int *)&mData;
    void *data = (void *)*(volatile int *)&mData;
    if (check != 0) {
        count = *((int *)check - 1) & 0x3FFFFFFF;
    }
    int i = 0;
    int offset = 0;
    while (i < count) {
        eDynamicMeshVisSkinTriList___dtor_eDynamicMeshVisSkinTriList_void((char *)data + offset, 2);
        i++;
        offset += 36;
        data = (void *)*(volatile int *)&mData;
    }
    if (data != 0) {
        int *alloc = (int *)data - 1;
        if (alloc != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(alloc);
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, alloc);
        }
        *(void **)&mData = 0;
    }
}

template cArrayBase<eDynamicMeshVisSkinTriList>::~cArrayBase();
#pragma control sched=2
