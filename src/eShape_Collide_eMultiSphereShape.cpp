#include "eShape.h"
#include "eCollision.h"

int eShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeMultiSphere(*this, *(const eMultiSphereShape *)shape, ocs1, ocs2, info);
}
