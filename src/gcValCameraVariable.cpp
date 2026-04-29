#include "cBase.h"

class cMemPool;
class cFile;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcLValue {
public:
    void Write(cFile &) const;
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);
};

class gcDesiredEntity {
public:
    char _pad[0x2C];
    gcDesiredEntity &operator=(const gcDesiredEntity &);
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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValCameraVariable {
public:
    char _pad0[0x10];
    int mField10;
    gcDesiredEntity mDesiredEntity;
    unsigned char mField40;
    char _pad41[3];
    int mField44;

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    ~gcValCameraVariable(void);
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];
extern char gcValCameraVariablevirtualtable[];
void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredCamera___dtor_gcDesiredCamera_void(void *, int);

struct WriteRecord {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

// 0x003223ac, 88B
void gcValCameraVariable::AssignCopy(const cBase *base) {
    gcValCameraVariable *other = dcast<gcValCameraVariable>(base);
    mField10 = other->mField10;
    mDesiredEntity.operator=(other->mDesiredEntity);
    mField40 = other->mField40;
    mField44 = other->mField44;
}

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

// 0x003225f8, 132B
void gcValCameraVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcLValue *)this)->Write(file);
    char *typeInfo = *(char **)((const char *)this + 12);
    WriteRecord *rec = (WriteRecord *)(typeInfo + 0x28);
    char *base = (char *)this + 8;
    rec->fn(base + rec->offset, wb.file);
    wb.Write(mField40);
    wb.Write(mField44);
    wb.End();
}

// 0x00322f2c, 136B
gcValCameraVariable::~gcValCameraVariable(void) {
    *(void **)((char *)this + 4) = gcValCameraVariablevirtualtable;
    gcDesiredCamera___dtor_gcDesiredCamera_void((char *)this + 8, 2);
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
