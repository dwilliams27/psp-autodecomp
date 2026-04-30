// gcValNetworkState — decompiled from gcAll_psp.obj.
// Methods in this file:
//   gcValNetworkState::New(cMemPool *, cBase *) static  @ 0x00352db4 (144B)
//   gcValNetworkState::Write(cFile &) const             @ 0x00352f5c (100B)
//
// Class layout (16 bytes, alloc size 0x10):
//   [0x00] parent (cBase *)
//   [0x04] vtable
//   [0x08] mField8 (int, defaults to 0 in New)
//   [0x0C] gcDesiredValue (4 bytes; init'd as (this|1))

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *, short);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcValNetworkStatevirtualtable[];
extern char gcValNetworkState_cBase_vtable[];

class gcValNetworkState : public gcValue {
public:
    int pad0;
    int pad4;
    int mField8;
    int mField0C;

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

// ── gcValNetworkState::Write(cFile &) const @ 0x00352f5c ──
void gcValNetworkState::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mField8);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.End();
}

// ── gcValNetworkState::New(cMemPool *, cBase *) static @ 0x00352db4 ──
cBase *gcValNetworkState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValNetworkState *result = 0;
    gcValNetworkState *obj =
        (gcValNetworkState *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValNetworkState_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValNetworkStatevirtualtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcValNetworkState::AssignCopy(const cBase *base) {
    void (*temp_a2_3)(void *, int);
    short temp_a1_2;
    short temp_a3;
    cBase *(*temp_a3_2)(void *, cMemPool *, cBase *);
    int temp_a0;
    int temp_a0_2;
    int temp_a2;
    int temp_s1;
    int temp_s2;
    int var_a0;
    int var_a0_2;
    int var_a1;
    int var_a1_2;
    int var_a2;
    int var_a2_2;
    cMemPool *temp_v0_2;
    void *temp_a1;
    void *temp_a2_2;
    gcValNetworkState *temp_v0;

    temp_v0 = dcast<gcValNetworkState>(base);
    temp_s1 = (int)this + 0xC;
    *(int *)((char *)this + 8) = *(int *)((char *)temp_v0 + 8);
    if ((int)temp_v0 + 0xC != temp_s1) {
        temp_a2 = *(int *)temp_s1;
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
            } else {
                var_a2 = *(int *)temp_a2;
            }
            *(int *)temp_s1 = var_a2 | 1;
            if (temp_a2 != 0) {
                temp_a2_2 = *(void **)(temp_a2 + 4);
                temp_a3 = *(short *)((char *)temp_a2_2 + 0x50);
                temp_a2_3 =
                    *(void (**)(void *, int))((char *)temp_a2_2 + 0x54);
                temp_a2_3((char *)temp_a2 + temp_a3, 3);
            }
        }

        temp_s2 = *(int *)((char *)temp_v0 + 0xC);
        var_a0 = 1;
        if (temp_s2 & 1) {
            var_a0 = 0;
        }
        if (var_a0 != 0) {
            temp_a1 = *(void **)(temp_s2 + 4);
            temp_a1_2 = *(short *)((char *)temp_a1 + 0x10);
            temp_v0_2 = cMemPool::GetPoolFromPtr((void *)temp_s1, temp_a1_2);
            temp_a0_2 = *(int *)temp_s1;
            var_a2_2 = 0;
            if (temp_a0_2 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                var_a0_2 = temp_a0_2 & ~1;
            } else {
                var_a0_2 = *(int *)temp_a0_2;
            }
            temp_a3_2 =
                *(cBase * (**)(void *, cMemPool *, cBase *))((char *)temp_a1 +
                                                             0x14);
            *(int *)temp_s1 = (int)temp_a3_2((char *)temp_s2 + temp_a1_2,
                                             temp_v0_2, (cBase *)var_a0_2);
        }
    }
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValNetworkState;

const cType *gcValNetworkState::GetType(void) const {
    if (!type_gcValNetworkState) {
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
        type_gcValNetworkState = cType::InitializeType(
            0, 0, 0x14A, type_value, gcValNetworkState::New, 0, 0, 0);
    }
    return type_gcValNetworkState;
}
