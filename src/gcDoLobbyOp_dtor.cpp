// gcDoLobbyOp_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoLobbyOp locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoLobbyOp.cpp (New/AssignCopy/GetType/Write/GetText). Adding a
// destructor declaration to that header could perturb the matched siblings.

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

class gcDoLobbyOp {
public:
    static void operator delete(void *);
    ~gcDoLobbyOp(void);
};

extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoLobbyOpvirtualtable[];

inline void gcDoLobbyOp::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oLgcDoLobbyOpdtv, 0x184\n");

gcDoLobbyOp::~gcDoLobbyOp(void) {
    *(void **)((char *)this + 4) = gcDoLobbyOpvirtualtable;
    char *first = (char *)this + 0x18;
    char *second = (char *)this + 0x14;
    char *third = (char *)this + 0x10;

    if ((void *)first != 0) {
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

    if ((void *)second != 0) {
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

    if ((void *)third != 0) {
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

    gcAction___dtor_gcAction_void(this, 0);
}
