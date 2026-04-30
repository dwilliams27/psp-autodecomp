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

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoInputOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern const char gcDoInputOp_base_name[] asm("D_0036D894");
extern const char gcDoInputOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoInputOp asm("D_0042D1E8");

const cType *gcDoInputOp::GetType(void) const {
    if (!type_gcDoInputOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoInputOp_base_name, gcDoInputOp_base_desc, 1, 0, 0,
                        0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoInputOp = cType::InitializeType(
            0, 0, 0x211, type_action, gcDoInputOp::New, 0, 0, 0);
    }
    return type_gcDoInputOp;
}

void gcDoInputOp::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    wb.Write(*(int *)((const char *)this + 0x38));
    wb.Write(*(int *)((const char *)this + 0x3C));
    wb.Write(*(int *)((const char *)this + 0x40));
    wb.Write(*(int *)((const char *)this + 0x44));
    wb.Write(*(int *)((const char *)this + 0x48));
    wb.Write(*(int *)((const char *)this + 0x4C));

    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x54))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x58))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x60);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x5C;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.End();
}
