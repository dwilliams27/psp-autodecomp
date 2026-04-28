// gcDesiredPath - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredPath::Write(cFile &) const                 @ 0x00129628
//   gcDesiredPath::New(cMemPool *, cBase *) static      @ 0x0026bb34
//   gcDesiredPath::GetType(void) const                  @ 0x0026bbd4

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
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

void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char gcDesiredObjectT_gcDesiredPath_gcDesiredPathHelper_ePath_virtualtable[];
extern char gcDesiredEventParamsvirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F460;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredPath : public gcDesiredObject {
public:
    int mType;
    cHandle mHandle;

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

void gcDesiredPath::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);
    cWriteBlock inner(*wb._file, 3);
    inner.Write(mType);
    mHandle.Write(inner);
    inner.End();
    wb.End();
}

cBase *gcDesiredPath::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredPath *result = 0;
    gcDesiredPath *obj = (gcDesiredPath *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] = gcDesiredObjectT_gcDesiredPath_gcDesiredPathHelper_ePath_virtualtable;
        *(int *)((char *)obj + 12) = 7;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = gcDesiredEventParamsvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDesiredPath::GetType(void) const {
    if (D_0009F460 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F460 = cType::InitializeType(0, 0, 0x13F, D_0009F3F4,
                                           &gcDesiredPath::New,
                                           0, 0, 0);
    }
    return D_0009F460;
}
