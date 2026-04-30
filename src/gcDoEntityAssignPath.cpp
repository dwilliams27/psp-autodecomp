// gcDoEntityAssignPath.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

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
    void Write(unsigned int);
    void Write(float);
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

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityAssignPath : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityAssignPath asm("D_0009F614");

const cType *gcDoEntityAssignPath::GetType(void) const {
    if (!type_gcDoEntityAssignPath) {
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
        type_gcDoEntityAssignPath = cType::InitializeType(
            0, 0, 0x7D, type_action, gcDoEntityAssignPath::New, 0, 0, 0);
    }
    return type_gcDoEntityAssignPath;
}

void gcDoEntityAssignPath::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x0C));

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x10);
        rec->fn((char *)base + off, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x3C);
        rec->fn((char *)base + off, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x54))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x5C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (void *)((const char *)this + 0x58);
        rec->fn((char *)base + off, wb._file);
    }

    wb.Write(*(const float *)((const char *)this + 0x74));
    wb.End();
}
