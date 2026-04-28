// gcDoReadFile.cpp - decompiled from gcDoReadFile.obj.
//
// Functions:
//   0x001C3FA4 gcDoReadFile::New(cMemPool *, cBase *) static  152B
//   0x001C4154 gcDoReadFile::Write(cFile &) const             148B

class cBase;
class cFile;
class cMemPool;

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
    unsigned int mField0;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
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

extern char gcDoReadFilevirtualtable[];

class gcDoReadFile : public gcAction {
public:
    int mFieldC;
    gcDesiredValue mDesiredValue;
    unsigned int mBase;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

cBase *gcDoReadFile::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoReadFile *result = 0;
    gcDoReadFile *obj = (gcDoReadFile *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoReadFilevirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        *(unsigned int *)((char *)obj + 0x10) = tagged;
        *(unsigned int *)((char *)obj + 0x14) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoReadFile::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(mFieldC);
    mDesiredValue.Write(wb);
    int val = mBase;
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
