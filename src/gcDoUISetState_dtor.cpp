// gcDoUISetState_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoUISetState locally with only the
// destructor + operator delete. Do not promote this into a shared header
// without checking matched siblings in src/gcDoUISetState.cpp.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoUISetState {
public:
    static void operator delete(void *);
    ~gcDoUISetState(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");
extern char gcDoUISetStatevirtualtable[];
extern char D_000006F8[];

inline void gcDoUISetState::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oOgcDoUISetStatedtv, 0x364\n");

gcDoUISetState::~gcDoUISetState(void) {
    *(void **)((char *)this + 4) = gcDoUISetStatevirtualtable;
    char *p3C = (char *)this + 0x3C;
    char *p34 = (char *)this + 0x34;
    char *p30 = (char *)this + 0x30;
    char *p2C = (char *)this + 0x2C;
    char *p28 = (char *)this + 0x28;
    char *p24 = (char *)this + 0x24;
    char *p20 = (char *)this + 0x20;
    char *p1C = (char *)this + 0x1C;

    if ((void *)p3C != 0) {
        *(void **)((char *)this + 0x40) = D_000006F8;
        if ((void *)((char *)this + 0x44) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x44);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x44) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x40) = (void *)0x37E6A8;
    }

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

    if ((void *)p2C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x2C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x2C) = 0;
            }
        }
    }

    if ((void *)p28 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x28);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x28) = 0;
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

    if ((void *)p20 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x20);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x20) = 0;
            }
        }
    }

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

    gcAction_dtor(this, 0);
}
