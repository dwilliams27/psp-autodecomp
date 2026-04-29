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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);
extern "C" void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);

extern char gcDoUISetStatevirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];

class gcDoUISetState {
public:
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoUISetState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetState *result = 0;
    gcDoUISetState *obj = (gcDoUISetState *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISetStatevirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0x0C, 1);
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0x1C) = tagged;
        *(int *)((char *)obj + 0x20) = tagged;
        *(int *)((char *)obj + 0x24) = tagged;
        *(int *)((char *)obj + 0x28) = tagged;
        *(int *)((char *)obj + 0x2C) = tagged;
        *(int *)((char *)obj + 0x30) = tagged;
        *(int *)((char *)obj + 0x34) = tagged;
        *(int *)((char *)obj + 0x38) = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x3C, obj);
        ((void **)obj)[0x40 / 4] = D_000006F8;
        *(int *)((char *)obj + 0x48) = 7;
        *(int *)((char *)obj + 0x4C) = 0;
        ((void **)obj)[0x40 / 4] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}
