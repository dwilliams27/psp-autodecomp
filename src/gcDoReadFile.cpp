// gcDoReadFile.cpp - decompiled from gcDoReadFile.obj.
//
// Functions:
//   0x001C3FA4 gcDoReadFile::New(cMemPool *, cBase *) static  152B
//   0x001C4154 gcDoReadFile::Write(cFile &) const             148B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
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

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char gcDoReadFilevirtualtable[];

class gcDoReadFile : public gcAction {
public:
    int mFieldC;
    gcDesiredValue mDesiredValue;
    unsigned int mBase;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    gcDoReadFile &operator=(const gcDoReadFile &);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

int gcDoReadFile::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || gcAction::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);

    int sp14;
    int value = *(int *)((char *)this + 0x14);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x14);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x14) = newValue;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoReadFile::GetText(char *buf) const {
    register const gcDoReadFile *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    register const char *defaultText __asm__("$18") = (const char *)0x36C768;

    cStrAppend(out, (const char *)0x36C758, defaultText);

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
            cStrCat(out, (const char *)0x36C740);
        }
        cStrAppend(out, (const char *)0x36C76C);
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
        cStrAppend(out, (const char *)0x36C770, text);
    }

    cStrAppend(out, (const char *)0x36C774);
}
