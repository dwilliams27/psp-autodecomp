// gcDoLobbyMailOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e16c4 gcDoLobbyMailOp::New(cMemPool *, cBase *) static  156B
//   0x002e1878 gcDoLobbyMailOp::Write(cFile &) const             112B

#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyMailOp {
public:
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    ~gcDoLobbyMailOp(void);
    void Write(cFile &) const;
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
    void Write(cWriteBlock &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

void gcAction_gcAction(gcDoLobbyMailOp *, cBase *);
void gcAction_Write(const gcDoLobbyMailOp *, cFile &);
void *cMemPool_GetPoolFromPtr(const void *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoLobbyMailOpvirtualtable[];
extern const char gcDoLobbyMailOp_base_name[] asm("D_0036D894");
extern const char gcDoLobbyMailOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyMailOp asm("D_0009F694");

inline void gcDoLobbyMailOp::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

// ── gcDoLobbyMailOp::New(cMemPool *, cBase *) @ 0x002e16c4 ──
cBase *gcDoLobbyMailOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyMailOp *result = 0;
    gcDoLobbyMailOp *obj = (gcDoLobbyMailOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyMailOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 4;
        ((int *)obj)[5] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoLobbyMailOp::GetType(void) const @ 0x002e1760 ──
const cType *gcDoLobbyMailOp::GetType(void) const {
    if (!type_gcDoLobbyMailOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoLobbyMailOp_base_name, gcDoLobbyMailOp_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyMailOp = cType::InitializeType(
            0, 0, 0x1F3, type_action, gcDoLobbyMailOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyMailOp;
}

// ── gcDoLobbyMailOp::Write(cFile &) @ 0x002e1878 ──
void gcDoLobbyMailOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── gcDoLobbyMailOp::~gcDoLobbyMailOp(void) @ 0x002e1f00 ──
gcDoLobbyMailOp::~gcDoLobbyMailOp(void) {
    *(void **)((char *)this + 4) = gcDoLobbyMailOpvirtualtable;

    if ((void *)((char *)this + 0x14) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
