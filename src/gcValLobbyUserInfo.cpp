// gcValLobbyUserInfo.cpp — decompiled from gcAll_psp.obj
//
// Class layout (size 0x14):
//   [0x00] cBase parent (cBase *)
//   [0x04] vtable
//   [0x08] int field_8
//   [0x0C] gcDesiredValue field_C
//   [0x10] int field_10

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
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
    void End(void);
};

class gcDesiredValue {
public:
    int _value;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void *_base[2];   // cBase parent + vtable
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
};

class gcValLobbyUserInfo : public gcValue {
public:
    int field_8;
    gcDesiredValue field_C;
    int field_10;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern char cBaseclassdesc[];                          // @ 0x37E6A8
extern char gcValLobbyUserInfovirtualtable[];          // @ 0x91C0 (linker reloc)

struct ePoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct CloneEntry {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

// ── Write(cFile &) const  @ 0x0034f0b4 ──
void gcValLobbyUserInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(field_8);
    wb.Write(field_10);
    field_C.Write(wb);
    wb.End();
}

// ── New(cMemPool *, cBase *) static  @ 0x0034ef08 ──
cBase *gcValLobbyUserInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyUserInfo *result = 0;
    gcValLobbyUserInfo *obj = (gcValLobbyUserInfo *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyUserInfovirtualtable;
        *(int *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = (int)obj | 1;
        *(int *)((char *)obj + 0x10) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── AssignCopy(const cBase *)  @ 0x0034edb4 ──
void gcValLobbyUserInfo::AssignCopy(const cBase *base) {
    gcValLobbyUserInfo *temp_v0 = dcast<gcValLobbyUserInfo>(base);
    int *temp_s2 = (int *)((char *)this + 0xC);

    *(int *)((char *)this + 8) = *(int *)((char *)temp_v0 + 8);
    if ((int *)((char *)temp_v0 + 0xC) != temp_s2) {
        int value = *temp_s2;
        int flag = 1;
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag != 0) {
            int old = value;
            int flag2 = 0;
            if (tag != 0) {
                flag2 = 1;
            }
            if (flag2 != 0) {
                value &= ~1;
                value |= 1;
            } else {
                value = *(int *)value;
                value |= 1;
            }
            *temp_s2 = value;
            if (old != 0) {
                ReleaseEntry *entry =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                entry->fn((char *)old + entry->offset, 3);
            }
        }

        value = *(int *)((char *)temp_v0 + 0xC);
        flag = 1;
        if (value & 1) {
            flag = 0;
        }
        if (flag != 0) {
            int source = value;
            CloneEntry *entry =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            cMemPool *pool = cMemPool::GetPoolFromPtr(temp_s2);
            int current = *(int *)((char *)this + 0xC);
            int flag2 = 0;
            if (current & 1) {
                flag2 = 1;
            }
            if (flag2 != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            *(int *)((char *)this + 0xC) =
                (int)entry->fn((char *)source + entry->offset, pool,
                               (cBase *)current);
        }
    }
    *(int *)((char *)this + 0x10) = *(int *)((char *)temp_v0 + 0x10);
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyUserInfo;

// ── GetType(void) const  @ 0x0034ef9c ──
const cType *gcValLobbyUserInfo::GetType(void) const {
    if (!type_gcValLobbyUserInfo) {
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
        type_gcValLobbyUserInfo = cType::InitializeType(
            0, 0, 0x1CA, type_value, gcValLobbyUserInfo::New, 0, 0, 0);
    }
    return type_gcValLobbyUserInfo;
}
