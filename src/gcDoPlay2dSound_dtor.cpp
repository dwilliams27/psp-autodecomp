// gcDoPlay2dSound_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoPlay2dSound locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoPlay2dSound.cpp (AssignCopy/New/GetType/Write/GetText). Adding a
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

class gcDoPlay2dSound {
public:
    static void operator delete(void *);
    ~gcDoPlay2dSound(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoPlay2dSoundvirtualtable[];
extern char D_00000658[];
extern char D_00389800[];

inline void gcDoPlay2dSound::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcDoPlay2dSounddtv, 0x1c0\n");

gcDoPlay2dSound::~gcDoPlay2dSound(void) {
    *(void **)((char *)this + 4) = gcDoPlay2dSoundvirtualtable;
    char *first = (char *)this + 0x2C;
    char *second = (char *)this + 0x28;
    char *helper = (char *)this + 0x14;

    if ((void *)first != 0) {
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

    if ((void *)second != 0) {
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

    if ((void *)helper != 0) {
        *(void *volatile *)((char *)this + 0x18) = D_00389800;
        *(void *volatile *)((char *)this + 0x18) = D_00000658;
        *(void *volatile *)((char *)this + 0x18) = (void *)0x3889A8;

        if ((void *)((char *)this + 0x1C) != 0) {
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

        *(void **)((char *)this + 0x18) = (void *)0x37E6A8;
    }

    gcAction_dtor(this, 0);
}
