// gcDoEntityAssignPath_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcDoEntityAssignPath is also defined in
// src/gcDoEntityAssignPath.cpp. This TU redeclares only the destructor
// and operator delete so existing matched methods keep their current codegen.

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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class gcEvent {
public:
    ~gcEvent(void);
};

class gcDoEntityAssignPath {
public:
    static void operator delete(void *);
    ~gcDoEntityAssignPath(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoEntityAssignPathvirtualtable[];
extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");           // 0x388A48
extern char gcDesEnumEntry_vtbl[] asm("__0dZgcDesiredEnumerationEntryG__vtbl"); // 0x388568
extern char gcDesObj_vtbl_a[] asm("__0dPgcDesiredObjectG__vtbl");         // 0x3889A8
extern char gcDesObj_vtbl_b[] asm("__0dPgcDesiredObjectG__vtbl");         // 0x3889A8
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");                      // 0x37E6A8
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");                      // 0x37E6A8
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");                      // 0x37E6A8

inline void gcDoEntityAssignPath::operator delete(void *ptr) {
    if (ptr != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
        void *block = *(void **)((char *)pool + 0x24);
        char *entries = *(char **)((char *)block + 0x1C);
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
        slot->fn((char *)block + slot->offset, ptr);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oUgcDoEntityAssignPathdtv, 0x2b4\n");

gcDoEntityAssignPath::~gcDoEntityAssignPath(void) {
    *(void **)((char *)this + 4) = gcDoEntityAssignPathvirtualtable;
    ((gcEvent *)((char *)this + 0x58))->~gcEvent();

    char *p54 = (char *)this + 0x54;
    char *p50 = (char *)this + 0x50;
    char *p3C = (char *)this + 0x3C;
    char *p10 = (char *)this + 0x10;

    if ((void *)p54 != 0) {
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

    if ((void *)p50 != 0) {
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

    if ((void *)p3C != 0) {
        *(void **)((char *)this + 0x40) = gcDesObj_vtbl_a;

        if ((void *)((char *)this + 0x44) != 0) {
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
        *(void **)((char *)this + 0x40) = cBase_vtbl_a;
    }

    if ((void *)p10 != 0) {
        *(void **)((char *)this + 0x14) = gcDesEnt_vtbl;

        if ((void *)((char *)this + 0x24) != 0) {
            *(void **)((char *)this + 0x28) = gcDesEnumEntry_vtbl;

            if ((void *)((char *)this + 0x38) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x38);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x38) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x28) = cBase_vtbl_b;
        }

        *(void **)((char *)this + 0x14) = gcDesObj_vtbl_b;

        if ((void *)((char *)this + 0x18) != 0) {
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

        *(void **)((char *)this + 0x14) = cBase_vtbl_c;
    }

    gcAction_dtor(this, 0);
}
