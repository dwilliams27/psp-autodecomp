#include "cBase.h"

class cMemPool;

class gcDoSaveGameOp {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoSaveGameOp *, cBase *);
extern char gcDoSaveGameOpvirtualtable[];

cBase *gcDoSaveGameOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoSaveGameOp *result = 0;
    gcDoSaveGameOp *obj = (gcDoSaveGameOp *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSaveGameOpvirtualtable;
        ((int *)obj)[3] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[4] = encoded;
        ((int *)obj)[5] = encoded;
        ((int *)obj)[6] = encoded;
        ((int *)obj)[7] = encoded;
        result = obj;
    }
    return (cBase *)result;
}
