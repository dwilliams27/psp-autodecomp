#include "cBase.h"

class cFile;
class cMemPool;
class cBase;
class gcDoSetEventObjectArray;

extern "C" void gcAction_gcAction(gcDoSetEventObjectArray *, cBase *);
extern "C" void gcAction_Write(const gcDoSetEventObjectArray *, cFile &);

extern char gcDoSetEventObjectArrayvirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoSetEventObjectArray {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
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

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::Write(cFile &) const @ 0x002fdc9c
// ----------------------------------------------------------------
void gcDoSetEventObjectArray::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((cBaseArray *)((char *)this + 16))->Write(wb);
    ((gcDesiredValue *)((char *)this + 24))->Write(wb);
    wb.End();
}

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::New(cMemPool *, cBase *) static @ 0x002fdae8
// ----------------------------------------------------------------
cBase *gcDoSetEventObjectArray::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetEventObjectArray *result = 0;
    gcDoSetEventObjectArray *obj =
        (gcDoSetEventObjectArray *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSetEventObjectArrayvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int **)obj)[5] = (int *)obj;
        ((int *)obj)[6] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
