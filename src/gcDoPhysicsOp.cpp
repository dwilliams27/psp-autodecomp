// gcDoPhysicsOp.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction. Vtable at 0x43B8 (gcDoPhysicsOpvirtualtable, relocated).
//
// Functions:
//   0x002ec434 gcDoPhysicsOp::New(cMemPool *, cBase *) static     148B
//   0x002ec5e0 gcDoPhysicsOp::Write(cFile &) const                100B

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

struct ePoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;       // 0x00 (in gcExpression — this is the gcExpression's own field)
    void *mVTable;       // 0x04
    unsigned int mNext;  // 0x08
    gcAction(cBase *);
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    unsigned int mValue;  // 0x00, 4 bytes total
    void Write(cWriteBlock &) const;
};

class gcDoPhysicsOp : public gcAction {
public:
    unsigned int mFieldC;        // 0x0C
    gcDesiredValue mDesired;     // 0x10

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern char gcDoPhysicsOpvirtualtable[];   // @ 0x43B8

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPhysicsOp asm("D_0009F6B8");

inline void *operator new(unsigned int, void *p) { return p; }

// ── gcDoPhysicsOp::Write @ 0x002ec5e0 ──
void gcDoPhysicsOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write((int)mFieldC);
    mDesired.Write(wb);
    wb.End();
}

// ── gcDoPhysicsOp::New @ 0x002ec434 ──
cBase *gcDoPhysicsOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPhysicsOp *result = 0;
    gcDoPhysicsOp *obj = (gcDoPhysicsOp *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcAction(parent);
        *(void **)((char *)obj + 4) = gcDoPhysicsOpvirtualtable;
        *(unsigned int *)((char *)obj + 0xC) = 0;
        *(unsigned int *)((char *)obj + 0x10) = (unsigned int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoPhysicsOp::GetType @ 0x002ec4c8 ──
const cType *gcDoPhysicsOp::GetType(void) const {
    if (!type_gcDoPhysicsOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoPhysicsOp = cType::InitializeType(
            0, 0, 0x264, type_action, gcDoPhysicsOp::New, 0, 0, 0);
    }
    return type_gcDoPhysicsOp;
}
