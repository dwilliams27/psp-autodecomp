#include "cBase.h"

class cFile;
class cMemPool;
class gcDoSetEventObject;

extern "C" void gcAction_gcAction(void *, cBase *);
void gcAction_Write(const gcDoSetEventObject *, cFile &);

extern char gcDoSetEventObjectvirtualtable[];
extern char gcDoSetObjectRelationshipvirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDoSetEventObject {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcDoSetObjectRelationship {
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

cBase *gcDoSetEventObject::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetEventObject *result = 0;
    gcDoSetEventObject *obj =
        (gcDoSetEventObject *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSetEventObjectvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoSetEventObject::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction_Write(this, file);
    wb.Write(((const int *)this)[3]);
    int val = ((int *)this)[4];
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

cBase *gcDoSetObjectRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetObjectRelationship *result = 0;
    gcDoSetObjectRelationship *obj =
        (gcDoSetObjectRelationship *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSetObjectRelationshipvirtualtable;
        int self = (int)obj | 1;
        ((int *)obj)[3] = self;
        ((int *)obj)[4] = self;
        result = obj;
    }
    return (cBase *)result;
}
