#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoSaveGameOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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

void gcDoSaveGameOp::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    ((const gcAction *)this)->Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    wb.End();
}
