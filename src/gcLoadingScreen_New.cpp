class cBase;
class cMemPool;

void gcLoadingScreen_gcLoadingScreen(void *, cBase *);

class cFactory {
public:
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

class gcLoadingScreen : public cFactory {
public:
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcLoadingScreen::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLoadingScreen *result = 0;
    gcLoadingScreen *obj = (gcLoadingScreen *)entry->fn(base, 0xA0, 4, 0, 0);
    if (obj != 0) {
        gcLoadingScreen_gcLoadingScreen(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
