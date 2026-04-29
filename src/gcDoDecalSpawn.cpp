#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoDecalSpawn {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoDecalSpawn *, cBase *);
void gcAction_Write(const gcDoDecalSpawn *, cFile &);
extern char gcDoDecalSpawnvirtualtable[];

cBase *gcDoDecalSpawn::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoDecalSpawn *result = 0;
    gcDoDecalSpawn *obj = (gcDoDecalSpawn *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoDecalSpawnvirtualtable;
        *(float *)((char *)obj + 0x0C) = 0.1f;
        *(bool *)((char *)obj + 0x10) = true;
        ((int *)obj)[5] = 0;
        ((int *)obj)[6] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoDecalSpawn::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    wb.Write(*(bool *)((const char *)this + 0x10));
    wb.Write(*(float *)((const char *)this + 0x0C));
    ((const cHandle *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    wb.End();
}
