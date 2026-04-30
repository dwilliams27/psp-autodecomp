// gcDoLobbyConfigureGame.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002dfc90 gcDoLobbyConfigureGame::New(cMemPool *, cBase *) static  152B
//   0x002dfe40 gcDoLobbyConfigureGame::Write(cFile &) const             100B

#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyConfigureGame {
public:
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    ~gcDoLobbyConfigureGame(void);
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
    void Write(unsigned int);
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

void gcAction_gcAction(gcDoLobbyConfigureGame *, cBase *);
void gcAction_Write(const gcDoLobbyConfigureGame *, cFile &);
void *cMemPool_GetPoolFromPtr(const void *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoLobbyConfigureGamevirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyConfigureGame asm("D_0009F68C");

inline void gcDoLobbyConfigureGame::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

// ── gcDoLobbyConfigureGame::New(cMemPool *, cBase *) @ 0x002dfc90 ──
cBase *gcDoLobbyConfigureGame::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyConfigureGame *result = 0;
    gcDoLobbyConfigureGame *obj = (gcDoLobbyConfigureGame *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyConfigureGamevirtualtable;
        ((int *)obj)[3] = 1;
        ((int *)obj)[4] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoLobbyConfigureGame::GetType(void) const @ 0x002dfd28 ──
const cType *gcDoLobbyConfigureGame::GetType(void) const {
    if (!type_gcDoLobbyConfigureGame) {
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
        type_gcDoLobbyConfigureGame = cType::InitializeType(
            0, 0, 0x18A, type_action, gcDoLobbyConfigureGame::New, 0, 0, 0);
    }
    return type_gcDoLobbyConfigureGame;
}

// ── gcDoLobbyConfigureGame::Write(cFile &) @ 0x002dfe40 ──
void gcDoLobbyConfigureGame::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(unsigned int *)((char *)this + 0x0C));
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoLobbyConfigureGame::~gcDoLobbyConfigureGame(void) {
    *(void **)((char *)this + 4) = gcDoLobbyConfigureGamevirtualtable;

    if ((void *)((char *)this + 0x10) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x10) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
