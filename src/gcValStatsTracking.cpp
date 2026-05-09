class cBase;
class cFile;
class cFileHandle;
class cMemPool;
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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
    cType *(*fn)(void *, short, void *);
};

struct TextEntry {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

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
    void AssignCopy(const cBase *);
    gcValStatsTracking &operator=(const gcValStatsTracking &);
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

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

int gcValStatsTracking::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || gcLValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);

    int sp14;
    int value = *(int *)((char *)this + 0x10);
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

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x10);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x10) = newValue;
    __0oKcReadBlockdtv(rb, 2);
    return result;
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

void gcValStatsTracking::AssignCopy(const cBase *base) {
    const gcValStatsTracking *other = 0;

    if (base != 0) {
        if (!type_gcValStatsTracking) {
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
            type_gcValStatsTracking = cType::InitializeType(
                0, 0, 0xE8, type_variable, gcValStatsTracking::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *wanted = type_gcValStatsTracking;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
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
            other = (const gcValStatsTracking *)base;
        }
    }

    operator=(*other);
}

void gcValStatsTracking::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36E2EC, (const char *)0x36DAF0);

    int val = *(const int *)((const char *)this + 0x10);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }

    if (flag != 0) {
        val = 0;
    } else {
        val = (val != 0);
        val &= 0xFF;
        val = (val != 0);
    }

    if (val != 0) {
        cStrCat(buf, (const char *)0x36E300);

        register int val2 __asm__("$4") = *(const int *)((const char *)this + 0x10);
        register int check __asm__("$5") = 0;
        register int bit2 __asm__("$6") = val2 & 1;
        if (bit2) {
            check = 1;
        }

        char *typeInfo;
        if (check != 0) {
            check = 0;
            typeInfo = *(char **)(check + 4);
        } else {
            check = val2;
            typeInfo = *(char **)(check + 4);
        }
        TextEntry *entry = (TextEntry *)(typeInfo + 0x40);
        short off = entry->offset;
        void (*fn)(void *, char *) = entry->fn;
        fn((char *)check + off, buf);

        cStrCat(buf, (const char *)0x36E2E8);
    }
}
