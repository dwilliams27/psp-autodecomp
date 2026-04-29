// gcDoUIEditBoxOp — gcAll_psp.obj
//   0x00309070  gcDoUIEditBoxOp::New(cMemPool *, cBase *) static

class cBase;
class cMemPool;

class gcDesiredUIWidgetHelper {
public:
    gcDesiredUIWidgetHelper(int);
};

class gcDoUIEditBoxOp {
public:
    static cBase *New(cMemPool *, cBase *);
};

void gcAction_gcAction(gcDoUIEditBoxOp *, cBase *);
void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);

extern char gcDoUIEditBoxOpvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoUIEditBoxOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIEditBoxOp *result = 0;
    gcDoUIEditBoxOp *obj = (gcDoUIEditBoxOp *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUIEditBoxOpvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 0x0C), 1);
        ((int *)obj)[6] = 3;
        int tagged = (int)obj | 1;
        ((int *)obj)[7] = tagged;
        ((int *)obj)[8] = tagged;
        result = obj;
    }
    return (cBase *)result;
}
