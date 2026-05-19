#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
};

class cFileHandle;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cName {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

extern const char gcDoEntityAttach_base_name[];
extern const char gcDoEntityAttach_base_desc[];
extern char gcDoEntityAttachvirtualtable[];
extern char D_00000338[];
extern char gcDesEnt_vtbl_a[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesEnt_vtbl_b[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesEnumEntry_vtbl_a[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnumEntry_vtbl_b[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesObj_vtbl_a[] asm("__0dPgcDesiredObjectG__vtbl");
extern char gcDesObj_vtbl_b[] asm("__0dPgcDesiredObjectG__vtbl");
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_d[] asm("__0dFcBaseG__vtbl");

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
int gcAction_Read(void *, cFile &, cMemPool *)
    __asm__("__0fIgcActionEReadR6FcFileP6IcMemPool");
void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
void cFile_SetCurrentPos(void *, unsigned int);
void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
void __0oKcReadBlockdtv(void *, int);
}

class gcDoEntityAttach : public gcAction {
public:
    static void operator delete(void *);
    ~gcDoEntityAttach(void);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoEntityAttach &operator=(const gcDoEntityAttach &);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttach;

inline void gcDoEntityAttach::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

cBase *gcDoEntityAttach::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityAttach *result = 0;
    gcDoEntityAttach *obj =
        (gcDoEntityAttach *)entry->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)gcDoEntityAttachvirtualtable;

        void *desired = (char *)obj + 0x10;
        obj_i[3] = 0;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[5] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 1, 0, 0);

        obj_i[5] = 0x388A48;
        obj_i[10] = 0x37E6A8;
        obj_i[9] = (int)desired;
        obj_i[10] = 0x388568;
        ((char *)obj)[0x2C] = 1;
        ((char *)obj)[0x2D] = 0;
        obj_i[12] = 0;
        obj_i[13] = 0;
        obj_i[14] = (int)desired | 1;

        desired = (char *)obj + 0x3C;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[16] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x48, 0, 0, 0);

        obj_i[21] = 0x37E6A8;
        obj_i[16] = 0x388A48;
        obj_i[20] = (int)desired;
        obj_i[21] = 0x388568;
        ((char *)obj)[0x58] = 1;
        ((char *)obj)[0x59] = 0;
        obj_i[23] = 0;
        obj_i[24] = 0;
        obj_i[25] = (int)desired | 1;
        *(short *)((char *)obj + 0x7C) = 0;
        *(short *)((char *)obj + 0x7E) = 0;
        int encoded = (int)obj | 1;
        ((char *)obj)[0x68] = 0;
        obj_i[32] = encoded;
        obj_i[33] = encoded;

        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntityAttach::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityAttach) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityAttach = cType::InitializeType(
                0, 0, 0x156, type_action, gcDoEntityAttach::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityAttach;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoEntityAttach *)copy;
}

const cType *gcDoEntityAttach::GetType(void) const {
    if (!type_gcDoEntityAttach) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityAttach_base_name,
                        gcDoEntityAttach_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttach = cType::InitializeType(
            0, 0, 0x156, type_action, gcDoEntityAttach::New, 0, 0, 0);
    }
    return type_gcDoEntityAttach;
}

void gcDoEntityAttach::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x10;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const cName *)((const char *)this + 0x68))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x80))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);
    wb.End();
}

int gcDoEntityAttach::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if (rb[3] != 5 || gcAction_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);

    {
        char *base = (char *)this + 0x10;
        char *typeInfo = *(char **)((char *)this + 0x14);
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    {
        char *base = (char *)this + 0x3C;
        char *typeInfo = *(char **)((char *)this + 0x40);
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    ((cName *)((char *)this + 0x68))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x80))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x84))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oQgcDoEntityAttachdtv, 0x32c\n");

gcDoEntityAttach::~gcDoEntityAttach(void) {
    *(void **)((char *)this + 4) = gcDoEntityAttachvirtualtable;
    char *p84 = (char *)this + 0x84;
    char *p80 = (char *)this + 0x80;
    char *p3C = (char *)this + 0x3C;
    char *p10 = (char *)this + 0x10;

    if ((void *)p84 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x84);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x84) = 0;
            }
        }
    }

    if ((void *)p80 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x80);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x80) = 0;
            }
        }
    }

    if ((void *)p3C != 0) {
        *(void **)((char *)this + 0x40) = gcDesEnt_vtbl_a;

        if ((void *)((char *)this + 0x50) != 0) {
            *(void **)((char *)this + 0x54) = gcDesEnumEntry_vtbl_a;

            if ((void *)((char *)this + 0x64) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x64);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0) {
                    if (val != 0) {
                        char *typeInfo = *(char **)(val + 4);
                        DtorDeleteRecord *slot =
                            (DtorDeleteRecord *)(typeInfo + 0x50);
                        slot->fn((char *)val + slot->offset, (void *)3);
                        *(int *)((char *)this + 0x64) = 0;
                    }
                }
            }
            *(void **)((char *)this + 0x54) = cBase_vtbl_a;
        }

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
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x44) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x40) = cBase_vtbl_b;
    }

    if ((void *)p10 != 0) {
        *(void **)((char *)this + 0x14) = gcDesEnt_vtbl_b;

        if ((void *)((char *)this + 0x24) != 0) {
            *(void **)((char *)this + 0x28) = gcDesEnumEntry_vtbl_b;

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
            *(void **)((char *)this + 0x28) = cBase_vtbl_c;
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

        *(void **)((char *)this + 0x14) = cBase_vtbl_d;
    }

    gcAction_dtor(this, 0);
}
