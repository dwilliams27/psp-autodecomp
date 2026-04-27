// gcEntityGeomConfig — gcAll_psp.obj.
// Decompiled functions:
//   0x00267384  gcEntityGeomConfig::New(cMemPool *, cBase *) static     (124B)
//   0x00267400  gcEntityGeomConfig::GetType(void) const                 (160B)
//   0x002675FC  gcEntityGeomConfig::GetControllerType(void) const       (8B)
//   0x0026791C  gcEntityGeomConfig::~gcEntityGeomConfig(void)           (100B)

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" {
    void gcEntityGeomConfig__gcEntityGeomConfig_cBaseptr(void *self, cBase *parent);
}

class gcEntityGeomConfig {
public:
    ~gcEntityGeomConfig();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int GetControllerType(void) const;

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcEntityGeomConfig_cBase_vtable[];   // 0x37E6A8

extern cType *D_000385DC;
extern cType *D_0009F43C;

// ============================================================
// 0x00267384 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityGeomConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityGeomConfig *result = 0;
    gcEntityGeomConfig *obj =
        (gcEntityGeomConfig *)entry->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcEntityGeomConfig__gcEntityGeomConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00267400 — GetType(void) const
// ============================================================
const cType *gcEntityGeomConfig::GetType(void) const {
    if (D_0009F43C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                           &gcEntityGeomConfig::New,
                                           0, 0, 0);
    }
    return D_0009F43C;
}

// ============================================================
// 0x002675FC — GetControllerType(void) const
// ============================================================
int gcEntityGeomConfig::GetControllerType(void) const {
    return 0;
}

// ============================================================
// 0x0026791C — ~gcEntityGeomConfig(void)  (deleting destructor)
// ============================================================
gcEntityGeomConfig::~gcEntityGeomConfig() {
    *(void **)((char *)this + 4) = gcEntityGeomConfig_cBase_vtable;
}
