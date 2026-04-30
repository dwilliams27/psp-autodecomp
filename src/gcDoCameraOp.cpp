// gcDoCameraOp.cpp - gcAll_psp.obj

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
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mField0;

    void Write(cWriteBlock &) const;
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

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoCameraOpvirtualtable[];

class gcDoCameraOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    gcDoCameraOp &operator=(const gcDoCameraOp &);
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoCameraOp asm("D_0009F5C8");

cBase *gcDoCameraOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraOp *result = 0;
    gcDoCameraOp *obj = (gcDoCameraOp *)entry->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoCameraOpvirtualtable;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);
        *(int *)((char *)obj + 0x44) = 0;

        int encoded = (int)obj | 1;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        *(int *)((char *)obj + 0x4C) = encoded;
        *(int *)((char *)obj + 0x50) = encoded;
        *(int *)((char *)obj + 0x54) = -1;
        *(int *)((char *)obj + 0x58) = 0;
        *(int *)((char *)obj + 0x5C) = 0;

        char *desired = (char *)obj + 0x60;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x64) = D_00000338;

        char *helper = (char *)obj + 0x6C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x64) = (void *)0x388A48;
        *(void **)((char *)obj + 0x78) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x74) = desired;
        *(void **)((char *)obj + 0x78) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x7C) = one;
        *(unsigned char *)((char *)obj + 0x7D) = 0;
        *(int *)((char *)obj + 0x80) = 0;
        *(int *)((char *)obj + 0x84) = 0;
        *(int *)((char *)obj + 0x88) = (int)desired | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoCameraOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoCameraOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoCameraOp = cType::InitializeType(
                0, 0, 0x24D, type_action,
                gcDoCameraOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoCameraOp;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoCameraOp *)copy;
}

void gcDoCameraOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    wb.Write(*(int *)((const char *)this + 0x44));
    wb.Write(*(bool *)((const char *)this + 0x48));
    ((const gcDesiredValue *)((const char *)this + 0x4C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x50))->Write(wb);
    wb.Write(*(unsigned int *)((const char *)this + 0x54));
    wb.Write(*(unsigned int *)((const char *)this + 0x58));
    ((const cHandle *)((const char *)this + 0x5C))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x64);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x60;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.End();
}
