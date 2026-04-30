// gcValLobbyGameInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x00348c28  New(cMemPool *, cBase *) static
//   0x00348dd8  Write(cFile &) const
//
// Class layout (20 bytes, alloc size 0x14):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mField8 (int — written by Write between gcValue::Write and the gcDesiredValue::Write)
//   [0x0C] embedded gcDesiredValue (4 bytes; self-init to (this | 1) in New)
//   [0x10] mField10 (int — defaults to 5 in New)

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

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

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

template <class T> T *dcast(const cBase *);

struct CopyEntry {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

extern char gcValuevirtualtable[];
extern char gcValLobbyGameInfovirtualtable[];

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    gcValue(cBase *parent);
    void Write(cFile &) const;
};

class gcValLobbyGameInfo : public gcValue {
public:
    int mField8;
    int mFieldC;       // embedded gcDesiredValue (single self-pointer field)
    int mField10;

    gcValLobbyGameInfo(cBase *parent);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

inline gcValue::gcValue(cBase *parent) {
    mVtable = gcValuevirtualtable;
    mParent = parent;
}

inline gcValLobbyGameInfo::gcValLobbyGameInfo(cBase *parent) : gcValue(parent) {
    mVtable = gcValLobbyGameInfovirtualtable;
    mField8 = 0;
    mFieldC = (int)this | 1;
    mField10 = 5;
}

inline void *operator new(unsigned, void *p) { return p; }

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValLobbyGameInfo::New(cMemPool *, cBase *) static @ 0x00348c28 ──
cBase *gcValLobbyGameInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValLobbyGameInfo *result = 0;
    gcValLobbyGameInfo *obj = (gcValLobbyGameInfo *)e->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValLobbyGameInfo(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyGameInfo::Write(cFile &) const @ 0x00348dd8 ──
void gcValLobbyGameInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mField10);
    ((gcDesiredValue *)((char *)this + 12))->Write(wb);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyGameInfo;

// ── gcValLobbyGameInfo::GetType(void) const @ 0x00348cc0 ──
const cType *gcValLobbyGameInfo::GetType(void) const {
    if (!type_gcValLobbyGameInfo) {
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
        type_gcValLobbyGameInfo = cType::InitializeType(
            0, 0, 0x186, type_value, gcValLobbyGameInfo::New, 0, 0, 0);
    }
    return type_gcValLobbyGameInfo;
}

// ── gcValLobbyGameInfo::AssignCopy(const cBase *) @ 0x00348ad4 ──
void gcValLobbyGameInfo::AssignCopy(const cBase *base) {
    void (*temp_a2_3)(void *, int);
    short temp_a2_4;
    short temp_a3;
    cBase *(*temp_a3_2)(void *, cMemPool *, cBase *);
    int temp_a2;
    int temp_a0;
    int temp_a0_4;
    int var_a1;
    int var_a1_2;
    int var_a2;
    int temp_s2;
    int temp_s3;
    int var_a1_3;
    cMemPool *temp_a1;
    int var_a2_2;
    int var_a0_2;
    void *temp_a2_2;
    gcValLobbyGameInfo *temp_v0;

    temp_v0 = dcast<gcValLobbyGameInfo>(base);
    mField8 = temp_v0->mField8;
    if ((char *)temp_v0 + 0xC != (char *)this + 0xC) {
        temp_a2 = mFieldC;
        var_a1 = 1;
        temp_a0 = temp_a2 & 1;
        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        if (var_a1 != 0) {
            var_a1_2 = 0;
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
            mFieldC = var_a2;
            if (temp_a2 != 0) {
                temp_a2_2 = *(void **)(temp_a2 + 4);
                temp_a3 = *(short *)((char *)temp_a2_2 + 0x50);
                temp_a2_3 =
                    *(void (**)(void *, int))((char *)temp_a2_2 + 0x54);
                temp_a2_3((char *)temp_a2 + temp_a3, 3);
            }
        }

        temp_s3 = temp_v0->mFieldC;
        var_a1_3 = 1;
        if (temp_s3 & 1) {
            var_a1_3 = 0;
        }
        if (var_a1_3 != 0) {
            temp_s2 = *(int *)(temp_s3 + 4) + 0x10;
            temp_a2_4 = *(short *)temp_s2;
            temp_a1 = cMemPool::GetPoolFromPtr((char *)this + 0xC);
            temp_a0_4 = mFieldC;
            var_a2_2 = 0;
            if (temp_a0_4 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                var_a0_2 = temp_a0_4 & ~1;
            } else {
                var_a0_2 = *(int *)temp_a0_4;
            }
            temp_a3_2 =
                *(cBase * (**)(void *, cMemPool *, cBase *))(temp_s2 + 4);
            mFieldC =
                (int)temp_a3_2((char *)temp_s3 + temp_a2_4, temp_a1,
                               (cBase *)var_a0_2);
        }
    }
    mField10 = temp_v0->mField10;
}
