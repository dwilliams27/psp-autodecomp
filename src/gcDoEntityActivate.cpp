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
extern char gcDoEntityActivatevirtualtable[];

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

cBase *gcDoEntityActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityActivate *result = 0;
    gcDoEntityActivate *obj =
        (gcDoEntityActivate *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityActivatevirtualtable;

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
        *(int *)((char *)obj + 0x38) = 0;
        *(int *)((char *)obj + 0x3C) = one;
        *(int *)((char *)obj + 0x40) = 0;
        *(int *)((char *)obj + 0x44) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

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
