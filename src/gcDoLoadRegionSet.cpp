class cBase;
class cMemPool;

class gcDoLoadRegionSet {
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

extern char D_00003938[];

void gcAction_gcAction(void *, cBase *);
extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");

cBase *gcDoLoadRegionSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLoadRegionSet *result = 0;
    gcDoLoadRegionSet *obj =
        (gcDoLoadRegionSet *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = D_00003938;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)((unsigned int)obj | 1);
        ((int *)obj)[5] = ((int *)obj)[4];
        gcEvent_ctor((char *)obj + 0x18, (cBase *)obj, (const char *)0x36EBE8);
        gcEvent_ctor((char *)obj + 0x34, (cBase *)obj, (const char *)0x36EBF8);
        ((char *)obj)[0x50] = 0;
        ((char *)obj)[0x51] = 0;
        result = obj;
    }
    return (cBase *)result;
}
