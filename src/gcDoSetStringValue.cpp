class cBase;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

inline void *operator new(unsigned int, void *p) { return p; }

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcDoSetStringValue : public gcAction {
public:
    int mValue;
    int mString;

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoSetStringValue &operator=(const gcDoSetStringValue &);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcStatsTrackingStrings {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcDoSetStringValuevirtualtable[];
extern char gcStatsTrackingStringsvirtualtable[];

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_gcDoSetStringValue asm("D_0009F6F4");
static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");

cBase *gcDoSetStringValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetStringValue *result = 0;
    gcDoSetStringValue *obj =
        (gcDoSetStringValue *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcAction(parent);
        ((void **)obj)[1] = gcDoSetStringValuevirtualtable;
        int self = (int)obj | 1;
        ((int *)obj)[3] = self;
        ((int *)obj)[4] = self;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoSetStringValue::GetType(void) const {
    if (!type_gcDoSetStringValue) {
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
        type_gcDoSetStringValue = cType::InitializeType(
            0, 0, 0x180, type_action, gcDoSetStringValue::New, 0, 0, 0);
    }
    return type_gcDoSetStringValue;
}

void gcDoSetStringValue::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoSetStringValue) {
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
            type_gcDoSetStringValue = cType::InitializeType(
                0, 0, 0x180, type_action, gcDoSetStringValue::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoSetStringValue;
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
    *this = *(const gcDoSetStringValue *)copy;
}

void gcDoSetStringValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    int valC = ((const int *)this)[3];
    int flagC = 0;
    if (valC & 1) {
        flagC = 1;
    }
    cBase *ptrC;
    if (flagC != 0) {
        ptrC = 0;
    } else {
        ptrC = (cBase *)valC;
    }
    wb.WriteBase(ptrC);

    int val10 = ((const int *)this)[4];
    int flag10 = 0;
    if (val10 & 1) {
        flag10 = 1;
    }
    cBase *ptr10;
    if (flag10 != 0) {
        ptr10 = 0;
    } else {
        ptr10 = (cBase *)val10;
    }
    wb.WriteBase(ptr10);

    wb.End();
}

int gcDoSetStringValue::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || gcAction::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x0C);
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

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x0C);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int value2 = *(int *)((char *)this + 0x10);
    int tag2 = value2 & 1;
    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x0C) = newValue;

    int sp18;
    int flag3 = 0;
    if (tag2 != 0) {
        flag3 = 1;
    }

    int outValue2;
    if (flag3 != 0) {
        outValue2 = 0;
        goto second_out_done;
    }
    outValue2 = value2;
second_out_done:
    sp18 = outValue2;

    int flag4 = 0;
    if (tag2 != 0) {
        flag4 = 1;
    }

    int base2;
    if (flag4 != 0) {
        base2 = value2 & ~1;
    } else {
        base2 = *(int *)value2;
    }

    cMemPool *childPool2 = cMemPool::GetPoolFromPtr((char *)this + 0x10);
    ((cReadBlock *)rb)->ReadBase(childPool2, (cBase *)base2,
                                 *(cBase **)&sp18);

    int newValue2;
    if (sp18 == 0) {
        newValue2 = base2 | 1;
    } else {
        newValue2 = sp18;
    }
    *(int *)((char *)this + 0x10) = newValue2;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

cBase *gcStatsTrackingStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStatsTrackingStrings *result = 0;
    gcStatsTrackingStrings *obj =
        (gcStatsTrackingStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcStatsTrackingStringsvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int self = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = self;
        *(int *)((char *)obj + 0x10) = self;
        result = obj;
    }
    return (cBase *)result;
}
