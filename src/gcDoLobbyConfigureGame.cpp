// gcDoLobbyConfigureGame.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002dfc90 gcDoLobbyConfigureGame::New(cMemPool *, cBase *) static  152B
//   0x002dfe40 gcDoLobbyConfigureGame::Write(cFile &) const             100B

#include "cBase.h"

class cMemPool;
class cFile;

class gcDoLobbyConfigureGame {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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

// ── gcDoLobbyConfigureGame::Write(cFile &) @ 0x002dfe40 ──
void gcDoLobbyConfigureGame::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(unsigned int *)((char *)this + 0x0C));
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    wb.End();
}
