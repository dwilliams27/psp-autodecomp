// gcDoRecord.cpp - decompiled from gcDoRecord.obj.
//
// Functions:
//   0x001C4E24 gcDoRecord::New(cMemPool *, cBase *) static  152B
//   0x001C4EBC gcDoRecord::GetType(void) const              280B
//   0x001C4FD4 gcDoRecord::Write(cFile &) const             148B

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
    unsigned int mValue;
    void Write(cWriteBlock &) const;
};

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
};

class gcExpression {
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

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void gcAction_gcAction(void *, cBase *);

extern char gcDoRecordvirtualtable[];
extern const char gcDoRecord_base_name[];
extern const char gcDoRecord_base_desc[];

class gcDoRecord : public gcAction {
public:
    int mFieldC;
    gcDesiredValue mField10;
    unsigned int mField14;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    gcDoRecord &operator=(const gcDoRecord &);
    void Write(cFile &) const;
};

cBase *gcDoRecord::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoRecord *result = 0;
    gcDoRecord *obj = (gcDoRecord *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoRecordvirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        *(unsigned int *)((char *)obj + 0x14) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoRecord;

const cType *gcDoRecord::GetType(void) const {
    if (!type_gcDoRecord) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoRecord_base_name, gcDoRecord_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoRecord = cType::InitializeType(0, 0, 0x2E9, type_action,
                                                gcDoRecord::New, 0, 0, 0);
    }
    return type_gcDoRecord;
}

void gcDoRecord::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoRecord) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoRecord_base_name, gcDoRecord_base_desc, 1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoRecord = cType::InitializeType(
                0, 0, 0x2E9, type_action, gcDoRecord::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoRecord;
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
    *this = *(const gcDoRecord *)copy;
}

void gcDoRecord::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(mFieldC);
    mField10.Write(wb);

    int value = mField14;
    int flag = 0;
    if (value & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);
    wb.End();
}

void gcDoRecord::GetText(char *buf) const {
    register const gcDoRecord *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    register const char *defaultText __asm__("$18") = (const char *)0x36C7C4;

    cStrAppend(out, (const char *)0x36C7B0, defaultText);

    int op = *(int *)((const char *)self + 0x0C);
    if (op == 2) {
        int value = *(int *)((const char *)self + 0x10);
        int owned = 0;
        int tagged = value & 1;
        if (tagged != 0) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check = value;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36C790);
        }
        cStrAppend(out, (const char *)0x36C7C8);
    } else if (op == 3) {
        char text[256];
        int value = *(int *)((const char *)self + 0x14);
        text[0] = '\0';
        int owned = 0;
        int tagged = value & 1;
        if (tagged != 0) {
            owned = 1;
        }
        int valid;
        if (owned != 0) {
            valid = 0;
        } else {
            valid = value != 0;
            valid &= 0xFF;
            valid = valid != 0;
        }
        if (valid != 0) {
            int owned2 = 0;
            if (tagged != 0) {
                owned2 = 1;
            }
            char *typeInfo;
            if (owned2 != 0) {
                value = 0;
                typeInfo = *(char **)(value + 4);
            } else {
                typeInfo = *(char **)(value + 4);
            }
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0x40);
            slot->fn((char *)value + slot->offset, text);
        } else {
            ((cStr *)text)->Set(defaultText);
        }
        cStrAppend(out, (const char *)0x36C7CC, text);
    }

    cStrAppend(out, (const char *)0x36C7D0);
}
