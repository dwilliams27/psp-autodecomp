#include "cBase.h"

class cMemPool;

class gcDoLobbyScoreboardOp {
public:
    static cBase *New(cMemPool *, cBase *);
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

void gcAction_gcAction(gcDoLobbyScoreboardOp *, cBase *);
extern char gcDoLobbyScoreboardOpvirtualtable[];

cBase *gcDoLobbyScoreboardOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyScoreboardOp *result = 0;
    gcDoLobbyScoreboardOp *obj = (gcDoLobbyScoreboardOp *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyScoreboardOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = (int)obj | 1;
        ((int *)obj)[7] = (int)obj | 1;
        ((int *)obj)[8] = (int)obj | 1;
        ((int *)obj)[9] = (int)obj | 1;
        ((int *)obj)[10] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
