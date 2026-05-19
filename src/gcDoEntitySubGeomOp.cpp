class cBase;
class cFile;
class cMemPool;
class cType;

class gcExpression {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoEntitySubGeomOp : public gcAction {
public:
    gcDoEntitySubGeomOp(cBase *);
    static void operator delete(void *);
    ~gcDoEntitySubGeomOp(void);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoEntitySubGeomOp &operator=(const gcDoEntitySubGeomOp &);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern char D_00000338[];
extern char gcDoEntitySubGeomOpvirtualtable[];
extern char gcDesEnt_vtbl_a[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesEnt_vtbl_b[] asm("__0dPgcDesiredEntityG__vtbl");
extern char gcDesEnumEntry_vtbl_a[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesEnumEntry_vtbl_b[] asm("__0dZgcDesiredEnumerationEntryG__vtbl");
extern char gcDesObj_vtbl_a[] asm("__0dPgcDesiredObjectG__vtbl");
extern char gcDesObj_vtbl_b[] asm("__0dPgcDesiredObjectG__vtbl");
extern char gcDesObj_vtbl_c[] asm("__0dPgcDesiredObjectG__vtbl");
extern char cBase_vtbl_a[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_b[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_c[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_d[] asm("__0dFcBaseG__vtbl");
extern char cBase_vtbl_e[] asm("__0dFcBaseG__vtbl");
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySubGeomOp asm("D_0009F674");

inline void gcDoEntitySubGeomOp::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

void gcDoEntitySubGeomOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntitySubGeomOp) {
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
            type_gcDoEntitySubGeomOp = cType::InitializeType(
                0, 0, 0x1C0, type_action,
                gcDoEntitySubGeomOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntitySubGeomOp;
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
    *this = *(const gcDoEntitySubGeomOp *)copy;
}

gcDoEntitySubGeomOp::gcDoEntitySubGeomOp(cBase *parent) : gcAction(parent) {
    mVTable = gcDoEntitySubGeomOpvirtualtable;
    *(int *)((char *)this + 0x0C) = 0;

    char *desired0 = (char *)this + 0x10;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);
    *(void **)((char *)this + 0x14) = D_00000338;
    void *helper0 = (char *)this + 0x1C;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper0, 1, 0, 0);

    void *helperDesc = (void *)0x388A48;
    *(void **)((char *)this + 0x14) = helperDesc;
    *(void **)((char *)this + 0x24) = desired0;
    void *desiredVTable = (void *)0x388568;
    *(void **)((char *)this + 0x28) = desiredVTable;
    *(unsigned char *)((char *)this + 0x2C) = one;
    *(unsigned char *)((char *)this + 0x2D) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(int *)((char *)this + 0x34) = 0;
    *(int *)((char *)this + 0x38) = (int)desired0 | 1;
    int taggedThis = (int)this | 1;
    *(int *)((char *)this + 0x3C) = taggedThis;

    char *desired1 = (char *)this + 0x40;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x44) = D_00000338;
    gcDesiredEntityHelper_ctor((char *)this + 0x4C, 1, 0, 0);

    *(void **)((char *)this + 0x44) = helperDesc;
    *(void **)((char *)this + 0x54) = desired1;
    *(void **)((char *)this + 0x58) = desiredVTable;
    *(unsigned char *)((char *)this + 0x5C) = one;
    *(unsigned char *)((char *)this + 0x5D) = 0;
    *(int *)((char *)this + 0x60) = 0;
    *(int *)((char *)this + 0x64) = 0;
    *(int *)((char *)this + 0x68) = (int)desired1 | 1;

    gcDesiredObject_gcDesiredObject((char *)this + 0x6C, (cBase *)this);
    *(int *)((char *)this + 0x78) = 7;
    *(int *)((char *)this + 0x7C) = 0;
    *(void **)((char *)this + 0x70) = (void *)0x389760;
    *(short *)((char *)this + 0x94) = 0;
    *(short *)((char *)this + 0x96) = 0;
    *(unsigned char *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x98) = taggedThis;
    *(int *)((char *)this + 0x9C) = taggedThis;
}

const cType *gcDoEntitySubGeomOp::GetType(void) const {
    if (!type_gcDoEntitySubGeomOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntitySubGeomOp = cType::InitializeType(
            0, 0, 0x1C0, type_action, gcDoEntitySubGeomOp::New, 0, 0, 0);
    }
    return type_gcDoEntitySubGeomOp;
}

void gcDoEntitySubGeomOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    {
        char *typeInfo0 = *(char **)((const char *)this + 0x14);
        WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
        short off0 = rec0->offset;
        void *base0 = (char *)this + 0x10;
        rec0->fn((char *)base0 + off0, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x3C))->Write(wb);

    {
        char *typeInfo1 = *(char **)((const char *)this + 0x70);
        WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
        short off1 = rec1->offset;
        void *base1 = (char *)this + 0x6C;
        rec1->fn((char *)base1 + off1, wb._file);
    }

    {
        char *typeInfo2 = *(char **)((const char *)this + 0x44);
        WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
        short off2 = rec2->offset;
        void *base2 = (char *)this + 0x40;
        rec2->fn((char *)base2 + off2, wb._file);
    }

    ((const cName *)((const char *)this + 0x80))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x0C));
    ((const gcDesiredValue *)((const char *)this + 0x98))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x9C))->Write(wb);
    wb.End();
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oTgcDoEntitySubGeomOpdtv, 0x400\n");

gcDoEntitySubGeomOp::~gcDoEntitySubGeomOp(void) {
    *(void **)((char *)this + 4) = gcDoEntitySubGeomOpvirtualtable;
    char *p9C = (char *)this + 0x9C;
    char *p98 = (char *)this + 0x98;
    char *p6C = (char *)this + 0x6C;
    char *p40 = (char *)this + 0x40;
    char *p3C = (char *)this + 0x3C;
    char *p10 = (char *)this + 0x10;

    if ((void *)p9C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x9C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x9C) = 0;
            }
        }
    }

    if ((void *)p98 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x98);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x98) = 0;
            }
        }
    }

    if ((void *)p6C != 0) {
        *(void **)((char *)this + 0x70) = gcDesObj_vtbl_a;

        if ((void *)((char *)this + 0x74) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x74);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x74) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x70) = cBase_vtbl_a;
    }

    if ((void *)p40 != 0) {
        *(void **)((char *)this + 0x44) = gcDesEnt_vtbl_a;

        if ((void *)((char *)this + 0x54) != 0) {
            *(void **)((char *)this + 0x58) = gcDesEnumEntry_vtbl_a;

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
            *(void **)((char *)this + 0x58) = cBase_vtbl_b;
        }

        *(void **)((char *)this + 0x44) = gcDesObj_vtbl_b;

        if ((void *)((char *)this + 0x48) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x48);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot =
                        (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x48) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x44) = cBase_vtbl_c;
    }

    if ((void *)p3C != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x3C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x3C) = 0;
            }
        }
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
            *(void **)((char *)this + 0x28) = cBase_vtbl_d;
        }

        *(void **)((char *)this + 0x14) = gcDesObj_vtbl_c;

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
        *(void **)((char *)this + 0x14) = cBase_vtbl_e;
    }

    gcAction_dtor(this, 0);
}
