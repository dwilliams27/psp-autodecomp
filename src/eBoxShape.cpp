#include "eBoxShape.h"
#include "eSphereShape.h"
#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class eConvexHullShape;
class cFile;
class cBase;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

template <class T> T *dcast(const cBase *);

extern char eBoxShapevirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BD8;

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct eBoxShape_block_18 {
    int _[6];
};

int eBoxShape::Collide(const eBoxShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxBox(*this, *shape, ocs1, ocs2, info);
}

int eBoxShape::Collide(const eSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxSphere(*this, *shape, ocs1, ocs2, info);
}

int eBoxShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxMultiSphere(*this, *shape, ocs1, ocs2, info);
}

int eBoxShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxCapsule(*this, *shape, ocs1, ocs2, info);
}

int eBoxShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxConvexHull(*this, *shape, ocs1, ocs2, info);
}

// eBoxShape::Collide(const eCompoundShape *, ...) — 0x0006dd80
int eBoxShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxCompound(*this, *shape, b, ocs1, ocs2, info);
}

// eBoxShape::Collide(const eMeshShape *, ...) — 0x0006dda8
int eBoxShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxMesh(*this, *shape, b, ocs1, ocs2, info);
}

// eBoxShape::Collide(const eHeightmapShape *, ...) — 0x0006ddd0
int eBoxShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

// eBoxShape::eBoxShape(cBase *) — 0x0006d4b4
eBoxShape::eBoxShape(cBase *parent) : eShape(parent) {
    *(void **)((char *)this + 4) = eBoxShapevirtualtable;
}

// eBoxShape::~eBoxShape(void) — 0x0006d4e8
#pragma control sched=1
inline void eBoxShape::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
    short off = rec->offset;
    __asm__ volatile("" ::: "memory");
    char *base = block + off;
    void (*fn)(void *, void *) = rec->fn;
    fn(base, p);
}

eBoxShape::~eBoxShape() {
    *(void **)((char *)this + 4) = eBoxShapevirtualtable;
}

// eBoxShape::New(cMemPool *, cBase *) static — 0x0020a394
eBoxShape *eBoxShape::New(cMemPool *pool, cBase *parent) {
    eBoxShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBoxShape *obj = (eBoxShape *)entry->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eBoxShape(parent);
        result = obj;
    }
    return result;
}

// eBoxShape::AssignCopy(const cBase *) @ 0x0020a2e4
void eBoxShape::AssignCopy(const cBase *src) {
    eBoxShape *other = dcast<eBoxShape>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) =
        *(unsigned char *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(eBoxShape_block_18 *)((char *)this + 0x54) =
        *(eBoxShape_block_18 *)((char *)other + 0x54);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    *(v4sf_t *)((char *)this + 0x80) = *(v4sf_t *)((char *)other + 0x80);
}
#pragma control sched=2

// eBoxShape::Write(cFile &) const — 0x0006d368
#pragma control sched=1
void eBoxShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape::Write(file);
    wb.Write(3, mHalfExtents);
    wb.End();
}
#pragma control sched=2

// eBoxShape::GetType(void) const — 0x0020a410
#pragma control sched=1
const cType *eBoxShape::GetType(void) const {
    if (D_00046BD8 == 0) {
        if (D_00040FE4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00040FE4 = cType::InitializeType(0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00040FE4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20A394;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BD8 = cType::InitializeType(0, 0, 0x229, parentType, factory, 0, 0, 0);
    }
    return D_00046BD8;
}
#pragma control sched=2
