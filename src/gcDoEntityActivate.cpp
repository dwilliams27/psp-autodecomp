// gcDoEntityActivate.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *file;
    int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
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
    cBase *owner;
    void *vtable;
    unsigned int next;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern const char gcDoEntityActivate_base_name[];
extern const char gcDoEntityActivate_base_desc[];

class gcDoEntityActivate : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityActivate;

const cType *gcDoEntityActivate::GetType(void) const {
    if (!type_gcDoEntityActivate) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityActivate_base_name,
                        gcDoEntityActivate_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityActivate = cType::InitializeType(
            0, 0, 0xC3, type_action, gcDoEntityActivate::New, 0, 0, 0);
    }
    return type_gcDoEntityActivate;
}

void gcDoEntityActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb.file);
    }

    wb.Write(*(const unsigned int *)((const char *)this + 0x38));
    wb.Write(*(const int *)((const char *)this + 0x40));
    ((const gcDesiredValue *)((const char *)this + 0x44))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x3C));
    wb.End();
}
