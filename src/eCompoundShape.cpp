#include "eCompoundShape.h"
#include "eCollision.h"

// ── eCompoundShape::eCompoundShape(cBase *) @ 0x00072dd4 ──
eCompoundShape::eCompoundShape(cBase *b) : eShape(b) {
    *(void **)((char *)this + 4) = (void *)0x3839F0;
    mSubShapes = 0;
    mSelf = this;
}

// ── eCompoundShape::Collide(const eCompoundShape *, ...) @ 0x00073ac8 ──
int eCompoundShape::Collide(const eCompoundShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundCompound(*this, *other, a, b, ocs1, ocs2, info);
}

// ── eCompoundShape::Collide(const eMeshShape *, ...) @ 0x00073ae4 ──
int eCompoundShape::Collide(const eMeshShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundMesh(*this, *other, a, b, ocs1, ocs2, info);
}

// ── eCompoundShape::Collide(const eHeightmapShape *, ...) @ 0x00073b00 ──
int eCompoundShape::Collide(const eHeightmapShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundHeightmap(*this, *other, a, b, ocs1, ocs2, info);
}
