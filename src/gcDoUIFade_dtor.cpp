// gcDoUIFade_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoUIFade locally with only the
// destructor + operator delete; the rest of the class lives across
// src/gcDoUIFade*.cpp split TUs. Adding a destructor declaration to the
// shared header could perturb the matched siblings' vtable handling.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoUIFade {
public:
    static void operator delete(void *);
    ~gcDoUIFade(void);
};

extern "C" void gcEvent_dtor(void *, int) asm("__0oHgcEventdtv");
extern "C" void gcDesiredCamera_dtor(void *, int) asm("__0oPgcDesiredCameradtv");
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoUIFadevirtualtable[];

inline void gcDoUIFade::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oKgcDoUIFadedtv, 0x1bc\n");

gcDoUIFade::~gcDoUIFade(void) {
    *(void **)((char *)this + 4) = gcDoUIFadevirtualtable;
    char *array78 = (char *)this + 0x78;
    char *ptr74 = (char *)this + 0x74;
    char *ptr54 = (char *)this + 0x54;
    char *ptr50 = (char *)this + 0x50;

    if ((void *)array78 != 0) {
        ((cBaseArray *)array78)->RemoveAll();
    }

    if ((void *)ptr74 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x74);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x74) = 0;
            }
        }
    }

    gcEvent_dtor((char *)this + 0x58, 2);

    if ((void *)ptr54 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x54);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x54) = 0;
            }
        }
    }

    if ((void *)ptr50 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x50);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x50) = 0;
            }
        }
    }

    gcDesiredCamera_dtor((char *)this + 0x14, 2);

    gcAction_dtor(this, 0);
}
