#ifndef ECAPSULESHAPE_H
#define ECAPSULESHAPE_H

class cBase;
class cFile;
class cMemPool;
class cType;
struct mVec3;
struct mOCS;
class eBoxShape;
class eSphereShape;
class eMultiSphereShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;

class eCapsuleShape {
public:
    char _pad[0x80];
    float radius;       // offset 0x80
    float halfHeight;   // offset 0x84

    eCapsuleShape(cBase *);

    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    int CanSweep(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    int NeedsRollingFriction(float *) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;

    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eCapsuleShape *New(cMemPool *, cBase *);
};

#endif
