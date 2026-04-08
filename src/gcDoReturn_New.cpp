#include "gcDoReturn.h"
#include "cBase.h"

void gcAction_gcAction(gcDoReturn *, cBase *);

extern char gcDoReturnvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoReturn::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoReturn *result = 0;
    gcDoReturn *obj = (gcDoReturn *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoReturnvirtualtable;
        ((int *)obj)[3] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
