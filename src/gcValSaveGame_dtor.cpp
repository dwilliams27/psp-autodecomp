// gcValSaveGame_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValSaveGame locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcValSaveGame.cpp. Keep the layout assumptions in sync if either
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

class gcValSaveGame {
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
    ~gcValSaveGame(void);
};

extern char gcValSaveGamevirtualtable[];

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oNgcValSaveGamedtv, 0x184\n");

gcValSaveGame::~gcValSaveGame(void) {
    *(void **)((char *)this + 4) = gcValSaveGamevirtualtable;
    char *p14 = (char *)this + 0x14;
    char *p10 = (char *)this + 0x10;
    char *pC = (char *)this + 0xC;

    if ((void *)p14 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }
    if ((void *)p10 != 0) {
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
    if ((void *)pC != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0xC);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0xC) = 0;
        }
    }
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
