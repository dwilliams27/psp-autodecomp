#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class cGUID {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoLoadMap {
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

extern char gcDoLoadMapvirtualtable[];
void gcAction_gcAction(gcDoLoadMap *, cBase *);
void gcAction_Write(const gcDoLoadMap *, cFile &);

cBase *gcDoLoadMap::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLoadMap *result = 0;
    gcDoLoadMap *obj = (gcDoLoadMap *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLoadMapvirtualtable;
        ((int *)obj)[3] = 0;
        *((unsigned char *)obj + 0x10) = 0;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = 0;
        *((unsigned char *)obj + 0x20) = 0;
        ((int *)obj)[9] = (int)obj | 1;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)obj | 1;
        ((int *)obj)[13] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoLoadMap::Write(cFile &file) const {
    cWriteBlock wb(file, 8);
    gcAction_Write(this, file);
    wb.Write(*(bool *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    ((cGUID *)((char *)this + 0x18))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x20));
    ((gcDesiredValue *)((char *)this + 0x24))->Write(wb);
    ((cGUID *)((char *)this + 0x28))->Write(wb);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x30))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x34))->Write(wb);
    wb.End();
}
