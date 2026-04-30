class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
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

extern char cBaseclassdesc[];
extern char gcValStatsTrackingvirtualtable[];

class gcValStatsTracking : public gcLValue {
public:
    int mField8;
    gcDesiredValue mDesiredC;
    int mField10;

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValStatsTracking;

cBase *gcValStatsTracking::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStatsTracking *result = 0;
    gcValStatsTracking *obj = (gcValStatsTracking *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValStatsTrackingvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = tagged;
        *(int *)((char *)obj + 0x10) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcValStatsTracking::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField8);
    mDesiredC.Write(wb);
    int val = mField10;
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

const cType *gcValStatsTracking::GetType(void) const {
    if (!type_gcValStatsTracking) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(0, 0, 0x6A,
                                                            type_base,
                                                            0, 0, 0, 0);
                }
                type_value = cType::InitializeType(0, 0, 0x6C,
                                                   type_expression,
                                                   0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(0, 0, 0x6D, type_value,
                                                  0, 0, 0, 0);
        }
        type_gcValStatsTracking = cType::InitializeType(0, 0, 0xE8,
                                                        type_variable,
                                                        gcValStatsTracking::New,
                                                        0, 0, 0);
    }
    return type_gcValStatsTracking;
}
