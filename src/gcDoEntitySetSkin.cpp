// gcDoEntitySetSkin.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoEntitySetSkin : public gcAction {
public:
    gcDoEntitySetSkin(cBase *);

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoEntitySetSkin &operator=(const gcDoEntitySetSkin &);
};

class gcExpressionList {
public:
    gcExpressionList(cBase *);
};

class gcDoSwitch : public gcAction {
public:
    gcDoSwitch(cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
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

extern const char gcDoEntitySetSkin_base_name[];
extern const char gcDoEntitySetSkin_base_desc[];

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcExpressionList_gcExpressionList(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntitySetSkinvirtualtable[];
extern char gcDoSwitchvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySetSkin asm("D_0009F664");

gcDoEntitySetSkin::gcDoEntitySetSkin(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoEntitySetSkinvirtualtable;

    char *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    void *desiredType = D_00000338;
    *(void **)((char *)this + 0x10) = desiredType;

    void *helper0 = (char *)this + 0x18;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper0, 1, 0, 0);

    void *helperDesc = (void *)0x388A48;
    *(void **)((char *)this + 0x10) = helperDesc;
    *(void **)((char *)this + 0x20) = desired0;

    void *desiredVTable = (void *)0x388568;
    *(void **)((char *)this + 0x24) = desiredVTable;
    *(unsigned char *)((char *)this + 0x28) = one;
    *(unsigned char *)((char *)this + 0x29) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(int *)((char *)this + 0x34) = (int)desired0 | 1;

    int selfFlag = (int)this | 1;
    *(int *)((char *)this + 0x38) = 0;
    *(int *)((char *)this + 0x3C) = selfFlag;

    char *desired1 = (char *)this + 0x40;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x44) = desiredType;
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
    *(int *)((char *)this + 0x78) = one;
    *(int *)((char *)this + 0x7C) = 0;
    *(void **)((char *)this + 0x70) = (void *)0x38A548;
    *(int *)((char *)this + 0x80) = selfFlag;
    *(int *)((char *)this + 0x84) = selfFlag;
    *(int *)((char *)this + 0x88) = selfFlag;
}

gcDoSwitch::gcDoSwitch(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoSwitchvirtualtable;
    *(unsigned int *)((char *)this + 0x0C) = (unsigned int)this | 1;
    gcExpressionList_gcExpressionList((char *)this + 0x10, (cBase *)this);
    gcExpressionList_gcExpressionList((char *)this + 0x18, (cBase *)this);
}

cBase *gcDoEntitySetSkin::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetSkin *result = 0;
    gcDoEntitySetSkin *obj = (gcDoEntitySetSkin *)rec->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetSkin(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySetSkin::GetType(void) const {
    if (!type_gcDoEntitySetSkin) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetSkin_base_name,
                        gcDoEntitySetSkin_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetSkin = cType::InitializeType(
            0, 0, 0x134, type_action, gcDoEntitySetSkin::New, 0, 0, 0);
    }
    return type_gcDoEntitySetSkin;
}

void gcDoEntitySetSkin::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntitySetSkin) {
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
            type_gcDoEntitySetSkin = cType::InitializeType(
                0, 0, 0x134, type_action, gcDoEntitySetSkin::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntitySetSkin;
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
    *this = *(const gcDoEntitySetSkin *)copy;
}

void gcDoEntitySetSkin::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x3C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x80))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x44);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x40;
    rec1->fn((char *)base1 + off1, wb._file);

    char *typeInfo2 = *(char **)((const char *)this + 0x70);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x6C;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(const int *)((const char *)this + 0x38));
    ((const gcDesiredValue *)((const char *)this + 0x88))->Write(wb);
    wb.End();
}
