#ifndef ESPHERESHAPE_H
#define ESPHERESHAPE_H

struct mVec3;
struct mOCS;
class eMultiSphereShape;
class eCapsuleShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;

class eSphereShape {
public:
    char _pad[0x74];
    float mRadius;  // 0x74

    int CanSweep(void) const;
    float GetVolume(void) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;
    void GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const;
    void GetInertialTensor(float mass, mVec3 *out) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

#endif
