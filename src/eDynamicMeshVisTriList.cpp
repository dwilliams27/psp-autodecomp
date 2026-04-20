// eDynamicMeshVisTriList / eStaticMeshVisTriList / gcEntityController

// ─── cReadBlock / cFileHandle forward decls ───
class cReadBlock {
public:
    void *mHandle;   // offset 0: cFileHandle*
    int _data[4];
};

class cFileHandle;

class cFileSystem {
public:
    static void Read(cFileHandle *handle, void *buf, unsigned int size);
};

// cMemPool allocator layout used in Read / dtor
// pool->field_24 = block;  block->field_1C = allocTable (array of {short off; short pad; fn*})
extern "C" void *cMemPool_GetPoolFromPtr(void *);

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *base, int size, int a, int b, int c);
};

struct DelEntry {
    short offset;
    short pad;
    void (*fn)(void *base, void *ptr);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

extern "C" void free(void *);

// ─── eDynamicMeshVisTriList ───
class eDynamicMeshVisTriList {
public:
    void *mDisplayList;   // 0x00
    int   mField4;        // 0x04
    int   mField8;        // 0x08 — stride (multiplied by arg_int)
    short mFieldC;        // 0x0C
    short mFieldE;        // 0x0E
    float mField10;       // 0x10

    eDynamicMeshVisTriList(void);
    ~eDynamicMeshVisTriList(void);
    void Read(cReadBlock &rb, unsigned int prim, int stride, void *vbuf);
};

eDynamicMeshVisTriList::eDynamicMeshVisTriList(void) {
    mDisplayList = 0;
    mField4 = 0;
    mField8 = 0;
    mFieldC = 0;
    mFieldE = 0;
    __asm__ volatile("" ::: "memory");
    mField10 = 0.0f;
    __asm__ volatile("" ::: "memory");
}

extern "C" void eDynamicMeshVisTriList___dtor_eDynamicMeshVisTriList_void(eDynamicMeshVisTriList *self, int flags) {
    if (self != 0) {
        void *dl = self->mDisplayList;
        if (dl != 0) {
            void *pool = cMemPool_GetPoolFromPtr(dl);
            void *block = *(void **)((char *)pool + 0x24);
            char *tbl = *(char **)((char *)block + 0x1C);
            DelEntry *e = (DelEntry *)(tbl + 0x30);
            short off = e->offset;
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = e->fn;
            fn(base, self->mDisplayList);
            self->mDisplayList = 0;
        }
        if (flags & 1) {
            void *pool2 = cMemPool_GetPoolFromPtr(self);
            if (pool2 != 0) {
                void *block2 = *(void **)((char *)pool2 + 0x24);
                char *tbl2 = *(char **)((char *)block2 + 0x1C);
                DelEntry *e2 = (DelEntry *)(tbl2 + 0x30);
                short off2 = e2->offset;
                void *base2 = (char *)block2 + off2;
                void (*fn2)(void *, void *) = e2->fn;
                fn2(base2, self);
            } else {
                free(self);
            }
        }
    }
}

void eDynamicMeshVisTriList::Read(cReadBlock &rb, unsigned int prim, int stride, void *vbuf) {
    void *pool = cMemPool_GetPoolFromPtr(this);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &mField10, 4);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &mFieldE, 2);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &mFieldC, 2);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &mField8, 4);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &mField4, 4);
    short localShort;
    int count;
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &localShort, 2);
    cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &count, 4);

    void *block = *(void **)((char *)pool + 0x24);
    char *tbl = *(char **)((char *)block + 0x1C);
    AllocEntry *ae = (AllocEntry *)(tbl + 0x28);
    short aoff = ae->offset;
    void *abase = (char *)block + aoff;
    void *(*afn)(void *, int, int, int, int) = ae->fn;
    void *buf = afn(abase, (count + 4) * 4, 0, 0, 0);
    mDisplayList = buf;

    unsigned int vaddr = (unsigned int)vbuf + (unsigned int)(mField8 * stride);
    int *dl = (int *)buf;
    dl[0] = (int)(prim | 0x12000000);
    dl[1] = (int)((((vaddr >> 24) & 0xF) << 16) | 0x10000000);
    dl[2] = (int)((vaddr & 0xFFFFFF) | 0x01000000);
    int *p = dl + 3;

    for (int i = 0; i < count; i++) {
        short v;
        cFileSystem::Read((cFileHandle *)*(void **)rb.mHandle, &v, 2);
        *p = (int)((unsigned short)v | 0x04040000);
        p = p + 1;
    }
    *p = 0x0B000000;
}

// ─── eStaticMeshVisTriList ───
class eStaticMeshVisTriList {
public:
    int   mField0;
    unsigned char mField4;
    // pad at 5
    short mField6;
    float mField8;
    int   mFieldC;
    int   mField10;

    eStaticMeshVisTriList(void);
};

eStaticMeshVisTriList::eStaticMeshVisTriList(void) {
    mField0 = 0;
    mField4 = 0;
    mField6 = 0;
    __asm__ volatile("" ::: "memory");
    mField8 = 0.0f;
    __asm__ volatile("" ::: "memory");
    mFieldC = 0;
    mField10 = 0;
}

// ─── gcEntityController ───
class gcEntityController {
public:
    void **mField0;
    void *GetCurrentNavMeshPosition(void);
};

void *gcEntityController::GetCurrentNavMeshPosition(void) {
    void *result = 0;
    int *nav = (int *)(mField0[0x1F8 / 4]);
    if (nav[0x4C / 4] & 8) {
        result = (char *)nav + 0xC0;
    }
    return result;
}
