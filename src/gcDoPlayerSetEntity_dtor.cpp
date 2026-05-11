// gcDoPlayerSetEntity_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoPlayerSetEntity locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoPlayerSetEntity.cpp (New/GetType/Read/Write/GetText). Adding a
// destructor declaration to that class would perturb the matched siblings'
// vtable handling.
//
// COMPILE FLAG: this TU requires -Xmopt=0 (disables SNC's "memory
// optimization" pass that caches frequently-used constants in extra
// callee-save registers). The original gcDoPlayerSetEntity destructor was
// compiled without that pass, so SNC emits inline `lui/addiu` pairs for the
// 0x37E6A8 base-vtable writes instead of caching the constant in s3. The
// per-TU pragma applies the same flag without touching the Makefile.

#pragma control mopt=0

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

class gcDoPlayerSetEntity {
public:
    static void operator delete(void *);
    ~gcDoPlayerSetEntity(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoPlayerSetEntityvirtualtable[];

inline void gcDoPlayerSetEntity::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oTgcDoPlayerSetEntitydtv, 0x1cc\n");

gcDoPlayerSetEntity::~gcDoPlayerSetEntity(void) {
    *(void **)((char *)this + 4) = gcDoPlayerSetEntityvirtualtable;
    char *value = (char *)this + 0x0C;
    char *helper = (char *)this + 0x10;

    if ((void *)helper != 0) {
        *(void **)((char *)this + 0x14) = (void *)0x388A48;
        if ((void *)((char *)this + 0x24) != 0) {
            *(void **)((char *)this + 0x28) = (void *)0x388568;
            if ((void *)((char *)this + 0x38) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x38);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x38) = 0;
                }
            }
            *(void **)((char *)this + 0x28) = (void *)0x37E6A8;
        }
        *(void **)((char *)this + 0x14) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x18) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x18);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x18) = 0;
            }
        }
        *(void **)((char *)this + 0x14) = (void *)0x37E6A8;
    }

    if ((void *)value != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    gcAction_dtor(this, 0);
}
