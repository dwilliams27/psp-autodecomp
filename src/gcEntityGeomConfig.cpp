// gcEntityGeomConfig — gcAll_psp.obj.
// Decompiled functions:
//   0x00267384  gcEntityGeomConfig::New(cMemPool *, cBase *) static     (124B)
//   0x00267400  gcEntityGeomConfig::GetType(void) const                 (160B)
//   0x002675FC  gcEntityGeomConfig::GetControllerType(void) const       (8B)
//   0x0026791C  gcEntityGeomConfig::~gcEntityGeomConfig(void)           (100B)
//   0x002672EC  gcEntityGeomConfig::AssignCopy(const cBase *)           (152B)
//   0x00125ACC  gcEntityGeomConfig::Write(cFile &) const                (132B)
//   0x0031CA38  gcGeomTrailConfig::AssignCopy(const cBase *)            (152B)

class cBase;
class cFile;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class cName {
public:
    int data[6];
    void Write(cWriteBlock &) const;
};

struct GeomConfigVec {
    v4sf_t vec;
};

extern "C" {
    void gcEntityGeomConfig__gcEntityGeomConfig_cBaseptr(void *self, cBase *parent);
}

class gcEntityGeomConfig {
public:
    char _base[8];
    cHandle mHandle;
    cName mNameData;
    int mInt24;
    char _pad28[8];
    GeomConfigVec mVec30;
    GeomConfigVec mVec40;
    bool mBool50;

    ~gcEntityGeomConfig();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int GetControllerType(void) const;

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class gcGeomTrailConfig {
public:
    char _base[8];
    cHandle mHandle;
    cName mNameData;
    int mInt24;
    char _pad28[8];
    GeomConfigVec mVec30;
    GeomConfigVec mVec40;
    bool mBool50;

    void AssignCopy(const cBase *);
};

template <class T> T *dcast(const cBase *);

extern char gcEntityGeomConfig_cBase_vtable[];   // 0x37E6A8

extern cType *D_000385DC;
extern cType *D_0009F43C;

// ============================================================
// 0x002672EC — AssignCopy(const cBase *)
// ============================================================
void gcEntityGeomConfig::AssignCopy(const cBase *src) {
    gcEntityGeomConfig *other = dcast<gcEntityGeomConfig>(src);
    int *srcHandle = &other->mHandle.mIndex;
    int handle = *srcHandle;
    int *dstHandle = &mHandle.mIndex;
    __asm__ volatile("" : "+r"(dstHandle));
    *dstHandle = handle;
    int *srcWords = other->mNameData.data;
    int *dstWords = mNameData.data;
    int word0 = srcWords[0];
    int word1 = srcWords[1];
    int word2 = srcWords[2];
    dstWords[0] = word0;
    word0 = srcWords[3];
    dstWords[1] = word1;
    word1 = srcWords[4];
    dstWords[2] = word2;
    word2 = srcWords[5];
    dstWords[3] = word0;
    dstWords[4] = word1;
    dstWords[5] = word2;
    int *srcInt = &other->mInt24;
    int int24 = *srcInt;
    int *dstInt = &mInt24;
    __asm__ volatile("" : "+r"(dstInt));
    *dstInt = int24;
    mVec30.vec = other->mVec30.vec;
    mVec40.vec = other->mVec40.vec;
    mBool50 = other->mBool50;
}

// ============================================================
// 0x00267384 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityGeomConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityGeomConfig *result = 0;
    gcEntityGeomConfig *obj =
        (gcEntityGeomConfig *)entry->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcEntityGeomConfig__gcEntityGeomConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00267400 — GetType(void) const
// ============================================================
const cType *gcEntityGeomConfig::GetType(void) const {
    if (D_0009F43C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                           &gcEntityGeomConfig::New,
                                           0, 0, 0);
    }
    return D_0009F43C;
}

// ============================================================
// 0x002675FC — GetControllerType(void) const
// ============================================================
int gcEntityGeomConfig::GetControllerType(void) const {
    return 0;
}

// ============================================================
// 0x00125ACC — Write(cFile &) const
// ============================================================
void gcEntityGeomConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    mHandle.Write(wb);
    mNameData.Write(wb);
    wb.Write(mInt24);
    wb.Write(3, (const float *)&mVec30);
    wb.Write(3, (const float *)&mVec40);
    wb.Write(mBool50);
    wb.End();
}

// ============================================================
// 0x0026791C — ~gcEntityGeomConfig(void)  (deleting destructor)
// ============================================================
gcEntityGeomConfig::~gcEntityGeomConfig() {
    *(void **)((char *)this + 4) = gcEntityGeomConfig_cBase_vtable;
}

// ============================================================
// 0x0031CA38 — gcGeomTrailConfig::AssignCopy(const cBase *)
// ============================================================
void gcGeomTrailConfig::AssignCopy(const cBase *src) {
    gcGeomTrailConfig *other = dcast<gcGeomTrailConfig>(src);
    int *srcHandle = &other->mHandle.mIndex;
    int handle = *srcHandle;
    int *dstHandle = &mHandle.mIndex;
    __asm__ volatile("" : "+r"(dstHandle));
    *dstHandle = handle;
    int *srcWords = other->mNameData.data;
    int *dstWords = mNameData.data;
    int word0 = srcWords[0];
    int word1 = srcWords[1];
    int word2 = srcWords[2];
    dstWords[0] = word0;
    word0 = srcWords[3];
    dstWords[1] = word1;
    word1 = srcWords[4];
    dstWords[2] = word2;
    word2 = srcWords[5];
    dstWords[3] = word0;
    dstWords[4] = word1;
    dstWords[5] = word2;
    int *srcInt = &other->mInt24;
    int int24 = *srcInt;
    int *dstInt = &mInt24;
    __asm__ volatile("" : "+r"(dstInt));
    *dstInt = int24;
    mVec30.vec = other->mVec30.vec;
    mVec40.vec = other->mVec40.vec;
    mBool50 = other->mBool50;
}
