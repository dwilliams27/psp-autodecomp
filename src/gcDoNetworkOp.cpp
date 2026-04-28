class cBase;
class cMemPool;

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
extern char gcDoNetworkOpvirtualtable[];

class gcDoNetworkOp {
public:
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoNetworkOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoNetworkOp *result = 0;
    gcDoNetworkOp *obj = (gcDoNetworkOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoNetworkOpvirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        *(int *)((char *)obj + 0x10) = (int)obj | 1;
        *(int *)((char *)obj + 0x14) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
