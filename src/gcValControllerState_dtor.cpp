// gcValControllerState_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValControllerState locally with only
// the destructor + operator delete; the rest of the class lives in
// src/gcValControllerState.cpp. Keep the layout assumptions in sync if
// either file changes.

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

class gcValControllerState {
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
    ~gcValControllerState(void);
};

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oUgcValControllerStatedtv, 0x1c0\n");

gcValControllerState::~gcValControllerState(void) {
    *(void **)((char *)this + 4) = (void *)0x388AE8;
    char *p40 = (char *)this + 0x40;
    char *outer = (char *)this + 0x14;
    void *baseDesc = (void *)0x37E6A8;

    if ((void *)p40 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x40);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x40) = 0;
        }
    }
    if ((void *)outer != 0) {
        *(void **)((char *)this + 0x18) = (void *)0x388A48;
        if ((void *)((char *)this + 0x28) != 0) {
            *(void **)((char *)this + 0x2C) = (void *)0x388568;
            if ((void *)((char *)this + 0x3C) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x3C);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x3C) = 0;
                }
            }
            *(void **)((char *)this + 0x2C) = baseDesc;
        }
        *(void **)((char *)this + 0x18) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x1C) != 0) {
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
        *(void **)((char *)this + 0x18) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
