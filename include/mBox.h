#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3;

// mBox: axis-aligned bounding box (32 bytes, 0x20)
// Confirmed from mBox::Extend (0x001a0a74): quadword accesses at offsets 0x00 and 0x10
// Two mVec3 members: min and max corners
struct mBox {
    SceULong128 min;  // offset 0x00 — mVec3
    SceULong128 max;  // offset 0x10 — mVec3

    void Extend(int, const mVec3 *, int);
    void Extend(const mBox &);
    void GetCorners(mVec3 *) const;
    void XformInv(const mBox &, const struct mOCS &);
};
