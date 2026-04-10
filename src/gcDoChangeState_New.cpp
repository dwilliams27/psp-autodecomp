#include "gcDoChangeState.h"
#include "cBase.h"

void gcAction_gcAction(gcDoChangeState *, cBase *);
extern char gcDoChangeStatevirtualtable[];

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

cBase *gcDoChangeState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoChangeState *result = 0;
    gcDoChangeState *obj = (gcDoChangeState *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoChangeStatevirtualtable;
        ((int *)obj)[3] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = encoded;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = (int)obj;
        ((int *)obj)[8] = encoded;
        void *arr = (char *)obj + 0x18;
        ((char *)obj)[0x24] = 0;
        cBaseArray_SetSize(arr, 4);
        result = obj;
    }
    return (cBase *)result;
}
