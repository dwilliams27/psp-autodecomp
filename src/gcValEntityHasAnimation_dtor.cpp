// gcValEntityHasAnimation_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValEntityHasAnimation locally with
// only the destructor + operator delete; the rest of the class lives in
// src/gcValEntityHasAnimation.cpp. Keep the layout assumptions in sync
// if either file changes.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValEntityHasAnimation {
public:
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    ~gcValEntityHasAnimation(void);
};

extern char gcValEntityHasAnimationvirtualtable[];

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oXgcValEntityHasAnimationdtv, 0x1dc\n");

gcValEntityHasAnimation::~gcValEntityHasAnimation(void) {
    *(void **)((char *)this + 4) = gcValEntityHasAnimationvirtualtable;
    char *p34 = (char *)this + 0x34;
    void *baseDesc = (void *)0x37E6A8;
    char *p8 = (char *)this + 8;

    if ((void *)p34 != 0) {
        *(void **)((char *)this + 0x38) = (void *)0x388568;
        if ((void *)((char *)this + 0x48) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x48);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x48) = 0;
            }
        }
        *(void **)((char *)this + 0x38) = baseDesc;
    }
    if ((void *)p8 != 0) {
        *(void **)((char *)this + 0xC) = (void *)0x388A48;
        if ((void *)((char *)this + 0x1C) != 0) {
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
