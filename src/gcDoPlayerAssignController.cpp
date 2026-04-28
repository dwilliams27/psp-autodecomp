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
    void Write(cFile &) const;
};

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

// ── gcDoPlayerAssignController::Write(cFile &) const  @ 0x002F1194, 112B ──
void gcDoPlayerAssignController::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(mController);
    mDesired1.Write(wb);
    mDesired2.Write(wb);
    wb.End();
}
