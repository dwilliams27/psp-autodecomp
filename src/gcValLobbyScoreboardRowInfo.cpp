// gcValLobbyScoreboardRowInfo -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

extern char cBaseclassdesc[];
extern char gcValLobbyScoreboardRowInfovirtualtable[];

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

class gcValue {
public:
    cBase *mParent;
    void *mVtable;

    void Write(cFile &) const;
};

class gcValLobbyScoreboardRowInfo : public gcValue {
public:
    int mField8;
    int mFieldC;
    int mDesired10;
    int mDesired14;
    int mBase18;

    gcValLobbyScoreboardRowInfo &operator=(const gcValLobbyScoreboardRowInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyScoreboardRowInfo;

// 0x0034d2c4 -- gcValLobbyScoreboardRowInfo::New(cMemPool *, cBase *) static
cBase *gcValLobbyScoreboardRowInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyScoreboardRowInfo *result = 0;
    gcValLobbyScoreboardRowInfo *obj =
        (gcValLobbyScoreboardRowInfo *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbyScoreboardRowInfovirtualtable;
        ((int *)obj)[2] = 0;
        int tagged = (int)obj | 1;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = tagged;
        ((int *)obj)[5] = tagged;
        ((int *)obj)[6] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0034d360 -- gcValLobbyScoreboardRowInfo::GetType(void) const
const cType *gcValLobbyScoreboardRowInfo::GetType(void) const {
    if (!type_gcValLobbyScoreboardRowInfo) {
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
        type_gcValLobbyScoreboardRowInfo = cType::InitializeType(
            0, 0, 0x127, type_value, gcValLobbyScoreboardRowInfo::New, 0, 0, 0);
    }
    return type_gcValLobbyScoreboardRowInfo;
}

// 0x0034d120 -- gcValLobbyScoreboardRowInfo::AssignCopy(const cBase *)
void gcValLobbyScoreboardRowInfo::AssignCopy(const cBase *base) {
    const gcValLobbyScoreboardRowInfo *other = 0;

    if (base != 0) {
        if (!type_gcValLobbyScoreboardRowInfo) {
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
            type_gcValLobbyScoreboardRowInfo = cType::InitializeType(
                0, 0, 0x127, type_value, gcValLobbyScoreboardRowInfo::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValLobbyScoreboardRowInfo;
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
            other = (const gcValLobbyScoreboardRowInfo *)base;
        }
    }

    operator=(*other);
}

// 0x0034d478 -- gcValLobbyScoreboardRowInfo::Write(cFile &) const
void gcValLobbyScoreboardRowInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    int val = this->mBase18;
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
