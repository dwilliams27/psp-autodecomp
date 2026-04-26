#ifndef ESHAPE_H
#define ESHAPE_H

class cBase;
class cType;
class eBoxShape;
class eSphereShape;
class eMultiSphereShape;
class eCapsuleShape;
class eCylinderShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;
class mOCS;
class mVec3;
class mCollideInfo;

// eShape base class for all collision shapes.
// Layout inferred from eShape::GetSurface (+0x6C), eCompoundShape ctor
// (base fields up to +0x7F), and matched source files.
class cFile;

class eShape {
public:
    char _pad00[0x80];  // 0x00 — includes vtable, cBase*, surface, and other fields

    eShape(cBase *);

    int GetNumSubShapes(void) const;
    const eShape *GetSubShape(int) const;
    int IsSubShape(void) const;
    void *GetSurface(void) const;
    int CanSweep(void) const;

    int GetCollisionHandler(const mCollideInfo &) const;
    int GetCollisionHandler(const mVec3 &) const;
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    int NeedsRollingFriction(float *) const;

    void Write(cFile &) const;

    const cType *GetType(void) const;

    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
};

#endif
