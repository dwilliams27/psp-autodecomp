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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntitySetPrimaryControllervirtualtable[];

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

cBase *gcDoEntitySetPrimaryController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySetPrimaryController *result = 0;
    gcDoEntitySetPrimaryController *obj =
        (gcDoEntitySetPrimaryController *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntitySetPrimaryControllervirtualtable;

        char *desired = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[4] = D_00000338;

        char *helper = (char *)obj + 0x18;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[4] = (void *)0x388A48;
        ((void **)obj)[9] = (void *)0x37E6A8;
        ((void **)obj)[8] = desired;
        ((void **)obj)[9] = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x28) = one;
        *(unsigned char *)((char *)obj + 0x29) = 0;
        *(int *)((char *)obj + 0x2C) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = (int)desired | 1;
        *(int *)((char *)obj + 0x38) = (int)obj | 1;
        *(unsigned char *)((char *)obj + 0x3C) = 0;
        *(unsigned char *)((char *)obj + 0x3D) = 0;
        result = obj;
    }
    return (cBase *)result;
}

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
