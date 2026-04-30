class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

typedef void (*DesiredWriteFn)(cBase *, cFile *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
};

struct DesiredTypeInfoWrite {
    char _pad[0x28];
    DesiredWriteSlot mSlot;
};

struct PoolBlock {
    char _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *);
};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcValTableInfo : public gcValue {
public:
    gcValTableInfo &operator=(const gcValTableInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char gcValTableInfovirtualtable[];
extern char D_00000838[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValTableInfo asm("D_0009F8EC");

cBase *gcValTableInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValTableInfo *result = 0;
    gcValTableInfo *obj =
        (gcValTableInfo *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValTableInfovirtualtable;
        gcDesiredObject_gcDesiredObject((char *)obj + 8, (cBase *)obj);
        ((void **)obj)[3] = D_00000838;
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[3] = 0x38A000;
        ((int *)obj)[7] = 2;
        ((int *)obj)[8] = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValTableInfo::GetType(void) const {
    if (!type_gcValTableInfo) {
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
        type_gcValTableInfo = cType::InitializeType(0, 0, 0x248, type_value,
                                                    gcValTableInfo::New,
                                                    0, 0, 0);
    }
    return type_gcValTableInfo;
}

void gcValTableInfo::AssignCopy(const cBase *base) {
    const gcValTableInfo *other = 0;

    if (base != 0) {
        if (!type_gcValTableInfo) {
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
            type_gcValTableInfo = cType::InitializeType(
                0, 0, 0x248, type_value, gcValTableInfo::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValTableInfo;
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
            other = (const gcValTableInfo *)base;
        }
    }

    operator=(*other);
}

void gcValTableInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    DesiredTypeInfoWrite *ti = *(DesiredTypeInfoWrite **)((char *)this + 0x0C);
    DesiredWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x08);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(*(int *)((char *)this + 0x1C));
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}
