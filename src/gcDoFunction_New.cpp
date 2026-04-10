#include "gcDoEntitySendMessage.h"
#include "cBase.h"

void gcAction_gcAction(gcDoFunction *, cBase *);
extern char gcDoFunctionvirtualtable[];

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

cBase *gcDoFunction::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoFunction *result = 0;
    gcDoFunction *obj = (gcDoFunction *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoFunctionvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = (int)obj;
        __asm__ volatile("" ::: "memory");
        __asm__ volatile(
            "ori $5, %0, 1\n"
            "sw $5, 0x18(%0)"
            : : "r"(obj) : "$5", "memory"
        );
        cBaseArray_SetSize((char *)obj + 0x10, 4);
        result = obj;
    }
    return (cBase *)result;
}
