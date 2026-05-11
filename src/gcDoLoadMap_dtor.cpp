// gcDoLoadMap_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoLoadMap locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoLoadMap.cpp (New/GetType/AssignCopy/Write). Adding a
// destructor declaration to that class would perturb the matched siblings'
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

class gcDoLoadMap {
public:
    static void operator delete(void *);
    ~gcDoLoadMap(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoLoadMapvirtualtable[];

inline void gcDoLoadMap::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oLgcDoLoadMapdtv, 0x1dc\n");

gcDoLoadMap::~gcDoLoadMap(void) {
    *(void **)((char *)this + 4) = gcDoLoadMapvirtualtable;
    char *p34 = (char *)this + 0x34;
    char *p30 = (char *)this + 0x30;
    char *p24 = (char *)this + 0x24;
    char *p14 = (char *)this + 0x14;

    if ((void *)p34 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x34);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x34) = 0;
            }
        }
    }

    if ((void *)p30 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x30);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x30) = 0;
            }
        }
    }

    if ((void *)p24 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x24);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x24) = 0;
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

    gcAction_dtor(this, 0);
}
