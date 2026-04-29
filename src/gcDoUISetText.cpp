// gcDoUISetText.cpp - gcAll_psp.obj

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
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];
    gcDesiredUIWidgetHelper(int);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    char _pad[12];
    gcAction(cBase *);
    void Write(cFile &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
};

class gcDoUISetText : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoUISetText &operator=(const gcDoUISetText &);
};

class gcValObjectHasCategory : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern char cBaseclassdesc[];
extern char gcValuevirtualtable[];
extern char gcDoUISetTextvirtualtable[];
extern char gcValObjectHasCategoryvirtualtable[];

void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);

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

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUISetText asm("D_0009F748");

// -- gcDoUISetText::AssignCopy(const cBase *) @ 0x003132d0 --
void gcDoUISetText::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoUISetText) {
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
            type_gcDoUISetText = cType::InitializeType(
                0, 0, 0xFD, type_action, gcDoUISetText::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoUISetText;
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
    *this = *(const gcDoUISetText *)copy;
}

// -- gcDoUISetText::New(cMemPool *, cBase *) static @ 0x00313474 --
cBase *gcDoUISetText::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetText *result = 0;
    gcDoUISetText *obj = (gcDoUISetText *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetTextvirtualtable;
        *(int *)((char *)obj + 0x0C) = 0;
        *(int *)((char *)obj + 0x10) = 0;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0x14, 1);
        *(int *)((char *)obj + 0x20) = 0;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0x24) = 0;
        *(int *)((char *)obj + 0x28) = tagged;
        *(int *)((char *)obj + 0x2C) = tagged;
        *(int *)((char *)obj + 0x30) = tagged;
        *(int *)((char *)obj + 0x34) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// -- gcDoUISetText::GetType(void) const @ 0x0031352c --
const cType *gcDoUISetText::GetType(void) const {
    if (!type_gcDoUISetText) {
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
        type_gcDoUISetText = cType::InitializeType(
            0, 0, 0xFD, type_action, gcDoUISetText::New, 0, 0, 0);
    }
    return type_gcDoUISetText;
}

// -- gcDoUISetText::Write(cFile &) const @ 0x00313644 --
void gcDoUISetText::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    gcAction::Write(file);
    wb.Write(*(int *)((const char *)this + 0x0C));
    wb.Write(*(int *)((const char *)this + 0x10));
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x14))->Write(wb);
    ((const cHandle *)((const char *)this + 0x20))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0x24));

    int val0 = *(int *)((const char *)this + 0x28);
    unsigned char flag0 = 0;
    int tag0 = val0 & 1;
    if (tag0 != 0) {
        flag0 = 1;
    }
    cBase *base0;
    if (flag0 == 0) {
        base0 = (cBase *)val0;
    } else {
        base0 = 0;
    }
    wb.WriteBase(base0);

    ((const gcDesiredValue *)((const char *)this + 0x30))->Write(wb);

    int val1 = *(int *)((const char *)this + 0x2C);
    unsigned char flag1 = 0;
    int tag1 = val1 & 1;
    if (tag1 != 0) {
        flag1 = 1;
    }
    cBase *base1;
    if (flag1 == 0) {
        base1 = (cBase *)val1;
    } else {
        base1 = 0;
    }
    wb.WriteBase(base1);

    wb.Write(*(int *)((const char *)this + 0x34));
    wb.End();
}

// -- gcValObjectHasCategory::New(cMemPool *, cBase *) static @ 0x00354890 --
cBase *gcValObjectHasCategory::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectHasCategory *result = 0;
    gcValObjectHasCategory *obj =
        (gcValObjectHasCategory *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValuevirtualtable;
        int tagged = (int)obj | 1;
        *(void **)((char *)obj + 0x10) = cBaseclassdesc;
        *(int *)((char *)obj + 0x08) = tagged;
        *(gcValObjectHasCategory **)((char *)obj + 0x0C) = obj;
        *(void **)((char *)obj + 0x10) = gcValObjectHasCategoryvirtualtable;
        *(unsigned char *)((char *)obj + 0x14) = 1;
        *(unsigned char *)((char *)obj + 0x15) = 0;
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = 0;
        *(int *)((char *)obj + 0x20) = tagged;
        result = obj;
    }
    return (cBase *)result;
}
