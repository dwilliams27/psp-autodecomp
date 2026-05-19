// gcDoObjectLoad_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcDoObjectLoad is also defined in src/gcDoObjectLoad.cpp.
// This TU redeclares only the destructor + operator delete so the
// already-matched methods (New, AssignCopy, GetType, Write, Read) keep
// their current codegen.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoObjectLoad {
public:
    static void operator delete(void *);
    ~gcDoObjectLoad(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoObjectLoadvirtualtable[];
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char gcDesEnum_vtbl_a[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesObj_vtbl[] asm("__0dPgcDesiredObjectG__vtbl");

inline void gcDoObjectLoad::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oOgcDoObjectLoaddtv, 0x288\n");

gcDoObjectLoad::~gcDoObjectLoad(void) {
    *(void **)((char *)this + 4) = gcDoObjectLoadvirtualtable;
    char *p20 = (char *)this + 0x20;
    char *p1C = (char *)this + 0x1C;
    char *p18 = (char *)this + 0x18;
    char *p14 = (char *)this + 0x14;

    if ((void *)p20 != 0) {
        *(void **)((char *)this + 0x24) = gcDesEnt_vtbl;

        if ((void *)((char *)this + 0x34) != 0) {
            *(void **)((char *)this + 0x38) = gcDesEnum_vtbl_a;

            if ((void *)((char *)this + 0x48) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x48);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x48) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x38) = cBase_vtbl_a;
        }

        *(void **)((char *)this + 0x24) = gcDesObj_vtbl;

        if ((void *)((char *)this + 0x28) != 0) {
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
        *(void **)((char *)this + 0x24) = cBase_vtbl_b;
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

    if ((void *)p18 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x18) = 0;
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
