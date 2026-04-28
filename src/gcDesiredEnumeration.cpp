inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cObject;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    cBase *mParent;
    void *mVTable;
    unsigned int mNext;

    gcDesiredObject(cBase *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char gcDesiredEnumerationvirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F450;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredEnumeration : public gcDesiredObject {
public:
    int mType;
    cHandle mHandle;

    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

cObject *gcDesiredEnumeration::GetObject(bool b) const {
    return Get(b);
}

cBase *gcDesiredEnumeration::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEnumeration *result = 0;
    gcDesiredEnumeration *obj = (gcDesiredEnumeration *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new ((gcDesiredObject *)obj) gcDesiredObject(parent);
        obj->mVTable = gcDesiredEnumerationvirtualtable;
        obj->mType = 7;
        obj->mHandle.mId = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDesiredEnumeration::GetType(void) const {
    if (D_0009F450 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
        }
        D_0009F450 = cType::InitializeType(0, 0, 0x2C1, D_0009F3F4,
                                           &gcDesiredEnumeration::New, 0, 0, 0);
    }
    return D_0009F450;
}

void gcDesiredEnumeration::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);
    cWriteBlock inner(*wb._file, 3);
    inner.Write(mType);
    mHandle.Write(inner);
    inner.End();
    wb.End();
}
