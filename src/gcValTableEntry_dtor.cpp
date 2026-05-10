// gcValTableEntry_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValTableEntry locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcValTableEntry.cpp. Keep the layout assumptions in sync if either
// file changes.

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

class gcValTableEntry {
public:
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    ~gcValTableEntry(void);
};

extern char D_0000A198[];
extern char D_00000838[];

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcValTableEntrydtv, 0x1B8\n");

gcValTableEntry::~gcValTableEntry(void) {
    *(void **)((char *)this + 4) = D_0000A198;
    char *p20 = (char *)this + 0x20;
    char *p1C = (char *)this + 0x1C;
    char *p8 = (char *)this + 8;
    void *baseDesc = (void *)0x37E6A8;

    if ((void *)p20 != 0) {
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
    if ((void *)p1C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x1C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x1C) = 0;
        }
    }
    if ((void *)p8 != 0) {
        *(void * volatile *)((char *)this + 0xC) = (void *)0x38A000;
        *(void * volatile *)((char *)this + 0xC) = D_00000838;
        *(void * volatile *)((char *)this + 0xC) = (void *)0x3889A8;
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
        *(void **)((char *)this + 0xC) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
