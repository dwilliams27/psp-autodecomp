// gcUIWidgetList_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcUIWidgetList locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcUIWidgetList.cpp. Do not promote this into the shared header without
// rechecking the matched siblings in that TU.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DtorSlot80Record {
    short offset;
    short pad;
    void (*fn)(void *, short, int, void *);
};

class gcUIWidgetList {
public:
    static void operator delete(void *);
    ~gcUIWidgetList(void);
};

extern "C" void gcUIWidgetGroup___dtor_gcUIWidgetGroup_void(void *, int);

inline void gcUIWidgetList::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oOgcUIWidgetListdtv, 0x144\n");

gcUIWidgetList::~gcUIWidgetList(void) {
    *(void **)((char *)this + 4) = (void *)0x38A9A8;
    int var_a2 = *(int *)((char *)this + 0xD8);
    int var_a0 = 0;
    int temp_a1 = var_a2 & 1;
    char *slotD8 = (char *)this + 0xD8;
    if (temp_a1 != 0) {
        var_a0 = 1;
    }
    int var_a0_2;
    if (var_a0 != 0) {
        var_a0_2 = 0;
    } else {
        var_a0_2 = ((var_a2 != 0) & 0xFF) != 0;
    }
    int var_a0_3 = 0;
    if (var_a0_2 != 0) {
        if (temp_a1 != 0) {
            var_a0_3 = 1;
        }
        char *typeInfo;
        if (var_a0_3 == 0) {
            typeInfo = *(char **)(var_a2 + 4);
        } else {
            var_a2 = 0;
            typeInfo = *(char **)(var_a2 + 4);
        }
        DtorSlot80Record *slot = (DtorSlot80Record *)(typeInfo + 0x80);
        short off = slot->offset;
        void (*fn)(void *, short, int, void *) = slot->fn;
        fn((char *)var_a2 + off, off, var_a2, (void *)fn);
    }

    if ((void *)slotD8 != 0) {
        int val = *(int *)((char *)this + 0xD8);
        int owned = 1;
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0xD8) = 0;
        }
    }

    gcUIWidgetGroup___dtor_gcUIWidgetGroup_void(this, 0);
}
