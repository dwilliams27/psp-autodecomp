#include "cBase.h"

class cMemPool;

class gcDoQueryNavMesh {
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

extern char gcDoQueryNavMeshvirtualtable[];
void gcAction_gcAction(gcDoQueryNavMesh *, cBase *);

cBase *gcDoQueryNavMesh::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoQueryNavMesh *result = 0;
    gcDoQueryNavMesh *obj = (gcDoQueryNavMesh *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoQueryNavMeshvirtualtable;
        ((int *)obj)[3] = (int)obj | 1;
        ((int *)obj)[4] = (int)obj | 1;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = (int)obj | 1;
        ((short *)obj)[24] = 0;
        ((short *)obj)[25] = 0;
        *((unsigned char *)obj + 0x1C) = 0;
        ((short *)obj)[36] = 0;
        ((short *)obj)[37] = 0;
        *((unsigned char *)obj + 0x34) = 0;
        ((int *)obj)[19] = 0;
        result = obj;
    }
    return (cBase *)result;
}
