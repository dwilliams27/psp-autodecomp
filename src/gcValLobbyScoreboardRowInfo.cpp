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

struct GetTextSlot {
    short offset;
    short pad;
    void *fn;
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
    void GetText(char *) const;
    void Write(cFile &) const;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

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

// 0x0034d9cc -- gcValLobbyScoreboardRowInfo::GetText(char *) const
void gcValLobbyScoreboardRowInfo::GetText(char *buf) const {
    register const gcValLobbyScoreboardRowInfo *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char local[256];

    cStrAppend(out, (const char *)0x36F530);
    cStrAppend(out, (const char *)0x36DCB8, (const char *)0x36DAF0);

    register const char *open __asm__("$18") = (const char *)0x36E300;
    cStrAppend(out, open);

    int tagged = 0;
    int val = *(int *)((const char *)self + 0x10);
    int tag = val & 1;
    register const char *close __asm__("$19") = (const char *)0x36E2E8;
    if (tag != 0) {
        tagged = 1;
    }
    if (tagged != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int check = val;
    if (check != 0) {
        char *type = *(char **)(check + 4);
        GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
        ((void (*)(void *, char *))slot->fn)((char *)val + slot->offset, out);
    } else {
        cStrCat(out, (const char *)0x36DB24);
    }

    cStrAppend(out, close);

    int isTwo = *(int *)((const char *)self + 8) == 2;
    isTwo &= 0xFF;
    if (isTwo != 0) {
        cStrAppend(out, open);

        if (*(int *)((const char *)self + 0x0C) == 0) {
            int obj = *(int *)((const char *)self + 0x14);
            int owned = 0;
            if (obj & 1) {
                owned = 1;
            }
            if (owned != 0) {
                obj = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int objCheck = obj;
            if (objCheck != 0) {
                char *type = *(char **)(objCheck + 4);
                GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)obj + slot->offset, out);
            } else {
                cStrCat(out, (const char *)0x36DB24);
            }
        } else {
            int obj = *(int *)((const char *)self + 0x18);
            int owned = 0;
            int objTag = obj & 1;
            if (objTag != 0) {
                owned = 1;
            }

            int useFallback;
            if (owned == 0) {
                goto object_not_owned;
            }
            useFallback = 1;
            goto object_fallback_done;
        object_not_owned:
            int zero = obj == 0;
            zero &= 0xFF;
            useFallback = zero != 0;
        object_fallback_done:

            if (useFallback != 0) {
                cStrAppend(out, (const char *)0x36DACC);
            } else {
                local[0] = '\0';

                int ownedAgain = 0;
                if (objTag != 0) {
                    ownedAgain = 1;
                }

                char *type;
                int base = obj;
                if (ownedAgain != 0) {
                    base = 0;
                    type = *(char **)(base + 4);
                } else {
                    type = *(char **)(base + 4);
                }

                GetTextSlot *slot = (GetTextSlot *)(type + 0x40);
                ((void (*)(void *, char *))slot->fn)(
                    (char *)base + slot->offset, local);
                cStrAppend(out, local);
            }
        }
        cStrAppend(out, close);
    }
}
