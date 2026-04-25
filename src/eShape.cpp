#include "eShape.h"
#include "eCollision.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"

class cBase;
class cMemPool;

extern char eShapevirtualtable[];

class cMemPool_shim {
public:
    static void *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
    void free(void *);
}

int eShape::GetNumSubShapes(void) const {
    return 1;
}

const eShape *eShape::GetSubShape(int) const {
    return this;
}

int eShape::IsSubShape(void) const {
    return *((const unsigned char *)this + 0x50);
}

void *eShape::GetSurface(void) const {
    return *(void **)((const char *)this + 0x6C);
}

int eShape::CanSweep(void) const {
    return 0;
}

int eShape::Collide(const eSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeMultiSphere(*this, *shape, ocs1, ocs2, info);
}

int eShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eCylinderShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

// eShape::Collide(const eCompoundShape *, ...) — 0x0002bac8
int eShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeCompound(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::Collide(const eMeshShape *, ...) — 0x0002baf0
int eShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeMesh(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::Collide(const eHeightmapShape *, ...) — 0x0002bb18
int eShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::eShape(cBase *) — 0x0002b92c
// Stores cBase* at +0x00, eShape vtable at +0x04, zero-init various fields,
// sets float +0x78 = 0.0f and float +0x74 = -1.0f.
eShape::eShape(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = eShapevirtualtable;
    *((char *)this + 0x50) = 0;
    *(short *)((char *)this + 0x68) = 0;
    *(short *)((char *)this + 0x6A) = 0;
    *((char *)this + 0x54) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(float *)((char *)this + 0x74) = -1.0f;
    *(float *)((char *)this + 0x78) = 0.0f;
    __asm__ volatile("" ::: "memory");
}

// eShape::~eShape(void) — 0x0002b970
// Sets vtable to cDynamicMemAllocatorvirtualtable-8 (0x37E6A8), then if flags & 1,
// delegates to mempool alloc-table destroy fn, falling back to free().
extern "C" void eShape___dtor_eShape_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = (void *)0x37E6A8;
        if (flags & 1) {
            void *pool = cMemPool_shim::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                char *allocTable = *(char **)((char *)block + 0x1C);
                DeleteRec *rec = (DeleteRec *)(allocTable + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}
