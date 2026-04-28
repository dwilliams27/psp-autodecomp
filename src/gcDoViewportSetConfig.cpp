// gcDoViewportSetConfig.cpp -- decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00317a98 gcDoViewportSetConfig::New(cMemPool *, cBase *) static
//   0x00317c4c gcDoViewportSetConfig::Write(cFile &) const
//   0x00238d70 gcEnumeration::New(cMemPool *, cBase *) static

class cBase;
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
    unsigned int mValue;
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;       // 0x00
    void *mVTable;       // 0x04
    unsigned int mNext;  // 0x08

    void Write(cFile &) const;
};

class cObject {
public:
    cObject(cBase *);
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

extern "C" {
    void gcAction_gcAction(void *, cBase *);
    void cObject_cObject(void *, cBase *);
}

extern char gcDoViewportSetConfigvirtualtable[];
extern char gcEnumerationvirtualtable[];

class gcDoViewportSetConfig : public gcAction {
public:
    int mFieldC;             // 0x0C
    int mField10;            // 0x10
    cHandle mHandle;         // 0x14
    gcDesiredValue mDesired; // 0x18

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcEnumeration : public cObject {
public:
    char pad[0x44];
    void *mArrayData;        // 0x44
    void *mArrayOwner;       // 0x48
    unsigned char mField4C;  // 0x4C
    char pad4D[3];
    int mField50;            // 0x50
    int mHandle;             // 0x54

    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoViewportSetConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoViewportSetConfig *result = 0;
    gcDoViewportSetConfig *obj =
        (gcDoViewportSetConfig *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoViewportSetConfigvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        ((int *)obj)[5] = 0;
        ((unsigned int *)obj)[6] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoViewportSetConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);
    wb.Write(mFieldC);
    wb.Write(mField10);
    mHandle.Write(wb);
    mDesired.Write(wb);
    wb.End();
}

cBase *gcEnumeration::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEnumeration *result = 0;
    gcEnumeration *obj = (gcEnumeration *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = gcEnumerationvirtualtable;
        obj->mArrayData = 0;
        obj->mArrayOwner = obj;
        obj->mField4C = 0;
        obj->mField50 = 0;
        obj->mHandle = 0;
        result = obj;
    }
    return (cBase *)result;
}
