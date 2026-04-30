// gcValConstant — gcAll_psp.obj.
//
// Functions in this file:
//   gcValConstant::Write(cFile &) const            @ 0x00325534  (88B)
//   gcValConstant::New(cMemPool *, cBase *) static @ 0x00325394 (136B)
//   gcValConstant::~gcValConstant(void)            @ 0x00325aac (100B)
//
// AssignCopy is matched separately in src/gcExternalCinematicGroup.cpp.
// Layout: derives from gcValue (no fields contributed). cBase header at
// offset 0/4 (parent + vtable). cHandle subobject at offset 8.

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcValConstant : public gcValue {
public:
    char _pad0[8];          // 0x00 — cBase parent + vtable
    cHandle mHandle;        // 0x08
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
}

extern char gcValConstantvirtualtable[];
extern char gcValConstant_cBase_vtable[];   // 0x37E6A8

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

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValConstant asm("D_0009F7F0");

// ── GetType @ 0x0032541c ───────────────────────────────────────────────
const cType *gcValConstant::GetType(void) const {
    if (!type_gcValConstant) {
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
        type_gcValConstant = cType::InitializeType(0, 0, 0x9E, type_value,
                                                   gcValConstant::New,
                                                   0, 0, 0);
    }
    return type_gcValConstant;
}

// ── Write @ 0x00325534 ─────────────────────────────────────────────────
void gcValConstant::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    this->mHandle.Write(wb);
    wb.End();
}

// ── New @ 0x00325394 ───────────────────────────────────────────────────
cBase *gcValConstant::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValConstant *result = 0;
    gcValConstant *obj = (gcValConstant *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValConstant_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValConstantvirtualtable;
        ((int *)obj)[2] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── ~gcValConstant @ 0x00325aac ────────────────────────────────────────
extern "C" void gcValConstant___dtor_gcValConstant_void(void *self, int flags) {
    if (self != 0) {
        ((char **)self)[1] = gcValConstant_cBase_vtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec =
                (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}
