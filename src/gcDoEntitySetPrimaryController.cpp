class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    unsigned int value;

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

    void Write(cFile &) const;
};

typedef void (*WriteFn)(void *, cFile *);

class gcDoEntitySetPrimaryController : public gcAction {
public:
    unsigned int desiredController;
    cType *desiredControllerType;
    char pad14[0x24];
    gcDesiredValue desiredValue;
    bool field_3C;
    bool field_3D;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

extern const char gcDoEntitySetPrimaryController_base_name[];
extern const char gcDoEntitySetPrimaryController_base_desc[];

void gcDoEntitySetPrimaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    const cTypeMethod *slot = &desiredControllerType->write_m;
    char *base = (char *)this + 0x0C;
    ((WriteFn)slot->fn)(base + slot->offset, wb.file);

    desiredValue.Write(wb);
    wb.Write(field_3D);
    wb.Write(field_3C);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySetPrimaryController;

const cType *gcDoEntitySetPrimaryController::GetType(void) const {
    if (!type_gcDoEntitySetPrimaryController) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetPrimaryController_base_name,
                        gcDoEntitySetPrimaryController_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetPrimaryController = cType::InitializeType(
            0, 0, 0x11D, type_action,
            gcDoEntitySetPrimaryController::New, 0, 0, 0);
    }
    return type_gcDoEntitySetPrimaryController;
}
