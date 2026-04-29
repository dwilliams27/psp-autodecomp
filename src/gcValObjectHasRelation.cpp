class cBase;
class cMemPool;

extern char cBaseclassdesc[];
extern char D_000098C0[];
extern char gcDesiredEnumerationEntryvirtualtable[];

struct AllocBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValObjectHasRelation {
public:
    static cBase *New(cMemPool *, cBase *);
};

// ── gcValObjectHasRelation::New(cMemPool *, cBase *) static @ 0x00355768 ──
cBase *gcValObjectHasRelation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectHasRelation *result = 0;
    gcValObjectHasRelation *obj =
        (gcValObjectHasRelation *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = D_000098C0;
        int flagged = (int)obj | 1;
        *(void **)((char *)obj + 0x10) = cBaseclassdesc;
        *(int *)((char *)obj + 8) = flagged;
        *(gcValObjectHasRelation **)((char *)obj + 0xC) = obj;
        *(void **)((char *)obj + 0x10) = gcDesiredEnumerationEntryvirtualtable;
        *(unsigned char *)((char *)obj + 0x14) = 1;
        *(unsigned char *)((char *)obj + 0x15) = 0;
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = 0;
        *(int *)((char *)obj + 0x20) = flagged;
        *(unsigned char *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}
