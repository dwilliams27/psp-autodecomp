#include "eShape.h"
#include "eCollision.h"

int eShape::Collide(const eBoxShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}
