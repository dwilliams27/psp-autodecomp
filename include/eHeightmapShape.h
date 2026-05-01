#ifndef EHEIGHTMAPSHAPE_H
#define EHEIGHTMAPSHAPE_H

class cBase;
class cFile;
class cMemPool;
class cType;
class eBoxShape;
class eCapsuleShape;
class eCollisionContactInfo;
class eCompoundShape;
class eConvexHullShape;
class eCylinderShape;
class eMeshShape;
class eMultiSphereShape;
class eShape;
class eSphereShape;
class mBox;
class mOCS;
class mVec3;

class eHeightmapShape {
public:
    eHeightmapShape(cBase *);

    const cType *GetType(void) const;
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    void GetAABB(mBox *, const mOCS &) const;
    void GetInertialTensor(float, mVec3 *) const;
    int GetTileIndex(void) const;
    int GetTileTriCount(void) const;
    float GetVolume(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static eHeightmapShape *New(cMemPool *, cBase *);
};

#endif
