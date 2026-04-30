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
extern char gcDoEntityActivatePartialControllervirtualtable[];

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

cBase *gcDoEntityActivatePartialController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityActivatePartialController *result = 0;
    gcDoEntityActivatePartialController *obj =
        (gcDoEntityActivatePartialController *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityActivatePartialControllervirtualtable;

        char *desired = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[4] = D_00000338;

        char *helper = (char *)obj + 0x18;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[4] = (void *)0x388A48;
        void *baseType = (void *)0x37E6A8;
        ((void **)obj)[9] = baseType;
        ((void **)obj)[8] = desired;
        void *desiredVtable = (void *)0x388568;
        ((void **)obj)[9] = desiredVtable;
        *(unsigned char *)((char *)obj + 0x28) = one;
        *(unsigned char *)((char *)obj + 0x29) = 0;
        *(int *)((char *)obj + 0x2C) = 0;
        int encodedDesired = (int)desired | 1;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = encodedDesired;
        __asm__ volatile("" ::: "memory");
        *(void **)((char *)obj + 0x40) = baseType;
        *(int *)((char *)obj + 0x38) = 0;
        *(void **)((char *)obj + 0x3C) = obj;
        *(void **)((char *)obj + 0x40) = desiredVtable;
        *(unsigned char *)((char *)obj + 0x44) = one;
        *(unsigned char *)((char *)obj + 0x45) = 0;
        *(int *)((char *)obj + 0x48) = 0;
        int encodedObj = (int)obj | 1;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x4C) = 0;
        *(int *)((char *)obj + 0x50) = encodedObj;
        result = obj;
    }
    return (cBase *)result;
}

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
