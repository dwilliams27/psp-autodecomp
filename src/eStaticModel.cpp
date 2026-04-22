#include "eStaticModel.h"

class cBase;
class cMemPool;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class ePhysics {
public:
    static ePhysics *Get(void);
    void InvalidateCacheEntries(eGeom *);
};

extern "C" {
    void eStaticModel__eStaticModel_cBaseptr(void *self, cBase *parent);
}

extern char eStaticModelvirtualtable[];

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── eStaticModel::OnRemovedFromWorld(void) @ 0x00043324 ──
void eStaticModel::OnRemovedFromWorld(void) {
    ePhysics::Get()->InvalidateCacheEntries(this);
}

// ── eStaticModel::eStaticModel(cBase *) @ 0x00041F14 ──
eStaticModel::eStaticModel(cBase *base) : eGeom(base) {
    *(void **)((char *)this + 4) = eStaticModelvirtualtable;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0x94) = 0;
    *(int *)((char *)this + 0x98) = 0;
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
