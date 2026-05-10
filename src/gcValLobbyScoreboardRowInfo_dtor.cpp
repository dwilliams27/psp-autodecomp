// gcValLobbyScoreboardRowInfo_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValLobbyScoreboardRowInfo locally with
// only the destructor + operator delete; the rest of the class lives in
// src/gcValLobbyScoreboardRowInfo.cpp. Keep the layout assumptions in sync
// if either file changes.

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

class gcValLobbyScoreboardRowInfo {
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
    ~gcValLobbyScoreboardRowInfo(void);
};

extern char gcValLobbyScoreboardRowInfovirtualtable[];

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0obgcValLobbyScoreboardRowInfodtv, 0x184\n");

gcValLobbyScoreboardRowInfo::~gcValLobbyScoreboardRowInfo(void) {
    *(void **)((char *)this + 4) = gcValLobbyScoreboardRowInfovirtualtable;
    char *p18 = (char *)this + 0x18;
    char *p14 = (char *)this + 0x14;
    char *p10 = (char *)this + 0x10;

    if ((void *)p18 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }
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
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
