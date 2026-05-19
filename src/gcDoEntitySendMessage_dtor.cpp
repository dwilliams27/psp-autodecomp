// gcDoEntitySendMessage_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcDoEntitySendMessage is also defined in
// src/gcDoEntitySendMessage.cpp (and other split TUs). This TU redeclares
// only the destructor + operator delete so the existing matched methods
// keep their current codegen.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

class gcDoEntitySendMessage {
public:
    static void operator delete(void *);
    ~gcDoEntitySendMessage(void);
};

extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoEntitySendMessagevirtualtable[];
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");
extern char gcDesEnum_vtbl_a[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnum_vtbl_b[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnt_vtbl[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesObj_vtbl[] asm("__0dPgcDesiredObjectG__vtbl");

inline void gcDoEntitySendMessage::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oVgcDoEntitySendMessagedtv, 0x26c\n");

gcDoEntitySendMessage::~gcDoEntitySendMessage(void) {
    *(void **)((char *)this + 4) = gcDoEntitySendMessagevirtualtable;
    char *p58 = (char *)this + 0x58;
    char *p50 = (char *)this + 0x50;
    char *p38 = (char *)this + 0x38;
    char *p0C = (char *)this + 0x0C;

    if ((void *)p58 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x58);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x58) = 0;
            }
        }
    }

    if ((void *)p50 != 0) {
        ((cBaseArray *)p50)->RemoveAll();
    }

    if ((void *)p38 != 0) {
        *(void **)((char *)this + 0x3C) = gcDesEnum_vtbl_a;

        if ((void *)((char *)this + 0x4C) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x4C);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x4C) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x3C) = cBase_vtbl_a;
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
