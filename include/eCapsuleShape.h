#ifndef ECAPSULESHAPE_H
#define ECAPSULESHAPE_H

class cBase;
struct mVec3;
struct mOCS;
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

    int CanSweep(void) const;
    void GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const;

    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

#endif
