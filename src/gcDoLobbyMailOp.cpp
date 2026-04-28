// gcDoLobbyMailOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e16c4 gcDoLobbyMailOp::New(cMemPool *, cBase *) static  156B
//   0x002e1878 gcDoLobbyMailOp::Write(cFile &) const             112B

#include "cBase.h"

class cMemPool;
class cFile;

class gcDoLobbyMailOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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

// ── gcDoLobbyMailOp::Write(cFile &) @ 0x002e1878 ──
void gcDoLobbyMailOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    wb.End();
}
