// gcDoSaveGameOp_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoSaveGameOp locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoSaveGameOp.cpp (New/GetType/AssignCopy/Write/Read/GetText). Adding
// a destructor declaration to that class would perturb the matched siblings'
// vtable handling.

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

class gcDoSaveGameOp {
public:
    static void operator delete(void *);
    ~gcDoSaveGameOp(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoSaveGameOpvirtualtable[];

inline void gcDoSaveGameOp::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oOgcDoSaveGameOpdtv, 0x1dc\n");

gcDoSaveGameOp::~gcDoSaveGameOp(void) {
    *(void **)((char *)this + 4) = gcDoSaveGameOpvirtualtable;
    char *p1C = (char *)this + 0x1C;
    char *p18 = (char *)this + 0x18;
    char *p14 = (char *)this + 0x14;
    char *p10 = (char *)this + 0x10;

    if ((void *)p1C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x1C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x1C) = 0;
            }
        }
    }

    if ((void *)p18 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x18) = 0;
            }
        }
    }

    if ((void *)p14 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
    }

    if ((void *)p10 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
    }

    gcAction_dtor(this, 0);
}
