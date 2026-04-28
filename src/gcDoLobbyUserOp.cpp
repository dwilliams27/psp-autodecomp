// gcDoLobbyUserOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e61d8 gcDoLobbyUserOp::New(cMemPool *, cBase *) static  156B
//   0x002e638c gcDoLobbyUserOp::Write(cFile &) const             124B

#include "cBase.h"

class cMemPool;
class cFile;

class gcDoLobbyUserOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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

void gcAction_gcAction(gcDoLobbyUserOp *, cBase *);
void gcAction_Write(const gcDoLobbyUserOp *, cFile &);
extern char gcDoLobbyUserOpvirtualtable[];

// 0x002e61d8 — New(cMemPool *, cBase *) static
cBase *gcDoLobbyUserOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyUserOp *result = 0;
    gcDoLobbyUserOp *obj = (gcDoLobbyUserOp *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyUserOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = (int)obj | 1;
        *((char *)obj + 24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002e638c — Write(cFile &) const
void gcDoLobbyUserOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x18));
    wb.End();
}
