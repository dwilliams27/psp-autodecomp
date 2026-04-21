#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mOCS.h"

class cBase;

extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
}

extern char eHeightmapShapevirtualtable[];

float eHeightmapShape::GetVolume(void) const {
    return 0.0f;
}

// eHeightmapShape::eHeightmapShape(cBase *) — 0x00050684
eHeightmapShape::eHeightmapShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr(this, parent);
    *(void **)((char *)this + 4) = eHeightmapShapevirtualtable;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
}

#pragma control sched=1

// eHeightmapShape::Collide(const eCapsuleShape *, ...) — 0x0005143c
int eHeightmapShape::Collide(const eCapsuleShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eCylinderShape *, ...) — 0x00051468
int eHeightmapShape::Collide(const eCylinderShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeHeightmap(*(const eShape *)shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eConvexHullShape *, ...) — 0x00051494
int eHeightmapShape::Collide(const eConvexHullShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ConvexHullHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eCompoundShape *, ...) — 0x000514c0
int eHeightmapShape::Collide(const eCompoundShape *shape, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundHeightmap(*shape, *this, a, b, ocs2, ocs1, info);
}

#pragma control sched=2
