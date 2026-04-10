#include "gcDoPlayCinematic.h"
#include "cBase.h"

void gcAction_gcAction(gcDoPlayCinematic *, cBase *);
extern char gcDoPlayCinematicvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void cBaseArray_SetSize(void *, int);

cBase *gcDoPlayCinematic::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayCinematic *result = 0;
    gcDoPlayCinematic *obj = (gcDoPlayCinematic *)entry->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlayCinematicvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = 0;
        ((int *)obj)[6] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[7] = 0;
        void *arr = (char *)obj + 0x24;
        ((int *)obj)[8] = encoded;
        ((int *)obj)[9] = 0;
        ((int *)obj)[10] = (int)obj;
        ((int *)obj)[11] = encoded;
        ((int *)obj)[12] = encoded;
        cBaseArray_SetSize(arr, 1);
        result = obj;
    }
    return (cBase *)result;
}
