// gcDoPlayerAssignController.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction. Class holds an int controller field at 0xC and two
// gcDesiredValue subobjects at 0x10 and 0x14.
//
// Functions:
//   0x002F0FE4 gcDoPlayerAssignController::New(cMemPool *, cBase *) static  152B
//   0x002F1194 gcDoPlayerAssignController::Write(cFile &) const             112B
//   0x002F155C gcDoPlayerAssignController::GetText(char *) const            408B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcDesiredValue {
public:
    unsigned int mField0;
    void Read(cReadBlock &);
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
    int Read(cFile &, cMemPool *);
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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);

extern char gcDoPlayerAssignControllervirtualtable[];

class gcDoPlayerAssignController : public gcAction {
public:
    int mController;          // 0x0C
    gcDesiredValue mDesired1; // 0x10
    gcDesiredValue mDesired2; // 0x14

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoPlayerAssignController &operator=(const gcDoPlayerAssignController &);
    static void operator delete(void *);
    ~gcDoPlayerAssignController(void);
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoPlayerAssignController asm("D_0009F6C4");

inline void gcDoPlayerAssignController::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

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

int gcDoPlayerAssignController::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] == 5 &&
        ((gcAction *)this)->gcAction::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    {
        cFileHandle *h = *(cFileHandle **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x0C, 4);
    }
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoPlayerAssignController::GetText(char *buf) const {
    register const gcDoPlayerAssignController *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;

    cStrAppend(out, (const char *)0x36EDBC, (const char *)0x36DAF0);

    int valid = 0;
    int controller = *(int *)((const char *)self + 0x0C);
    if ((controller == 3) || (controller == 5)) {
        valid = 1;
    }
    valid &= 0xFF;

    if (valid != 0) {
        cStrAppend(out, (const char *)0x36E440);

        int flag = 0;
        int val = *(int *)((const char *)self + 0x10);
        int tag = val & 1;
        if (tag != 0) {
            flag = 1;
        }
        if (flag != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        controller = *(int *)((const char *)self + 0x0C);
    }

    int valid2 = 0;
    if ((controller == 3) || (controller == 5)) {
        valid2 = 1;
    }
    valid2 &= 0xFF;

    if (valid2 != 0) {
        cStrAppend(out, (const char *)0x36DAD8);

        int flag2 = 0;
        int val2 = *(int *)((const char *)self + 0x14);
        if (val2 & 1) {
            flag2 = 1;
        }
        if (flag2 != 0) {
            val2 = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check2 = val2;
        if (check2 != 0) {
            char *typeInfo2 = *(char **)(check2 + 4);
            GetTextSlot *slot2 = (GetTextSlot *)(typeInfo2 + 0xD0);
            slot2->fn((char *)val2 + slot2->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    cStrAppend(out, (const char *)0x36DCEC);
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoPlayerAssignController::~gcDoPlayerAssignController(void) {
    *(void **)((char *)this + 4) = gcDoPlayerAssignControllervirtualtable;
    char *second = (char *)this + 0x10;

    if ((void *)((char *)this + 0x14) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x10) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
