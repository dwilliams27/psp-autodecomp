// gcValObjectCompare - decompiled from gcAll_psp.obj
//
// Functions:
//   0x00353978 gcValObjectCompare::New(cMemPool *, cBase *) static  144B
//   0x00353a08 gcValObjectCompare::GetType(void) const              280B
//   0x00353b20 gcValObjectCompare::Write(cFile &) const             172B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mVTable;
    int mObjectA;
    int mObjectB;

    void Write(cFile &) const;
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

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
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

class gcValObjectCompare : public gcValue {
public:
    gcValObjectCompare &operator=(const gcValObjectCompare &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern char cBaseclassdesc[];
extern char gcValObjectComparevirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValObjectCompare asm("D_0009F8B8");

void gcValObjectCompare::AssignCopy(const cBase *base) {
    const gcValObjectCompare *other = 0;

    if (base != 0) {
        if (!type_gcValObjectCompare) {
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
            type_gcValObjectCompare = cType::InitializeType(
                0, 0, 0x141, type_value, gcValObjectCompare::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValObjectCompare;
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
            other = (const gcValObjectCompare *)base;
        }
    }

    operator=(*other);
}

cBase *gcValObjectCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectCompare *result = 0;
    gcValObjectCompare *obj =
        (gcValObjectCompare *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValObjectComparevirtualtable;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 8) = tagged;
        *(int *)((char *)obj + 0xC) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValObjectCompare::GetType(void) const {
    if (type_gcValObjectCompare == 0) {
        if (type_value == 0) {
            if (type_expression == 0) {
                if (type_base == 0) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValObjectCompare = cType::InitializeType(0, 0, 0x141,
                                                        type_value,
                                                        gcValObjectCompare::New,
                                                        0, 0, 0);
    }
    return type_gcValObjectCompare;
}

void gcValObjectCompare::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue::Write(file);

    int valueA = mObjectA;
    int tagA = valueA & 1;
    int flagA = 0;
    if (tagA != 0) {
        flagA = 1;
    }
    cBase *ptrA;
    if (flagA != 0) {
        ptrA = 0;
    } else {
        ptrA = (cBase *)valueA;
    }
    wb.WriteBase(ptrA);

    int valueB = mObjectB;
    int flagB = 0;
    if (valueB & 1) {
        flagB = 1;
    }
    cBase *ptrB;
    if (flagB != 0) {
        ptrB = 0;
    } else {
        ptrB = (cBase *)valueB;
    }
    wb.WriteBase(ptrB);
    wb.End();
}
