// eCapsuleShape::NeedsRollingFriction(float *) const — 0x00209884
//
// Trivial leaf: stores the capsule's radius (offset 0x80) into *out and
// returns 1. The volatile-store pattern (mirrors eSphereShape::NeedsRollingFriction)
// forces SNC to keep the swc1 immediately after the lwc1, putting `li v0, 1`
// in the jr delay slot.
#include "eCapsuleShape.h"

int eCapsuleShape::NeedsRollingFriction(float *out) const {
    *(volatile float *)out = radius;
    return 1;
}
