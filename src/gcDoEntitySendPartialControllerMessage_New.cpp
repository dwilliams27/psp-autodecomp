#include "gcDoEntitySendMessage.h"
#include "cBase.h"

void gcAction_gcAction(gcDoEntitySendPartialControllerMessage *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoEntitySendPartialControllerMessage::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySendPartialControllerMessage *result = 0;
    gcDoEntitySendPartialControllerMessage *obj = (gcDoEntitySendPartialControllerMessage *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
