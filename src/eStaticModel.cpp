#include "eStaticModel.h"

class cBase;
class cFile;
class cType;
class eCollisionInfo;
class eContactCollector;
class eStaticMeshVisData;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eStaticMeshVisData {
public:
    int mData;
    eStaticMeshVisData(void);
    void Read(cReadBlock &, cMemPool *);
};

class ePhysics {
public:
    static ePhysics *Get(void);
    void InvalidateCacheEntries(eGeom *);
};

extern "C" {
    void eStaticModel__eStaticModel_cBaseptr(void *self, cBase *parent);
}

extern char eStaticModelvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469A4;
extern cType *D_000469B4;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct cGUID {
    int a;
    int b;
};

typedef int v4sf_t __attribute__((mode(V4SF)));

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    char _data[0x108];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(int, const float *);
    void End(void);
};

class eStaticGeom {
public:
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

void cFile_SetCurrentPos(void *, unsigned int);
void cMemPool_GetValue(cMemPool *, const cGUID &, void **)
    __asm__("__0fIcMemPoolIGetValueRC6FcGUIDPPv");

// ── eStaticModel::OnRemovedFromWorld(void) @ 0x00043324 ──
void eStaticModel::OnRemovedFromWorld(void) {
    ePhysics::Get()->InvalidateCacheEntries(this);
}

// ── eStaticModel::eStaticModel(cBase *) @ 0x00041F14 ──
eStaticModel::eStaticModel(cBase *base) : eGeom(base) {
    *(void **)((char *)this + 4) = eStaticModelvirtualtable;
    mNodeCullIdCount = 0;
    mNodeCullIdLODs.mData = 0;
    mField98 = 0;
}

// ── eStaticModel::Write(cFile &) const @ 0x00041AFC ──
void eStaticModel::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eStaticGeom *)this)->Write(file);
    wb.Write(3, (const float *)((const char *)this + 0x40));
    wb.Write(3, (const float *)((const char *)this + 0x10));
    wb.Write(3, (const float *)((const char *)this + 0x20));
    wb.Write(3, (const float *)((const char *)this + 0x30));
    wb.Write((unsigned int)mField98);
    wb.End();
}

// ── eStaticModel::GetType(void) const @ 0x001ED138 ──
const cType *eStaticModel::GetType(void) const {
    if (D_000469B4 == 0) {
        if (D_000469A4 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469A4 = cType::InitializeType(0, 0, 0x1A, D_00040FF4,
                                               0, 0, 0, 0);
        }
        D_000469B4 = cType::InitializeType(0, 0, 0x1D, D_000469A4,
                                           &eStaticModel::New, 0, 0, 0);
    }
    return D_000469B4;
}

// ── eStaticModel::PlatformRead(cFile &, cMemPool *) @ 0x00041CE4 ──
void eStaticModel::PlatformRead(cFile &file, cMemPool *pool) {
    cGUID guidArg;
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    cMemPool *visPool;
    cGUID guid;
    cGUID guidCopy;
    char pad[12];
    char hasVisData;
    eStaticMeshVisData *result;
    eStaticMeshVisData *obj;
    __asm__ volatile("" : : "m"(pad));
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }

    visPool = 0;
    guidCopy.a = 0x812D2B72;
    guidCopy.b = 0xB1E8A1F9;
    guid.a = 0x812D2B72;
    guid.b = 0xB1E8A1F9;
    guidArg = guid;
    cMemPool_GetValue(pool, guidArg, (void **)&visPool);

    cFileSystem::Read(*(void **)rb._data[0], &hasVisData, 1);
    if ((unsigned char)(hasVisData != 0) != 0) {
        void *block = ((void **)visPool)[9];
        AllocRec *rec = (AllocRec *)(((PoolBlock *)block)->allocTable + 0x28);
        short off = rec->offset;
        void *base = (char *)block + off;
        result = 0;
        obj = (eStaticMeshVisData *)rec->fn(base, 4, 0, 0, -1);
        if (obj != 0) {
            new (obj) eStaticMeshVisData;
            result = obj;
        }
        *(eStaticMeshVisData **)((char *)this + 0x90) = result;
        result->Read(rb, visPool);
    }
}

// ── eStaticModel::GetSweptContacts(...) const @ 0x0004271C ──
void eStaticModel::GetSweptContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, const mCollideInfo *ci, eContactCollector *cc) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xA8);
    ((void (*)(char *, int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *))entry[1])(
        shape + *(short *)entry, idx, sphere, ci, info, cc);
}

// ── eStaticModel::CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const @ 0x00042650 ──
void eStaticModel::CastRay(const eCollisionInfo &info, const mRay &ray, mCollideHit *hit) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0x98);
    ((void (*)(char *, const mRay &, const eCollisionInfo &, void *, void *, void *))entry[1])(
        shape + *(short *)entry, ray, info,
        (char *)hit + 0x10, (char *)hit + 0x20, (char *)hit + 0x30);
}

// ── eStaticModel::New(cMemPool *, cBase *) static @ 0x001ED0BC ──
cBase *eStaticModel::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eStaticModel *result = 0;
    eStaticModel *obj = (eStaticModel *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eStaticModel__eStaticModel_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eStaticModel::AssignCopy(const cBase *) @ 0x001ECFD4 ──
void eStaticModel::AssignCopy(const cBase *base) {
    eStaticModel *other = dcast<eStaticModel>(base);
    *(v4sf_t *)((char *)this + 0x40) = *(const v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(const v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(const v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);
    float value5C = *(const float *)((char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = value5C;
    *(int *)((char *)this + 0x60) = *(const int *)((char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(const int *)((char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(const int *)((char *)other + 0x68);
    *(int *)((char *)this + 0x6C) = *(const int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(const int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(const float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(const float *)((char *)other + 0x78);
    *(int *)((char *)this + 0x7C) = *(const int *)((char *)other + 0x7C);
    *(int *)((char *)this + 0x80) = *(const int *)((char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(const int *)((char *)other + 0x84);
    *(int *)((char *)this + 0x88) = *(const int *)((char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((char *)other + 0x8D);

    int value90 = other->mNodeCullIdCount;
    cArrayBase<eStaticModelNodeCullIdLOD> *dstArray = &mNodeCullIdLODs;
    const cArrayBase<eStaticModelNodeCullIdLOD> *srcArray = &other->mNodeCullIdLODs;
    mNodeCullIdCount = value90;
    *dstArray = *srcArray;
    mField98 = other->mField98;
}
