class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
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

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mClassDesc;

    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mClassDesc;
    unsigned int mNext;

    int Read(cFile &, cMemPool *);
};

class cReadBlock {
public:
    int _data[5];

    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredEnumerationEntry {
public:
    cBase *mParent;
    void *mClassDesc;
    unsigned char mField8;
    unsigned char mField9;
    char padA[2];
    int mFieldC;
    int mField10;
    int mBase14;

    void Write(cWriteBlock &) const;
};

extern char cBaseclassdesc[];
extern char D_000098C0[];
extern char gcDesiredEnumerationEntryvirtualtable[];

struct AllocBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct ReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcValObjectHasRelation : public gcValue {
public:
    int mBase8;
    gcDesiredEnumerationEntry mDesiredC;
    bool mField24;

    gcValObjectHasRelation &operator=(const gcValObjectHasRelation &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcDoSetObjectRelationship : public gcAction {
public:
    int mFieldC;
    int mField10;

    int Read(cFile &, cMemPool *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValObjectHasRelation asm("D_0009F8C0");

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// ── gcValObjectHasRelation::New(cMemPool *, cBase *) static @ 0x00355768 ──
cBase *gcValObjectHasRelation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectHasRelation *result = 0;
    gcValObjectHasRelation *obj =
        (gcValObjectHasRelation *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = D_000098C0;
        int flagged = (int)obj | 1;
        *(void **)((char *)obj + 0x10) = cBaseclassdesc;
        *(int *)((char *)obj + 8) = flagged;
        *(gcValObjectHasRelation **)((char *)obj + 0xC) = obj;
        *(void **)((char *)obj + 0x10) = gcDesiredEnumerationEntryvirtualtable;
        *(unsigned char *)((char *)obj + 0x14) = 1;
        *(unsigned char *)((char *)obj + 0x15) = 0;
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = 0;
        *(int *)((char *)obj + 0x20) = flagged;
        *(unsigned char *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValObjectHasRelation::GetType(void) const {
    if (!type_gcValObjectHasRelation) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValObjectHasRelation =
            cType::InitializeType(0, 0, 0x140, type_value,
                                  gcValObjectHasRelation::New, 0, 0, 0);
    }
    return type_gcValObjectHasRelation;
}

void gcValObjectHasRelation::AssignCopy(const cBase *base) {
    const gcValObjectHasRelation *other = 0;

    if (base != 0) {
        if (!type_gcValObjectHasRelation) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValObjectHasRelation = cType::InitializeType(
                0, 0, 0x140, type_value, gcValObjectHasRelation::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValObjectHasRelation;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValObjectHasRelation *)base;
        }
    }

    operator=(*other);
}

void gcValObjectHasRelation::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const gcValue *)this)->Write(file);
    int val = this->mBase8;
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
    char *desc = *(char **)((char *)this + 0x10);
    WriteEntry *entry = (WriteEntry *)(desc + 0x28);
    short off = entry->offset;
    char *sub = (char *)this + 0xC;
    entry->fn(sub + off, (void *)wb._data[0]);
    wb.Write(mField24);
    wb.End();
}

int gcValObjectHasRelation::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x08);
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

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x08);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x08) = newValue;

    char *typeInfo = *(char **)((char *)this + 0x10);
    ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
    char *desired = (char *)this + 0x0C;
    short off = rec->offset;
    char *target = desired + off;
    cFile *f = *(cFile **)&rb[0];
    cMemPool *desiredPool = cMemPool::GetPoolFromPtr(desired);
    rec->fn(target, f, desiredPool);

    char sp18;
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp18, 1);
    mField24 = sp18 != 0;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

int gcDoSetObjectRelationship::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if (rb[3] != 4 || gcAction::Read(file, pool) == 0) {
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
        goto first_out_done;
    }
    outValue = value;
first_out_done:
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
