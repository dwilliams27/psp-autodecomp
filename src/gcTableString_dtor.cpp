// gcTableString::~gcTableString(void) @ 0x0028c064, 440B
//
// Split TU per CLAUDE.md SPLIT-TU rule. The rest of gcTableString's methods
// live in src/gcTableString.cpp; adding a destructor declaration there could
// shift codegen for matched siblings.
//
// ODR-WARNING: gcTableString is also defined in src/gcTableString.cpp. The
// two TUs reference disjoint method sets and agree on field offsets touched
// here (+0x08 gcDesiredObject base, +0x0C vtable in sub-object, +0x10/+0x1C/+0x20
// tagged-pointer/handle fields).

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

extern char D_000015A8[];
extern char D_00000838[];

class gcTableString {
public:
    static void operator delete(void *);
    ~gcTableString(void);
};

inline void gcTableString::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oNgcTableStringdtv, 0x1b8\n");

gcTableString::~gcTableString(void) {
    *(void **)((char *)this + 4) = D_000015A8;

    char *p_20 = (char *)this + 0x20;
    char *p_1C = (char *)this + 0x1C;
    char *p_08 = (char *)this + 0x08;

    if ((void *)p_20 != 0) {
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

    if ((void *)p_1C != 0) {
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

    if ((void *)p_08 != 0) {
        *(void * volatile *)((char *)this + 0x0C) = (void *)0x38A000;
        *(void * volatile *)((char *)this + 0x0C) = D_00000838;
        *(void * volatile *)((char *)this + 0x0C) = (void *)0x3889A8;

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

        *(void **)((char *)this + 0x0C) = (void *)0x37E6A8;
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
