#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mQuat;
struct mVec3;

// mBasis: 3x3 rotation matrix stored as 3 quadword rows (48 bytes, 0x30)
// Confirmed from mBasis::SetQuat (0x0019fb80): quadword stores at this+0x00, this+0x10, this+0x20
// Confirmed from mBasis::Orthonormalize (0x001a020c): quadword loads/stores at same offsets
struct mBasis {
    SceULong128 row0;  // offset 0x00
    SceULong128 row1;  // offset 0x10
    SceULong128 row2;  // offset 0x20

    void SetQuat(const mQuat &);
    void Orthonormalize(void);
    void GetAxisAngle(mVec3 *, float *) const;
};
