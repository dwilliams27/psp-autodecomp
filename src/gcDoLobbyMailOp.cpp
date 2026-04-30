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
    const cType *GetType(void) const;
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

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoLobbyMailOp *, cBase *);
void gcAction_Write(const gcDoLobbyMailOp *, cFile &);
extern char gcDoLobbyMailOpvirtualtable[];
extern const char gcDoLobbyMailOp_base_name[] asm("D_0036D894");
extern const char gcDoLobbyMailOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyMailOp asm("D_0009F694");

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
