#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mCollideHit;

// mSphere: bounding sphere (16 bytes)
// Confirmed from mSphere::CalcEmbeddedContact (0x001a1544):
//   quadword load at offset 0x00 (center xyz), lwc1 at offset 0x0C (radius)
// The radius is stored in the w component of the quadword (same as mVec3 layout)
struct mSphere {
    union {
        SceULong128 qw;
        struct {
            float x, y, z, radius;
        };
    };

    int CalcEmbeddedContact(const mSphere &, mCollideHit *) const;
};
