// gcValFlyingControllerVariable_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValFlyingControllerVariable locally with
// only the destructor + operator delete. The matched siblings live in
// src/gcValFlyingControllerVariable.cpp; keep layout assumptions in sync if
// either file changes.

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

class gcValFlyingControllerVariable {
public:
    static void operator delete(void *);
    ~gcValFlyingControllerVariable(void);
};

extern char gcValFlyingControllerVariablevirtualtable[];

inline void gcValFlyingControllerVariable::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0odgcValFlyingControllerVariabledtv, 0x168\n");

gcValFlyingControllerVariable::~gcValFlyingControllerVariable(void) {
    *(void **)((char *)this + 4) = gcValFlyingControllerVariablevirtualtable;
    char *baseDesc = (char *)0x37E6A8;
    char *outer = (char *)this + 8;

    if ((void *)outer != 0) {
        *(void **)((char *)this + 0xC) = (void *)0x388A48;
        char *first = (char *)this + 0x1C;
        if ((void *)first != 0) {
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
