#include "gcDoReturn.h"
#include "cBase.h"

void gcAction_gcAction(gcDoStateParentHandler *, cBase *);
extern char gcDoStateParentHandlervirtualtable[];

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

cBase *gcDoStateParentHandler::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoStateParentHandler *result = 0;
    gcDoStateParentHandler *obj = (gcDoStateParentHandler *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoStateParentHandlervirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)obj;
        __asm__ volatile("ori $5, %0, 1\nsw $5, 20(%0)" :: "r"(obj) : "$5", "memory");
        cBaseArray_SetSize((char *)obj + 0x0C, 4);
        result = obj;
    }
    return (cBase *)result;
}
