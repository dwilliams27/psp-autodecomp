// gcDoPlayerSetViewport.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction. Class holds two gcDesiredValue subobjects at offsets 0xC and 0x10.
//
// Functions:
//   0x002F29EC gcDoPlayerSetViewport::New(cMemPool *, cBase *) static  148B
//   0x002F2B98 gcDoPlayerSetViewport::Write(cFile &) const             100B

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
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
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

// Local gcAction declaration with full layout matching gcAction.cpp.
class gcAction : public gcExpression {
public:
    cBase *mOwner;            // 0x00
    void *mVTable;            // 0x04
    unsigned int mNext;       // 0x08

    gcAction(cBase *);
    void Write(cFile &) const;
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

struct PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcAction_gcAction(void *, cBase *);

extern char gcDoPlayerSetViewportvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoPlayerSetViewport asm("D_0009F6CC");

class gcDoPlayerSetViewport : public gcAction {
public:
    gcDesiredValue mDesired1;  // 0x0C
    gcDesiredValue mDesired2;  // 0x10

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoPlayerSetViewport &operator=(const gcDoPlayerSetViewport &);
    void Write(cFile &) const;
};

// ── gcDoPlayerSetViewport::New(cMemPool *, cBase *)  @ 0x002F29EC, 148B ──
cBase *gcDoPlayerSetViewport::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayerSetViewport *result = 0;
    gcDoPlayerSetViewport *obj = (gcDoPlayerSetViewport *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoPlayerSetViewportvirtualtable;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0xC) = tagged;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoPlayerSetViewport::GetType(void) const {
    if (!type_gcDoPlayerSetViewport) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoPlayerSetViewport = cType::InitializeType(
            0, 0, 0xA8, type_action, gcDoPlayerSetViewport::New, 0, 0, 0);
    }
    return type_gcDoPlayerSetViewport;
}

void gcDoPlayerSetViewport::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoPlayerSetViewport) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoPlayerSetViewport = cType::InitializeType(
                0, 0, 0xA8, type_action, gcDoPlayerSetViewport::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoPlayerSetViewport;
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
    *this = *(const gcDoPlayerSetViewport *)copy;
}

// ── gcDoPlayerSetViewport::Write(cFile &) const  @ 0x002F2B98, 100B ──
void gcDoPlayerSetViewport::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);
    mDesired1.Write(wb);
    mDesired2.Write(wb);
    wb.End();
}
