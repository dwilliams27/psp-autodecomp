// ODR-WARNING: split-TU method addition for gcValEntityIsActive. The canonical
// gcValEntityIsActive.cpp does not declare this destructor / operator delete
// pair; keep this local redeclaration isolated so matched siblings in
// src/gcValEntityIsActive.cpp do not see new class members.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValEntityIsActive {
public:
    ~gcValEntityIsActive();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcValEntityIsActivevirtualtable[];

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oTgcValEntityIsActivedtv, 0x168\n");

// -----------------------------------------------------------------------------
// gcValEntityIsActive::~gcValEntityIsActive(void) @ 0x00332bc0, 360B
// -----------------------------------------------------------------------------
gcValEntityIsActive::~gcValEntityIsActive() {
    *(void **)((char *)this + 4) = gcValEntityIsActivevirtualtable;
    void *baseVtable = (void *)0x37E6A8;
    char *helper = (char *)this + 8;

    if ((void *)helper != 0) {
        *(void **)((char *)this + 0xC) = (void *)0x388A48;

        if ((void *)((char *)this + 0x1C) != 0) {
            *(void **)((char *)this + 0x20) = (void *)0x388568;
            if ((void *)((char *)this + 0x30) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x30);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x30) = 0;
                }
            }
            *(void **)((char *)this + 0x20) = baseVtable;
        }

        *(void **)((char *)this + 0xC) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x10) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x10);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
        *(void **)((char *)this + 0xC) = baseVtable;
    }

    *(void **)((char *)this + 4) = baseVtable;
}
