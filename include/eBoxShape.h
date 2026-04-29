#ifndef EBOXSHAPE_H
#define EBOXSHAPE_H

#include "eShape.h"

struct mOCS;
class cBase;
class cFile;
class cMemPool;
class eSphereShape;
class eMultiSphereShape;
class eCapsuleShape;
class eConvexHullShape;
class eCompoundShape;
class eMeshShape;
class eHeightmapShape;
class eCollisionContactInfo;

class eBoxShape : public eShape {
public:
    float mHalfExtents[4]; // 0x80 — quad-aligned (x,y,z,_)

    eBoxShape(cBase *);
    ~eBoxShape();

    float GetVolume(void) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;

    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    static eBoxShape *New(cMemPool *, cBase *);
    static void operator delete(void *);
};

#endif
