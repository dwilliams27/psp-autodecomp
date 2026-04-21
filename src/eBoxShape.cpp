#include "eBoxShape.h"
#include "eSphereShape.h"
#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mOCS.h"

class eConvexHullShape;
class cFile;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

extern char eBoxShapevirtualtable[];

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

// eBoxShape::Write(cFile &) const — 0x0006d368
#pragma control sched=1
void eBoxShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape::Write(file);
    wb.Write(3, mHalfExtents);
    wb.End();
}
#pragma control sched=2
