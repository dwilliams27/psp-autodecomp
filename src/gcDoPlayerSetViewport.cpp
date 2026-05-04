// gcDoPlayerSetViewport.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction. Class holds two gcDesiredValue subobjects at offsets 0xC and 0x10.
//
// Functions:
//   0x002F29EC gcDoPlayerSetViewport::New(cMemPool *, cBase *) static  148B
//   0x002F2B98 gcDoPlayerSetViewport::Write(cFile &) const             100B
//   0x002F2BFC gcDoPlayerSetViewport::Read(cFile &, cMemPool *)        212B
//   0x002F2E24 gcDoPlayerSetViewport::GetText(char *) const            288B
//   0x002F3450 gcDoPlayerSetViewport::~gcDoPlayerSetViewport(void)     296B

class cBase;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcDesiredValue {
public:
    unsigned int mField0;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
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
    int  Read(cFile &, cMemPool *);
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
    short _pad;
    void (*fn)(void *, void *);
};

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                             unsigned int id, bool validate);
extern "C" void __0oKcReadBlockdtv(void *rb, int flags);

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

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
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    gcDoPlayerSetViewport &operator=(const gcDoPlayerSetViewport &);
    int  Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    ~gcDoPlayerSetViewport(void);
};

inline void gcDoPlayerSetViewport::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

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

// ── gcDoPlayerSetViewport::Read(cFile &, cMemPool *)  @ 0x002F2BFC, 212B ──
int gcDoPlayerSetViewport::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if (rb[3] != 4 || gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    mDesired1.Read(*(cReadBlock *)rb);
    mDesired2.Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── gcDoPlayerSetViewport::GetText(char *) const  @ 0x002F2E24, 288B ──
void gcDoPlayerSetViewport::GetText(char *buf) const {
    char text[256];
    int flag1 = 0;
    int val = *(int *)((const char *)this + 0x0C);
    text[0] = '\0';
    if (val & 1) {
        flag1 = 1;
    }
    if (flag1 != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    int check1 = val;
    if (check1 != 0) {
        char *typeInfo = *(char **)(check1 + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, text);
    } else {
        cStrCat(text, (const char *)0x36DB24);
    }
    cStrAppend(buf, (const char *)0x36EDDC, text);

    int val2 = *(int *)((const char *)this + 0x10);
    int flag2 = 0;
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
        char *typeInfo = *(char **)(check2 + 4);
        GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val2 + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }
    cStrCat(buf, (const char *)0x36DCEC);
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── gcDoPlayerSetViewport::~gcDoPlayerSetViewport(void)  @ 0x002F3450, 296B ──
gcDoPlayerSetViewport::~gcDoPlayerSetViewport(void) {
    *(void **)((char *)this + 4) = gcDoPlayerSetViewportvirtualtable;
    char *second = (char *)this + 0x0C;

    if ((void *)((char *)this + 0x10) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
