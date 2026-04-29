#include "gcDoObjectForEachRelationship.h"

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(void *, cBase *);
void gcExpressionList_gcExpressionList(void *, void *);

extern char gcDoObjectForEachRelationshipvirtualtable[];
extern char gcDesiredEnumerationEntryvirtualtable[];

int gcDoObjectForEachRelationship::GetMaxBranches(void) const {
    return 1;
}

cBase *gcDoObjectForEachRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoObjectForEachRelationship *result = 0;
    gcDoObjectForEachRelationship *obj =
        (gcDoObjectForEachRelationship *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoObjectForEachRelationshipvirtualtable;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[3] = self;
        ((int *)obj)[5] = 0x37E6A8;
        ((gcDoObjectForEachRelationship **)obj)[4] = obj;
        ((void **)obj)[5] = gcDesiredEnumerationEntryvirtualtable;
        *((char *)obj + 0x18) = 1;
        *((char *)obj + 0x19) = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = self;
        gcExpressionList_gcExpressionList((char *)obj + 0x28, obj);
        result = obj;
    }
    return (cBase *)result;
}
