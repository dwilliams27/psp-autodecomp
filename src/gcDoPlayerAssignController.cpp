// gcDoPlayerAssignController.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction. Class holds an int controller field at 0xC and two
// gcDesiredValue subobjects at 0x10 and 0x14.
//
// Functions:
//   0x002F0FE4 gcDoPlayerAssignController::New(cMemPool *, cBase *) static  152B
//   0x002F1194 gcDoPlayerAssignController::Write(cFile &) const             112B

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
    void Write(int);
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

extern char gcDoPlayerAssignControllervirtualtable[];

class gcDoPlayerAssignController : public gcAction {
public:
    int mController;          // 0x0C
    gcDesiredValue mDesired1; // 0x10
    gcDesiredValue mDesired2; // 0x14

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoPlayerAssignController &operator=(const gcDoPlayerAssignController &);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoPlayerAssignController asm("D_0009F6C4");

// ── gcDoPlayerAssignController::New(cMemPool *, cBase *)  @ 0x002F0FE4, 152B ──
cBase *gcDoPlayerAssignController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayerAssignController *result = 0;
    gcDoPlayerAssignController *obj = (gcDoPlayerAssignController *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoPlayerAssignControllervirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        *(unsigned int *)((char *)obj + 0x14) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoPlayerAssignController::GetType(void) const {
    if (!type_gcDoPlayerAssignController) {
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
        type_gcDoPlayerAssignController = cType::InitializeType(
            0, 0, 0x14B, type_action, gcDoPlayerAssignController::New, 0, 0,
            0x80);
    }
    return type_gcDoPlayerAssignController;
}

void gcDoPlayerAssignController::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoPlayerAssignController) {
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
            type_gcDoPlayerAssignController = cType::InitializeType(
                0, 0, 0x14B, type_action, gcDoPlayerAssignController::New, 0,
                0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoPlayerAssignController;
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
    *this = *(const gcDoPlayerAssignController *)copy;
}

// ── gcDoPlayerAssignController::Write(cFile &) const  @ 0x002F1194, 112B ──
void gcDoPlayerAssignController::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(mController);
    mDesired1.Write(wb);
    mDesired2.Write(wb);
    wb.End();
}
