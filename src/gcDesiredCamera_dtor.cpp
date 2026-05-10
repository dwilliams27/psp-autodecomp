// gcDesiredCamera::~gcDesiredCamera(void) @ 0x00121680, 360B
//
// Split TU per CLAUDE.md SPLIT-TU rule. Local redeclaration of gcDesiredCamera
// here exposes only the destructor + class-local operator delete; the rest of
// the class lives in src/gcDesiredCamera.cpp (Read/Write/GetText/New/ctor).
// Adding a destructor declaration to that TU's class would shift the matched
// constructor's vtable usage, so the destructor stays isolated here.
//
// ODR-WARNING: gcDesiredCamera is also defined in src/gcDesiredCamera.cpp and
// in src/gcCamera.cpp (constructor). The TUs reference disjoint method sets
// and agree on the field offsets touched by the destructor.

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

class gcDesiredCamera {
public:
    static void operator delete(void *);
    ~gcDesiredCamera(void);
};

inline void gcDesiredCamera::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcDesiredCameradtv, 0x168\n");

gcDesiredCamera::~gcDesiredCamera(void) {
    *(void **)((char *)this + 4) = (void *)0x388CF0;
    char *p_C = (char *)this + 0x0C;

    if ((void *)p_C != 0) {
        *(void **)((char *)this + 0x10) = (void *)0x388A48;
        char *p_20 = (char *)this + 0x20;

        if ((void *)p_20 != 0) {
            *(void **)((char *)this + 0x24) = (void *)0x388568;
            char *p_34 = (char *)this + 0x34;

            if ((void *)p_34 != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x34);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x34) = 0;
                }
            }
            *(void **)((char *)this + 0x24) = (void *)0x37E6A8;
        }

        *(void **)((char *)this + 0x10) = (void *)0x3889A8;

        if ((void *)((char *)this + 0x14) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x14);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
        *(void **)((char *)this + 0x10) = (void *)0x37E6A8;
    }
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
