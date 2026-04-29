class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

extern const char gcDoSetObjectRelationship_base_name[];
extern const char gcDoSetObjectRelationship_base_desc[];

class gcDoSetObjectRelationship : public gcAction {
public:
    int mFieldC;
    int mField10;

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoSetObjectRelationship;

const cType *gcDoSetObjectRelationship::GetType(void) const {
    if (!type_gcDoSetObjectRelationship) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoSetObjectRelationship_base_name,
                        gcDoSetObjectRelationship_base_desc, 1, 0, 0, 0, 0,
                        0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoSetObjectRelationship = cType::InitializeType(
            0, 0, 0x138, type_action, gcDoSetObjectRelationship::New, 0, 0, 0);
    }
    return type_gcDoSetObjectRelationship;
}

void gcDoSetObjectRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    int value = ((const int *)this)[3];
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

    int value2 = ((const int *)this)[4];
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
