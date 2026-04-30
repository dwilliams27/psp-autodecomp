// gcDoReadFile.cpp - decompiled from gcDoReadFile.obj.
//
// Functions:
//   0x001C3FA4 gcDoReadFile::New(cMemPool *, cBase *) static  152B
//   0x001C4154 gcDoReadFile::Write(cFile &) const             148B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    unsigned int mField0;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    short _pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);

extern char gcDoReadFilevirtualtable[];

class gcDoReadFile : public gcAction {
public:
    int mFieldC;
    gcDesiredValue mDesiredValue;
    unsigned int mBase;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    gcDoReadFile &operator=(const gcDoReadFile &);
    void Write(cFile &) const;
};

static cType *type_gcDoReadFile asm("D_000385D0");
static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");

cBase *gcDoReadFile::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoReadFile *result = 0;
    gcDoReadFile *obj = (gcDoReadFile *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoReadFilevirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        *(unsigned int *)((char *)obj + 0x14) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoReadFile::GetType(void) const {
    if (!type_gcDoReadFile) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36C728,
                                                      (const char *)0x36C730,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoReadFile = cType::InitializeType(
            0, 0, 0x2EA, type_action, gcDoReadFile::New, 0, 0, 0);
    }
    return type_gcDoReadFile;
}

void gcDoReadFile::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoReadFile) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36C728,
                                                          (const char *)0x36C730,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoReadFile = cType::InitializeType(
                0, 0, 0x2EA, type_action, gcDoReadFile::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoReadFile;
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
    *this = *(const gcDoReadFile *)copy;
}

void gcDoReadFile::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(mFieldC);
    mDesiredValue.Write(wb);
    int val = mBase;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}
