// gcValStringCompare -- decompiled from gcAll_psp.obj
//
// Functions:
//   0x0035dad0  gcValStringCompare::New(cMemPool *, cBase *) static
//   0x0035dc84  gcValStringCompare::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcValStringComparevirtualtable[];

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValStringCompare : public gcValue {
public:
    int mLeft;
    int mRight;
    bool mCaseSensitive;
    bool mExact;

    gcValStringCompare &operator=(const gcValStringCompare &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
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

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
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

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValStringCompare;

// -- gcValStringCompare::New(cMemPool *, cBase *) static @ 0x0035dad0 --
cBase *gcValStringCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStringCompare *result = 0;
    gcValStringCompare *obj =
        (gcValStringCompare *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValStringComparevirtualtable;
        unsigned int tagged = ((unsigned int)obj) | 1;
        obj->mLeft = tagged;
        obj->mRight = tagged;
        obj->mCaseSensitive = true;
        obj->mExact = true;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValStringCompare::GetType(void) const {
    if (!type_gcValStringCompare) {
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
        type_gcValStringCompare =
            cType::InitializeType(0, 0, 0x1EC, type_value,
                                  gcValStringCompare::New, 0, 0, 0);
    }
    return type_gcValStringCompare;
}

void gcValStringCompare::AssignCopy(const cBase *base) {
    const gcValStringCompare *other = 0;

    if (base != 0) {
        if (!type_gcValStringCompare) {
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
            type_gcValStringCompare = cType::InitializeType(
                0, 0, 0x1EC, type_value, gcValStringCompare::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValStringCompare;
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
            other = (const gcValStringCompare *)base;
        }
    }

    operator=(*other);
}

// -- gcValStringCompare::Write(cFile &) const @ 0x0035dc84 --
void gcValStringCompare::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    int left = mLeft;
    int leftFlag = 0;
    if (left & 1) {
        leftFlag = 1;
    }
    cBase *leftPtr;
    if (leftFlag != 0) {
        leftPtr = 0;
    } else {
        leftPtr = (cBase *)left;
    }
    wb.WriteBase(leftPtr);

    int right = mRight;
    int rightFlag = 0;
    if (right & 1) {
        rightFlag = 1;
    }
    cBase *rightPtr;
    if (rightFlag != 0) {
        rightPtr = 0;
    } else {
        rightPtr = (cBase *)right;
    }
    wb.WriteBase(rightPtr);

    wb.Write(mCaseSensitive);
    wb.Write(mExact);
    wb.End();
}
