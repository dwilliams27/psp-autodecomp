// mBasis::GetAxisAngle(mVec3 *, float *) const @ 0x0019fc58 -> __0fGmBasisMGetAxisAngleP6FmVec3PfK
// Delegation: build the quaternion via GetQuat into a stack temp, then
// forward to mQuat::GetAxisAngle. GetAxisAngle is not declared in
// include/mBasis.h with this signature path, so the classes are declared
// locally (split-TU) to emit the exact mangled symbol.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3;

struct mQuat {
    SceULong128 q;  // 0x00
    void GetAxisAngle(mVec3 *, float *) const;
};

struct mBasis {
    SceULong128 row0;  // 0x00
    SceULong128 row1;  // 0x10
    SceULong128 row2;  // 0x20

    void GetQuat(mQuat *) const;
    void GetAxisAngle(mVec3 *, float *) const;
};

void mBasis::GetAxisAngle(mVec3 *axis, float *angle) const {
    mQuat quat;
    GetQuat(&quat);
    quat.GetAxisAngle(axis, angle);
}
