// gcDoLobbyFriendOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e09e4 gcDoLobbyFriendOp::New(cMemPool *, cBase *) static  156B
//   0x002e0b98 gcDoLobbyFriendOp::Write(cFile &) const             112B

#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyFriendOp {
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
    void Write(bool);
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

void gcAction_gcAction(gcDoLobbyFriendOp *, cBase *);
void gcAction_Write(const gcDoLobbyFriendOp *, cFile &);
extern char gcDoLobbyFriendOpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyFriendOp asm("D_0009F690");

// ── gcDoLobbyFriendOp::New(cMemPool *, cBase *) @ 0x002e09e4 ──
cBase *gcDoLobbyFriendOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyFriendOp *result = 0;
    gcDoLobbyFriendOp *obj = (gcDoLobbyFriendOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyFriendOpvirtualtable;
        ((int *)obj)[3] = 2;
        ((int *)obj)[4] = (int)obj | 1;
        *((char *)obj + 20) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoLobbyFriendOp::GetType(void) const @ 0x002e0a80 ──
const cType *gcDoLobbyFriendOp::GetType(void) const {
    if (!type_gcDoLobbyFriendOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyFriendOp = cType::InitializeType(
            0, 0, 0x1C6, type_action, gcDoLobbyFriendOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyFriendOp;
}

// ── gcDoLobbyFriendOp::Write(cFile &) @ 0x002e0b98 ──
void gcDoLobbyFriendOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x14));
    wb.End();
}
