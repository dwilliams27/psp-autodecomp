#ifndef ESPHERESHAPE_H
#define ESPHERESHAPE_H

struct mVec3;
struct mOCS;
struct mBox;
class cBase;
class cFile;
class cMemPool;
class cType;
class eBoxShape;
class eCollisionSupport;
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

    eSphereShape(cBase *);

    void AssignCopy(const cBase *);
    int CanSweep(void) const;
    const cType *GetType(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    int NeedsRollingFriction(float *) const;
    void GetAABB(mBox *, const mOCS &) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, eCollisionSupport *out) const;
    void GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const;
    void GetInertialTensor(float mass, mVec3 *out) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eSphereShape *New(cMemPool *, cBase *);
};

#endif
