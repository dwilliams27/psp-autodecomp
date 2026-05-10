// ODR-WARNING: split-TU method addition for gcValObjectHasCategory. The
// canonical gcValObjectHasCategory.cpp does not declare this destructor /
// operator delete pair; keep this local redeclaration isolated so matched
// siblings in src/gcValObjectHasCategory.cpp do not see new class members.

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

class gcValObjectHasCategory {
public:
    ~gcValObjectHasCategory();

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

extern char gcValObjectHasCategoryvirtualtable[];

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oWgcValObjectHasCategorydtv, 0x148\n");

// -----------------------------------------------------------------------------
// gcValObjectHasCategory::~gcValObjectHasCategory(void) @ 0x0035547c, 328B
// -----------------------------------------------------------------------------
gcValObjectHasCategory::~gcValObjectHasCategory() {
    *(void **)((char *)this + 4) = gcValObjectHasCategoryvirtualtable;
    void *baseVtable = (void *)0x37E6A8;
    char *first = (char *)this + 0x0C;
    char *outer = (char *)this + 0x08;

    if ((void *)first != 0) {
        *(void **)((char *)this + 0x10) = (void *)0x388568;
        if ((void *)((char *)this + 0x20) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x20);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x20) = 0;
            }
        }
        *(void **)((char *)this + 0x10) = baseVtable;
    }

    if ((void *)outer != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }

    *(void **)((char *)this + 4) = baseVtable;
}
