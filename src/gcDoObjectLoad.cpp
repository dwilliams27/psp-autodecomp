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

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern const char gcDoObjectLoad_base_name[];
extern const char gcDoObjectLoad_base_desc[];

class gcDoObjectLoad : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoObjectLoad;

const cType *gcDoObjectLoad::GetType(void) const {
    if (!type_gcDoObjectLoad) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoObjectLoad_base_name, gcDoObjectLoad_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoObjectLoad = cType::InitializeType(
            0, 0, 0x24A, type_action, gcDoObjectLoad::New, 0, 0, 0);
    }
    return type_gcDoObjectLoad;
}

void gcDoObjectLoad::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);
    wb.Write(((const int *)this)[3]);
    ((const cHandle *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);

    char *typeInfo = *(char **)((const char *)this + 0x24);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (void *)((const char *)this + 0x20);
    rec->fn((char *)base + off, wb._file);

    wb.End();
}
