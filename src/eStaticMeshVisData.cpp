// ODR-WARNING: split translation unit for eStaticMeshVisData destructor only.
// Keep local declarations minimal to avoid changing matched users of the header.

#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

class eStaticMeshVisTriList {
public:
    char _pad[0x14];
    ~eStaticMeshVisTriList(void);
};

class eStaticMeshVisData {
public:
    eStaticMeshVisTriList *mData;

    ~eStaticMeshVisData(void);

    static void operator delete(void *p) {
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
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSeStaticMeshVisDatadtv, 0x130\n");

eStaticMeshVisData::~eStaticMeshVisData(void) {
    eStaticMeshVisTriList *entries = mData;
    int count = 0;
    if (entries != 0) {
        count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
    }

    int i = 0;
    int offset = 0;
    if (i < count) {
        do {
            ((eStaticMeshVisTriList *)((char *)mData + offset))->~eStaticMeshVisTriList();
            i++;
            offset += 0x14;
        } while (i < count);
    }

    if (mData != 0) {
        char *basePtr = (char *)mData - 4;
        if (basePtr != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, basePtr);
        }
        mData = 0;
    }
}

#pragma control sched=2
