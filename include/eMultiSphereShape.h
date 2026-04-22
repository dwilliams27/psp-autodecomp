#ifndef EMULTISPHERESHAPE_H
#define EMULTISPHERESHAPE_H

struct mVec3;
struct mOCS;
class eBoxShape;
class eCapsuleShape;
class eCompoundShape;
class eConvexHullShape;
class eHeightmapShape;
class eMeshShape;
class eCollisionContactInfo;
class cBase;
class cFile;
class cMemPool;

class eMultiSphereShape {
public:
    char _pad[0x80];
    float mRadius;       // 0x80
    float mHalfLength;   // 0x84

    eMultiSphereShape(cBase *);
    int CanSweep(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    void GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eMultiSphereShape *New(cMemPool *, cBase *);
};

#endif
