// gcValGameSetting - gcLValue subclass with a desired value and enum entry.

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
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
    void *fn;
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class cType {
public:
    char _p0[0x1C];
    cType *mParent;
    char _p20[8];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValGameSetting : public gcLValue {
public:
    gcValGameSetting &operator=(const gcValGameSetting &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

extern char cBaseclassdesc[];
extern char gcLValuevirtualtable[];
extern char gcDesiredEnumerationEntryvirtualtable[];

typedef void (*WriteFn)(void *, cFile *);

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValGameSetting;

// 0x00344838 - gcValGameSetting::New(cMemPool *, cBase *) static
cBase *gcValGameSetting::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValGameSetting *result = 0;
    gcValGameSetting *obj = (gcValGameSetting *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((int *)obj)[2] = 0;
        int tagged = (int)obj | 1;
        ((void **)obj)[5] = cBaseclassdesc;
        ((int *)obj)[3] = tagged;
        ((gcValGameSetting **)obj)[4] = obj;
        ((void **)obj)[5] = gcDesiredEnumerationEntryvirtualtable;
        ((unsigned char *)obj)[0x18] = 1;
        ((unsigned char *)obj)[0x19] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = tagged;
        ((int *)obj)[10] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00344658 - gcValGameSetting::AssignCopy(const cBase *)
void gcValGameSetting::AssignCopy(const cBase *base) {
    const gcValGameSetting *other = 0;

    if (base != 0) {
        if (!type_gcValGameSetting) {
            if (!type_variable) {
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
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValGameSetting = cType::InitializeType(
                0, 0, 0x1B4, type_variable, gcValGameSetting::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValGameSetting;
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
                ok = 0;
            }
        } else {
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValGameSetting *)base;
        }
    }

    operator=(*other);
}

// 0x00344a4c - gcValGameSetting::Write(cFile &) const
void gcValGameSetting::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcLValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 0x0C))->Write(wb);
    wb.Write(*(int *)((const char *)this + 8));
    const cTypeMethod *entry = &((const cType *)*(void *const *)((const char *)this + 0x14))->write_m;
    char *base = (char *)this + 0x10;
    ((WriteFn)entry->fn)(base + entry->offset, *(cFile **)&wb._data[0]);
    wb.Write(*(int *)((const char *)this + 0x28));
    wb.End();
}

// 0x003448f8 - gcValGameSetting::GetType(void) const
const cType *gcValGameSetting::GetType(void) const {
    if (!type_gcValGameSetting) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValGameSetting = cType::InitializeType(
            0, 0, 0x1B4, type_variable, gcValGameSetting::New, 0, 0, 0);
    }
    return type_gcValGameSetting;
}
