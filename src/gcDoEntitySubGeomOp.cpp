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

    void Write(cFile &) const;
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
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySubGeomOp asm("D_0009F674");

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
