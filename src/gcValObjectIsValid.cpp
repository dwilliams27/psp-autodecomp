// gcValObjectIsValid.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00356628 gcValObjectIsValid::New(cMemPool *, cBase *) static  140B

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

class gcValue {
public:
    cBase *mParent;
    void *mClassDesc;
};

class gcValObjectIsValid : public gcValue {
public:
    int mObject;

    static cBase *New(cMemPool *, cBase *);
};

extern char cBaseclassdesc[];
extern char gcValObjectIsValidvirtualtable[];

// -- gcValObjectIsValid::New @ 0x00356628 --
cBase *gcValObjectIsValid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectIsValid *result = 0;
    gcValObjectIsValid *obj =
        (gcValObjectIsValid *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValObjectIsValidvirtualtable;
        obj->mObject = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
