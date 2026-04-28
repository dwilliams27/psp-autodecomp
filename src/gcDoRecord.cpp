// gcDoRecord.cpp - decompiled from gcDoRecord.obj.
//
// Functions:
//   0x001C4E24 gcDoRecord::New(cMemPool *, cBase *) static  152B

class cBase;
class cMemPool;

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
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

extern "C" void gcAction_gcAction(void *, cBase *);

extern char gcDoRecordvirtualtable[];

class gcDoRecord : public gcAction {
public:
    int mFieldC;
    unsigned int mField10;
    unsigned int mField14;

    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoRecord::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoRecord *result = 0;
    gcDoRecord *obj = (gcDoRecord *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoRecordvirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        *(unsigned int *)((char *)obj + 0x14) = tagged;
        result = obj;
    }
    return (cBase *)result;
}
