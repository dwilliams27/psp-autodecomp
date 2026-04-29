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

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoLobbyConfigureGame *, cBase *);
void gcAction_Write(const gcDoLobbyConfigureGame *, cFile &);
extern char gcDoLobbyConfigureGamevirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyConfigureGame asm("D_0009F68C");

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
