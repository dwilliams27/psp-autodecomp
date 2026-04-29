#include "cBase.h"

class cMemPool;

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);
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

class gcValCameraVariable {
public:
    char _pad[0x40];
    unsigned char mField40;
    int mField44;

    static cBase *New(cMemPool *, cBase *);
};

extern char cBaseclassdesc[];
extern char gcValCameraVariablevirtualtable[];
void gcDesiredCamera_gcDesiredCamera(void *, cBase *);

// 0x00322404, 160B
cBase *gcValCameraVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *rec = (AllocEntry *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValCameraVariable *result = 0;
    gcValCameraVariable *obj =
        (gcValCameraVariable *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)obj = parent;
        *(void **)((char *)obj + 4) = gcValCameraVariablevirtualtable;
        gcDesiredCamera_gcDesiredCamera((char *)obj + 8, (cBase *)obj);
        obj->mField40 = 0;
        obj->mField44 = 0;
        result = obj;
    }
    return (cBase *)result;
}
