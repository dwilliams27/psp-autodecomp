#ifndef ESPHERESHAPE_H
#define ESPHERESHAPE_H

struct mVec3;
struct mOCS;
class eMultiSphereShape;
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
};

#endif
