// gcValRandomNumber -- decompiled from gcAll_psp.obj

class cBase;
class cMemPool;

extern char cBaseclassdesc[];
extern char gcValRandomNumbervirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValRandomNumber {
public:
    cBase *mParent;
    void *mVtable;
    int mField8;
    int mDesiredC;
    int mDesired10;
    unsigned char mField14;

    static cBase *New(cMemPool *, cBase *);
};

// 0x00357d48 -- gcValRandomNumber::New(cMemPool *, cBase *) static
cBase *gcValRandomNumber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValRandomNumber *result = 0;
    gcValRandomNumber *obj = (gcValRandomNumber *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValRandomNumbervirtualtable;
        int one = 1;
        int tagged = (int)obj | 1;
        ((int *)obj)[2] = one;
        ((int *)obj)[3] = tagged;
        ((int *)obj)[4] = tagged;
        ((unsigned char *)obj)[0x14] = 0;
        result = obj;
    }
    return (cBase *)result;
}
