// gcValRandomNumber -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mClassDesc;

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mValue;

    void Write(cWriteBlock &) const;
};

extern char cBaseclassdesc[];
extern char gcValRandomNumbervirtualtable[];

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

class gcValRandomNumber : public gcValue {
public:
    int mField8;
    gcDesiredValue mDesiredC;
    gcDesiredValue mDesired10;
    bool mField14;

    gcValRandomNumber &operator=(const gcValRandomNumber &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValRandomNumber;

// 0x00357d48 -- gcValRandomNumber::New(cMemPool *, cBase *) static
cBase *gcValRandomNumber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValRandomNumber *result = 0;
    gcValRandomNumber *obj = (gcValRandomNumber *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValRandomNumbervirtualtable;
        int one = 1;
        int tagged = (int)obj | 1;
        ((int *)obj)[2] = one;
        ((int *)obj)[3] = tagged;
        ((int *)obj)[4] = tagged;
        ((unsigned char *)obj)[0x14] = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValRandomNumber::GetType(void) const {
    if (!type_gcValRandomNumber) {
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
        type_gcValRandomNumber = cType::InitializeType(0, 0, 0x11A, type_value,
                                                       gcValRandomNumber::New,
                                                       0, 0, 0);
    }
    return type_gcValRandomNumber;
}

void gcValRandomNumber::AssignCopy(const cBase *base) {
    const gcValRandomNumber *other = 0;

    if (base != 0) {
        if (!type_gcValRandomNumber) {
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
            type_gcValRandomNumber = cType::InitializeType(
                0, 0, 0x11A, type_value, gcValRandomNumber::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValRandomNumber;
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
            other = (const gcValRandomNumber *)base;
        }
    }

    operator=(*other);
}

void gcValRandomNumber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(mField8);
    wb.Write(mField14);
    mDesiredC.Write(wb);
    mDesired10.Write(wb);
    wb.End();
}
