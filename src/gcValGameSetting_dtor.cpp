// ODR-WARNING: split-TU method addition for gcValGameSetting. The canonical
// gcValGameSetting.cpp does not declare this destructor/operator delete pair;
// keep this local redeclaration isolated so matched siblings in
// src/gcValGameSetting.cpp do not see new class members.

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

class gcValGameSetting {
public:
    ~gcValGameSetting();

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

extern char gcValGameSettingvirtualtable[];

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oQgcValGameSettingdtv, 0x148\n");

// -----------------------------------------------------------------------------
// gcValGameSetting::~gcValGameSetting(void) @ 0x00345c14, 328B
// -----------------------------------------------------------------------------
gcValGameSetting::~gcValGameSetting() {
    *(void **)((char *)this + 4) = gcValGameSettingvirtualtable;
    void *baseVtable = (void *)0x37E6A8;
    char *first = (char *)this + 0x10;
    char *outer = (char *)this + 0x0C;

    if ((void *)first != 0) {
        *(void **)((char *)this + 0x14) = (void *)0x388568;
        if ((void *)((char *)this + 0x24) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x24);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x24) = 0;
            }
        }
        *(void **)((char *)this + 0x14) = baseVtable;
    }

    if ((void *)outer != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    *(void **)((char *)this + 4) = baseVtable;
}
