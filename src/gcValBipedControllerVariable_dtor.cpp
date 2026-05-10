// gcValBipedControllerVariable_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValBipedControllerVariable locally with
// only the destructor + operator delete; the rest of the class lives in
// src/gcValBipedControllerVariable.cpp. Keep the layout assumptions in sync
// if either file changes.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValBipedControllerVariable {
public:
    static void operator delete(void *);
    ~gcValBipedControllerVariable(void);
};

extern char gcValBipedControllerVariablevirtualtable[];

inline void gcValBipedControllerVariable::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0ocgcValBipedControllerVariabledtv, 0x168\n");

gcValBipedControllerVariable::~gcValBipedControllerVariable(void) {
    *(void **)((char *)this + 4) = gcValBipedControllerVariablevirtualtable;
    void *baseDesc = (void *)0x37E6A8;
    char *outer = (char *)this + 8;

    if ((void *)outer != 0) {
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
            *(void **)((char *)this + 0x20) = baseDesc;
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
        *(void **)((char *)this + 0xC) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
