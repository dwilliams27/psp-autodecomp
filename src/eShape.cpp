#include "eShape.h"
#include "eCollision.h"

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
