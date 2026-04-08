#ifndef ETRIANGLESHAPE_H
#define ETRIANGLESHAPE_H

class mSphere;
class mCollideInfo;
class eCollisionInfo;
class eCollisionContactInfo;
class mVec3;
class eContactCollector;
class eShape;
class mOCS;
class mPlane;
class eDragAreaUtil;

class eTriangleShape {
public:
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const;
    int GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    float GetVolume(void) const;
    void GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *, mVec3 *) const;
};

#endif
