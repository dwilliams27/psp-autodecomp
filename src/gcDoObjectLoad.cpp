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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoObjectLoadvirtualtable[];
extern const char gcDoObjectLoad_base_name[];
extern const char gcDoObjectLoad_base_desc[];

class gcDoObjectLoad : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoObjectLoad &operator=(const gcDoObjectLoad &);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoObjectLoad;

cBase *gcDoObjectLoad::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoObjectLoad *result = 0;
    gcDoObjectLoad *obj =
        (gcDoObjectLoad *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoObjectLoadvirtualtable;
        *(int *)((char *)obj + 0x0C) = 0;
        int encoded = (int)obj | 1;
        *(int *)((char *)obj + 0x10) = 0;
        *(int *)((char *)obj + 0x14) = encoded;
        *(int *)((char *)obj + 0x18) = encoded;
        *(int *)((char *)obj + 0x1C) = encoded;

        char *desired = (char *)obj + 0x20;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[9] = D_00000338;

        char *helper = (char *)obj + 0x2C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[9] = (void *)0x388A48;
        ((void **)obj)[14] = (void *)0x37E6A8;
        ((void **)obj)[13] = desired;
        ((void **)obj)[14] = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x3C) = one;
        *(unsigned char *)((char *)obj + 0x3D) = 0;
        *(int *)((char *)obj + 0x40) = 0;
        *(int *)((char *)obj + 0x44) = 0;
        *(int *)((char *)obj + 0x48) = (int)desired | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoObjectLoad::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoObjectLoad) {
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
            type_gcDoObjectLoad = cType::InitializeType(
                0, 0, 0x24A, type_action, gcDoObjectLoad::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoObjectLoad;
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
    *this = *(const gcDoObjectLoad *)copy;
}

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
