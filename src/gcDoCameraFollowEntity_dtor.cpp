// gcDoCameraFollowEntity_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoCameraFollowEntity locally with only the
// destructor + operator delete; the rest of the class lives in
// src/gcDoCameraFollowEntity.cpp (New/GetType/Write/Read/GetText) and
// src/gcDoCameraFollowEntity_Read.cpp. Adding a destructor declaration to the
// shared class definition could perturb the matched siblings' vtable handling.

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

class gcDoCameraFollowEntity {
public:
    static void operator delete(void *);
    ~gcDoCameraFollowEntity(void);
};

extern "C" void gcDesiredCamera_dtor(void *, int) asm("__0oPgcDesiredCameradtv");
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoCameraFollowEntityvirtualtable[];
extern char gcDoCameraModevirtualtable[];

extern char cBase_vtbl_a[]  asm("__0dFcBaseG__vtbl");                    // 0x37E6A8
extern char cBase_vtbl_b[]  asm("__0dFcBaseG__vtbl");                    // 0x37E6A8 (alias to prevent CSE)
extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");          // 0x388A48
extern char gcDesObj_vtbl[] asm("__0dPgcDesiredObjectG__vtbl");          // 0x3889A8
extern char gcDesEnum_vtbl[] asm("__0dZgcDesiredEnumerationEntryG__vtbl"); // 0x388568

inline void gcDoCameraFollowEntity::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oWgcDoCameraFollowEntitydtv, 0x1e0\n");

gcDoCameraFollowEntity::~gcDoCameraFollowEntity(void) {
    *(void **)((char *)this + 4) = gcDoCameraFollowEntityvirtualtable;
    char *p44 = (char *)this + 0x44;

    if ((void *)((char *)this + 0x4C) != 0) {
        *(void **)((char *)this + 0x50) = gcDesEnt_vtbl;

        if ((void *)((char *)this + 0x60) != 0) {
            *(void **)((char *)this + 0x64) = gcDesEnum_vtbl;

            if ((void *)((char *)this + 0x74) != 0) {
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
            *(void **)((char *)this + 0x64) = cBase_vtbl_a;
        }

        *(void **)((char *)this + 0x50) = gcDesObj_vtbl;

        if ((void *)((char *)this + 0x54) != 0) {
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
        *(void **)((char *)this + 0x50) = cBase_vtbl_b;
    }

    *(void **)((char *)this + 4) = gcDoCameraModevirtualtable;

    if ((void *)p44 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x44);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x44) = 0;
            }
        }
    }

    gcDesiredCamera_dtor((char *)this + 0x0C, 2);
    gcAction_dtor(this, 0);
}
