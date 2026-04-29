// gcDoEntityActivatePartialController.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

    void Write(cFile &) const;
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern const char gcDoEntityActivatePartialController_base_name[];
extern const char gcDoEntityActivatePartialController_base_desc[];

class gcDoEntityActivatePartialController : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityActivatePartialController;

const cType *gcDoEntityActivatePartialController::GetType(void) const {
    if (!type_gcDoEntityActivatePartialController) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityActivatePartialController_base_name,
                        gcDoEntityActivatePartialController_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityActivatePartialController = cType::InitializeType(
            0, 0, 0x10A, type_action,
            gcDoEntityActivatePartialController::New, 0, 0, 0);
    }
    return type_gcDoEntityActivatePartialController;
}

void gcDoEntityActivatePartialController::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb.file);
    }

    wb.Write(*(int *)((char *)this + 0x38));

    {
        char *typeInfo = *(char **)((char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb.file);
    }

    wb.End();
}
