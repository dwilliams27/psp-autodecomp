// gcDoUISetSprite - gcAll_psp.obj
//   0x00310560 gcDoUISetSprite::New(cMemPool *, cBase *) static

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

extern char gcDoUISetSpritevirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];

class gcDoUISetSprite {
public:
    static cBase *New(cMemPool *, cBase *);
};

// 0x00310560 - gcDoUISetSprite::New(cMemPool *, cBase *) static
cBase *gcDoUISetSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetSprite *result = 0;
    gcDoUISetSprite *obj =
        (gcDoUISetSprite *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISetSpritevirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0x0C, 1);
        ((int *)obj)[6] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x1C, obj);
        ((void **)obj)[8] = D_000006F8;
        ((int *)obj)[10] = 7;
        ((int *)obj)[11] = 0;
        ((void **)obj)[8] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}
