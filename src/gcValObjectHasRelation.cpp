class cBase;
class cFile;
class cMemPool;
class cType;

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

    void Write(cFile &) const;
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

class gcValObjectHasRelation : public gcValue {
public:
    int mBase8;
    gcDesiredEnumerationEntry mDesiredC;
    bool mField24;

    gcValObjectHasRelation &operator=(const gcValObjectHasRelation &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValObjectHasRelation asm("D_0009F8C0");

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
