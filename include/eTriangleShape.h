#ifndef ETRIANGLESHAPE_H
#define ETRIANGLESHAPE_H

class mSphere;
class mCollideInfo;
class eCollisionInfo;
class mVec3;

class eTriangleShape {
public:
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
};

#endif
