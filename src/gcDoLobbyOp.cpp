#include "cBase.h"

class cMemPool;
class cFile;

class gcDoLobbyOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoLobbyOp *, cBase *);
void gcAction_Write(const gcDoLobbyOp *, cFile &);
extern char gcDoLobbyOpvirtualtable[];

cBase *gcDoLobbyOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoLobbyOp *result = 0;
    gcDoLobbyOp *obj = (gcDoLobbyOp *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyOpvirtualtable;
        ((int *)obj)[3] = 1;
        int encoded = (int)obj | 1;
        ((int *)obj)[4] = encoded;
        ((int *)obj)[5] = encoded;
        ((int *)obj)[6] = encoded;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoLobbyOp::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);

    int val = ((int *)this)[6];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}
