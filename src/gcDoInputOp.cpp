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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDoInputOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_000006F8[];
extern char gcDoInputOpvirtualtable[];
extern const char gcDoInputOp_base_name[] asm("D_0036D894");
extern const char gcDoInputOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoInputOp asm("D_0042D1E8");

cBase *gcDoInputOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoInputOp *result = 0;
    gcDoInputOp *obj = (gcDoInputOp *)entry->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoInputOpvirtualtable;

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
        *(int *)((char *)obj + 0x38) = one;
        *(int *)((char *)obj + 0x3C) = 0;
        *(int *)((char *)obj + 0x40) = 0;
        *(int *)((char *)obj + 0x44) = 0;
        int mode = 0xE;
        *(int *)((char *)obj + 0x48) = 0;
        int encodedObj = (int)obj | 1;
        *(int *)((char *)obj + 0x4C) = mode;
        *(int *)((char *)obj + 0x50) = encodedObj;
        *(int *)((char *)obj + 0x54) = encodedObj;
        *(int *)((char *)obj + 0x58) = encodedObj;

        gcDesiredObject_gcDesiredObject((char *)obj + 0x5C, (cBase *)obj);

        ((void **)obj)[0x60 / 4] = D_000006F8;
        *(int *)((char *)obj + 0x68) = 7;
        *(int *)((char *)obj + 0x6C) = 0;
        ((void **)obj)[0x60 / 4] = (void *)0x3898A0;
        result = obj;
    }
    return (cBase *)result;
}

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
