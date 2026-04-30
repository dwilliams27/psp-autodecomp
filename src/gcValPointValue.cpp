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
    cBase *mParent;
    void *mVtable;
    int mValue0;
    int mValue1;

    void Write(cFile &) const;
};

class gcValPointValue : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

struct cTypeMethod {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern char cBaseclassdesc[];
extern char D_000005B8[];
extern char D_00009A80[];

void gcDesiredObject_gcDesiredObject(void *, cBase *);

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValPointValue;

cBase *gcValPointValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValPointValue *result = 0;
    gcValPointValue *obj = (gcValPointValue *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = D_00009A80;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x10, (cBase *)obj);
        ((void **)obj)[5] = D_000005B8;
        ((int *)obj)[7] = 7;
        ((int *)obj)[8] = 0;
        ((void **)obj)[5] = (void *)0x389760;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValPointValue::GetType(void) const {
    if (!type_gcValPointValue) {
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
        type_gcValPointValue = cType::InitializeType(0, 0, 0x208, type_value,
                                                     gcValPointValue::New,
                                                     0, 0, 0);
    }
    return type_gcValPointValue;
}

void gcValPointValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(((const int *)this)[2]);
    wb.Write(((const int *)this)[3]);

    const cTypeMethod *method =
        (const cTypeMethod *)((char *)((const void **)this)[5] + 0x28);
    char *base = (char *)this + 0x10;
    method->fn(base + method->offset, wb._file);
    wb.End();
}
