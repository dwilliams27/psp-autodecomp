// ODR-WARNING: split translation unit for gcDoEntityFindAttachedBase destructor
// only. The matched Read/Write/GetType bodies live in
// src/gcDoEntityFindAttachedBase.cpp; adding the destructor declaration there
// perturbs Write register allocation.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoEntityFindAttachedBase {
public:
    static void operator delete(void *);
    ~gcDoEntityFindAttachedBase(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");
extern "C" void free(void *);

extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesEnumEntry_vtbl[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesObj_vtbl_a[] asm("__0dPgcDesiredObjectG__vtbl");
extern char gcDesObj_vtbl_b[] asm("__0dPgcDesiredObjectG__vtbl");
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");

inline void gcDoEntityFindAttachedBase::operator delete(void *ptr) {
    if (ptr != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            char *entries = *(char **)((char *)block + 0x1C);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
            slot->fn((char *)block + slot->offset, ptr);
        } else {
            free(ptr);
        }
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oagcDoEntityFindAttachedBasedtv, 0x310\n");

gcDoEntityFindAttachedBase::~gcDoEntityFindAttachedBase(void) {
    *(void **)((char *)this + 4) = (void *)0x38BCA8;
    char *data = (char *)this;
    char *p60 = (char *)this + 0x60;
    char *p48 = (char *)this + 0x48;
    char *p10 = (char *)this + 0x10;

    if ((void *)p60 != 0) {
        data = *(char **)((char *)this + 0x60);
        int count = 0;
        if (data != 0) {
            count = *(int *)(data - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *slot =
                    (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
                slot->fn(block + slot->offset, data);
            }
            *(int *)((char *)this + 0x60) = 0;
        }
    }

    if ((void *)p48 != 0) {
        *(void **)((char *)this + 0x4C) = (void *)0x389508;

        if ((void *)((char *)this + 0x5C) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x5C);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x5C) = 0;
                }
            }
        }

        *(void **)((char *)this + 0x4C) = gcDesObj_vtbl_a;

        if ((void *)((char *)this + 0x50) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x50);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x50) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x4C) = cBase_vtbl_a;
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
                        DtorDeleteRecord *slot =
                            (DtorDeleteRecord *)(typeInfo + 0x50);
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
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x18) = 0;
                }
            }
        }

        *(void **)((char *)this + 0x14) = cBase_vtbl_c;
    }

    gcAction_dtor(this, 0);
}
