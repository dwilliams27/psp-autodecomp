// gcValLobbyFriendInfo.cpp — decompiled from gcAll_psp.obj
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

class gcValLobbyFriendInfo : public gcValue {
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
extern char gcValLobbyFriendInfovirtualtable[];        // @ 0x18AB8 (linker reloc)

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

// ── Write(cFile &) const  @ 0x00348030 ──
void gcValLobbyFriendInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(field_8);
    wb.Write(field_10);
    field_C.Write(wb);
    wb.End();
}

// ── New(cMemPool *, cBase *) static  @ 0x00347e84 ──
cBase *gcValLobbyFriendInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyFriendInfo *result = 0;
    gcValLobbyFriendInfo *obj = (gcValLobbyFriendInfo *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyFriendInfovirtualtable;
        *(int *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = (int)obj | 1;
        *(int *)((char *)obj + 0x10) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── AssignCopy(const cBase *)  @ 0x00347d30 ──
void gcValLobbyFriendInfo::AssignCopy(const cBase *base) {
    gcValLobbyFriendInfo *other = dcast<gcValLobbyFriendInfo>(base);
    int *temp_s2 = (int *)((char *)this + 12);

    field_8 = other->field_8;
    if ((int *)((char *)other + 12) != temp_s2) {
        int temp_a2 = *temp_s2;
        int var_a1;
        var_a1 = 1;
        int temp_a0 = temp_a2 & 1;
        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        if (var_a1 != 0) {
            int var_a1_2 = 0;
            int var_a2;
            if (temp_a0 != 0) {
                var_a1_2 = 1;
            }
            if (var_a1_2 != 0) {
                var_a2 = temp_a2 & ~1;
                var_a2 |= 1;
            } else {
                var_a2 = *(int *)temp_a2;
                var_a2 |= 1;
            }
            *temp_s2 = var_a2;
            if (temp_a2 != 0) {
                ReleaseEntry *entry =
                    (ReleaseEntry *)(*(char **)(temp_a2 + 4) + 0x50);
                entry->fn((char *)temp_a2 + entry->offset, 3);
            }
        }

        int temp_a0_2 = *(int *)((char *)other + 12);
        int var_a1_3 = 1;
        if (temp_a0_2 & 1) {
            var_a1_3 = 0;
        }
        if (var_a1_3 != 0) {
            void *temp_a0_3 = *(void **)(temp_a0_2 + 4);
            short temp_a2_4 = *(short *)((char *)temp_a0_3 + 0x10);
            int temp_a0_4 = *temp_s2;
            cMemPool *temp_a1 = cMemPool::GetPoolFromPtr(temp_s2);
            int var_a2_2 = 0;
            int var_a0;
            if (temp_a0_4 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                var_a0 = temp_a0_4 & ~1;
            } else {
                var_a0 = *(int *)temp_a0_4;
            }
            *temp_s2 =
                (int)((CloneEntry *)((char *)temp_a0_3 + 0x10))
                    ->fn((char *)temp_a0_2 + temp_a2_4, temp_a1,
                         (cBase *)var_a0);
        }
    }
    field_10 = other->field_10;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyFriendInfo;

// ── GetType(void) const  @ 0x00347f18 ──
const cType *gcValLobbyFriendInfo::GetType(void) const {
    if (!type_gcValLobbyFriendInfo) {
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
        type_gcValLobbyFriendInfo = cType::InitializeType(
            0, 0, 0x1C5, type_value, gcValLobbyFriendInfo::New, 0, 0, 0);
    }
    return type_gcValLobbyFriendInfo;
}
