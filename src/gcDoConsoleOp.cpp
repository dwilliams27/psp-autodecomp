class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    unsigned int mValue;

    void Write(cWriteBlock &) const;
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoConsoleOp : public gcAction {
public:
    int mFieldC;
    int mField10;
    gcDesiredValue mField14;
    gcDesiredValue mField18;
    unsigned int mField1C;
    unsigned int mField20;

    gcDoConsoleOp(cBase *);

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoConsoleOp &operator=(const gcDoConsoleOp &);
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

extern const char gcDoConsoleOp_base_name[] asm("D_0036D894");
extern const char gcDoConsoleOp_base_desc[] asm("D_0036D89C");
extern char gcDoConsoleOpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoConsoleOp asm("D_0009F5D0");

gcDoConsoleOp::gcDoConsoleOp(cBase *parent) : gcAction(parent) {
    mVTable = gcDoConsoleOpvirtualtable;
    mFieldC = 0;
    mField10 = 0;
    unsigned int tagged = (unsigned int)this | 1;
    mField14.mValue = tagged;
    mField18.mValue = tagged;
    mField1C = tagged;
    mField20 = tagged;
}

const cType *gcDoConsoleOp::GetType(void) const {
    if (!type_gcDoConsoleOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoConsoleOp_base_name, gcDoConsoleOp_base_desc, 1,
                        0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoConsoleOp = cType::InitializeType(
            0, 0, 0x1DF, type_action, gcDoConsoleOp::New, 0, 0, 0);
    }
    return type_gcDoConsoleOp;
}

void gcDoConsoleOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoConsoleOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoConsoleOp_base_name, gcDoConsoleOp_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoConsoleOp = cType::InitializeType(
                0, 0, 0x1DF, type_action, gcDoConsoleOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoConsoleOp;
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
    *this = *(const gcDoConsoleOp *)copy;
}

void gcDoConsoleOp::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    gcAction::Write(file);
    wb.Write(mFieldC);
    mField14.Write(wb);
    wb.Write(mField10);
    mField18.Write(wb);

    int value = mField1C;
    int flag = 0;
    int tagged = value & 1;
    if (tagged != 0) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);

    int value2 = mField20;
    int flag2 = 0;
    if (value2 & 1) {
        flag2 = 1;
    }
    cBase *ptr2;
    if (flag2 != 0) {
        ptr2 = 0;
    } else {
        ptr2 = (cBase *)value2;
    }
    wb.WriteBase(ptr2);
    wb.End();
}
