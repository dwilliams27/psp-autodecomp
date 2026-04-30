#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eStaticMeshVisTriList_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct eStaticMeshVisTriList_DeleteRecord4 {
    short offset;
    short pad;
    void (*fn)(void *, void *, short, void *);
};

extern "C" void free(void *);

class eStaticMeshVisTriList {
public:
    void *mField0;
    unsigned char mField4;
    char _pad5;
    short mField6;
    float mField8;
    int mFieldC;
    void *mField10;

    ~eStaticMeshVisTriList();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            register volatile eStaticMeshVisTriList_DeleteRecord *rec __asm__("a1") =
                (volatile eStaticMeshVisTriList_DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

eStaticMeshVisTriList::~eStaticMeshVisTriList() {
    void *field10 = mField10;
    if (field10 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(field10);
        char *block = ((char **)pool)[9];
        register volatile eStaticMeshVisTriList_DeleteRecord4 *rec __asm__("a1") =
            (volatile eStaticMeshVisTriList_DeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) = rec->fn;
        fn(base, *(void **)((char *)this + 0x10), off, (void *)fn);
        mField10 = 0;
    }
    void *field0 = mField0;
    if (field0 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(field0);
        char *block = ((char **)pool)[9];
        register volatile eStaticMeshVisTriList_DeleteRecord4 *rec __asm__("a1") =
            (volatile eStaticMeshVisTriList_DeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) = rec->fn;
        fn(base, *(void **)this, off, (void *)fn);
        mField0 = 0;
    }
}
