// gcValEntityVelocity_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcValEntityVelocity locally with only the
// destructor + operator delete. The matched siblings live in
// src/gcValEntityVelocity.cpp; field offsets here are shifted +4 versus the
// gcValEntityHasVariables family because gcValEntityVelocity carries an extra
// 4-byte field before the gcDesiredObject helper (sub starts at this+0xC).

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

class gcValEntityVelocity {
public:
    static void operator delete(void *);
    ~gcValEntityVelocity(void);
};

extern char gcValEntityVelocityvirtualtable[];

inline void gcValEntityVelocity::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oTgcValEntityVelocitydtv, 0x168\n");

gcValEntityVelocity::~gcValEntityVelocity(void) {
    *(void **)((char *)this + 4) = gcValEntityVelocityvirtualtable;
    char *baseDesc = (char *)0x37E6A8;
    char *outer = (char *)this + 0xC;

    if ((void *)outer != 0) {
        *(void **)((char *)this + 0x10) = (void *)0x388A48;
        char *first = (char *)this + 0x20;
        if ((void *)first != 0) {
            *(void **)((char *)this + 0x24) = (void *)0x388568;
            if ((void *)((char *)this + 0x34) != 0) {
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
            *(void **)((char *)this + 0x24) = baseDesc;
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
        *(void **)((char *)this + 0x10) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
