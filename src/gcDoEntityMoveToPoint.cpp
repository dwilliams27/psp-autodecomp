class cBase;
class cFile;
class cMemPool;

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
    void Write(bool);
    void Write(unsigned int);
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

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityMoveToPoint : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityMoveToPoint asm("D_0009F620");

const cType *gcDoEntityMoveToPoint::GetType(void) const {
    if (!type_gcDoEntityMoveToPoint) {
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
        type_gcDoEntityMoveToPoint = cType::InitializeType(
            0, 0, 0xDC, type_action, gcDoEntityMoveToPoint::New, 0, 0, 0);
    }
    return type_gcDoEntityMoveToPoint;
}

void gcDoEntityMoveToPoint::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x3C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x38;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x4C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0xB0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC8))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0xD8));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE4));
    ((const cName *)((const char *)this + 0xE8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x104));
    wb.Write(*(const unsigned int *)((const char *)this + 0xDC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE0));
    ((const gcDesiredValue *)((const char *)this + 0xBC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD4))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x7C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x78;
        rec->fn(base + rec->offset, wb._file);
    }

    wb.End();
}
