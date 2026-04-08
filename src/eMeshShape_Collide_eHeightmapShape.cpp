#include "eMeshShape.h"
#include "eHeightmapShape.h"

int eMeshShape::Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const {
    return 0;
}
