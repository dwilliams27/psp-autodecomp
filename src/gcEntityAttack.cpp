class cBase;
class cMemPool;

class gcEntityAttack {
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

class gcEntityAnimationConfig;

void gcEntityAnimationConfig_ctor(gcEntityAnimationConfig *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);

extern char cBaseclassdesc[];
extern char gcEntityAttackvirtualtable[];
extern const char gcEntityAttack_event_name[];

cBase *gcEntityAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAttack *result = 0;
    gcEntityAttack *obj =
        (gcEntityAttack *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAttackvirtualtable;
        gcEntityAnimationConfig_ctor((gcEntityAnimationConfig *)((char *)obj + 8), (cBase *)obj);
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = 0;
        ((int *)obj)[13] = -1;
        gcEvent_gcEvent((char *)obj + 0x38, (cBase *)obj, gcEntityAttack_event_name);
        *(char *)((char *)obj + 0x54) = 0;
        __asm__ volatile("" : : "m"(*(char *)((char *)obj + 0x54)));
        ((void **)obj)[23] = cBaseclassdesc;
        *(char *)((char *)obj + 0x55) = 0;
        ((gcEntityAttack **)obj)[22] = obj;
        int desiredVTable = 0x388568;
        int one = 1;
        ((int *)obj)[23] = desiredVTable;
        *(char *)((char *)obj + 0x60) = one;
        *(char *)((char *)obj + 0x61) = 0;
        ((int *)obj)[25] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[26] = 0;
        ((int *)obj)[27] = encoded;
        ((int *)obj)[28] = 0;
        ((gcEntityAttack **)obj)[29] = obj;
        result = obj;
    }
    return (cBase *)result;
}
