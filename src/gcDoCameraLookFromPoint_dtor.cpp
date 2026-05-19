// gcDoCameraLookFromPoint_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcDoCameraLookFromPoint is also defined in
// src/gcDoCameraLookFromPoint.cpp. This TU redeclares only the destructor and
// operator delete so the already-matched methods keep their current codegen.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoCameraLookFromPoint {
public:
    static void operator delete(void *);
    ~gcDoCameraLookFromPoint(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");
extern "C" void gcDesiredCamera_dtor(void *, int) asm("__0oPgcDesiredCameradtv");

extern char D_000018C8[];
extern char D_00001B68[];
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");
extern char gcDesiredEntity_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesiredEnumerationEntry_vtbl[]
    asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesiredObject_vtbl[] asm("__0dPgcDesiredObjectG__vtbl");

inline void gcDoCameraLookFromPoint::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oXgcDoCameraLookFromPointdtv, 0x26c\n");

gcDoCameraLookFromPoint::~gcDoCameraLookFromPoint(void) {
    *(void **)((char *)this + 4) = D_00001B68;
    char *p60 = (char *)this + 0x60;
    char *p4C = (char *)this + 0x4C;
    char *p44 = (char *)this + 0x44;

    if ((void *)p60 != 0) {
        *(void **)((char *)this + 0x64) = gcDesiredEntity_vtbl;

        if ((void *)((char *)this + 0x74) != 0) {
            *(void **)((char *)this + 0x78) = gcDesiredEnumerationEntry_vtbl;

            if ((void *)((char *)this + 0x88) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x88);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot =
                            (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x88) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x78) = cBase_vtbl_a;
        }

        *(void **)((char *)this + 0x64) = gcDesiredObject_vtbl;

        if ((void *)((char *)this + 0x68) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x68);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x68) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x64) = cBase_vtbl_b;
    }

    if ((void *)p4C != 0) {
        *(void **)((char *)this + 0x50) = gcDesiredObject_vtbl;

        if ((void *)((char *)this + 0x54) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x54);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x54) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x50) = cBase_vtbl_c;
    }

    *(void **)((char *)this + 4) = D_000018C8;

    if ((void *)p44 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x44);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot =
                    (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x44) = 0;
            }
        }
    }

    gcDesiredCamera_dtor((char *)this + 0x0C, 2);
    gcAction_dtor(this, 0);
}
