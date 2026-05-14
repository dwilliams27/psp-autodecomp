// gcDoEntityActivatePartialController_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcDoEntityActivatePartialController is also defined in
// src/gcDoEntityActivatePartialController.cpp. This TU redeclares only the
// destructor and operator delete so existing matched methods keep their
// current codegen.

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

class gcDoEntityActivatePartialController {
public:
    static void operator delete(void *);
    ~gcDoEntityActivatePartialController(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoEntityActivatePartialControllervirtualtable[];
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");
extern char gcDesEnum_vtbl_a[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnum_vtbl_b[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesObj_vtbl[] asm("__0dPgcDesiredObjectG__vtbl");

inline void gcDoEntityActivatePartialController::operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0ojgcDoEntityActivatePartialControllerdtv, 0x1f0\n");

gcDoEntityActivatePartialController::~gcDoEntityActivatePartialController(void) {
    *(void **)((char *)this + 4) = gcDoEntityActivatePartialControllervirtualtable;
    char *p0C = (char *)this + 0x0C;

    if ((void *)((char *)this + 0x3C) != 0) {
        *(void **)((char *)this + 0x40) = gcDesEnum_vtbl_a;

        if ((void *)((char *)this + 0x50) != 0) {
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
        *(void **)((char *)this + 0x40) = cBase_vtbl_a;
    }

    if ((void *)p0C != 0) {
        *(void **)((char *)this + 0x10) = gcDesEnt_vtbl;

        if ((void *)((char *)this + 0x20) != 0) {
            *(void **)((char *)this + 0x24) = gcDesEnum_vtbl_b;

            if ((void *)((char *)this + 0x34) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x34);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x34) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x24) = cBase_vtbl_b;
        }

        *(void **)((char *)this + 0x10) = gcDesObj_vtbl;

        if ((void *)((char *)this + 0x14) != 0) {
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
        *(void **)((char *)this + 0x10) = cBase_vtbl_c;
    }

    gcAction_dtor(this, 0);
}
