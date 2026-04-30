// gcValLobbyScoreboardInfo -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

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

class gcValLobbyScoreboardInfo : public gcValue {
public:
    int mField8;
    int mFieldC;
    int mDesired10;
    int mDesired14;
    int mDesired18;
    int mDesired1C;
    int mBase20;
    int mBase24;

    gcValLobbyScoreboardInfo &operator=(const gcValLobbyScoreboardInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyScoreboardInfo;

// 0x0034af2c -- gcValLobbyScoreboardInfo::AssignCopy(const cBase *)
void gcValLobbyScoreboardInfo::AssignCopy(const cBase *base) {
    const gcValLobbyScoreboardInfo *other = 0;

    if (base != 0) {
        if (!type_gcValLobbyScoreboardInfo) {
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
            type_gcValLobbyScoreboardInfo = cType::InitializeType(
                0, 0, 0x124, type_value, gcValLobbyScoreboardInfo::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValLobbyScoreboardInfo;
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
            other = (const gcValLobbyScoreboardInfo *)base;
        }
    }

    operator=(*other);
}

// 0x0034b290 -- gcValLobbyScoreboardInfo::Write(cFile &) const
void gcValLobbyScoreboardInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);

    int val0 = mBase20;
    int flag0 = 0;
    if (val0 & 1) {
        flag0 = 1;
    }
    cBase *ptr0;
    if (flag0 != 0) {
        ptr0 = 0;
    } else {
        ptr0 = (cBase *)val0;
    }
    wb.WriteBase(ptr0);

    int val1 = mBase24;
    int flag1 = 0;
    if (val1 & 1) {
        flag1 = 1;
    }
    cBase *ptr1;
    if (flag1 != 0) {
        ptr1 = 0;
    } else {
        ptr1 = (cBase *)val1;
    }
    wb.WriteBase(ptr1);
    wb.End();
}
