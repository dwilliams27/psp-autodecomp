#pragma once

#include "eShape.h"

class cBase;
class cFile;
class cMemPool;
class eBoxShape;
class eCapsuleShape;
class eCollisionContactInfo;
class eCompoundShape;
class eConvexHullShape;
class eCylinderShape;
class eHeightmapShape;
class eMeshShape;
class eMultiSphereShape;
class eShape;
class eSphereShape;
class mBox;
class mCollideInfo;
class mOCS;
class mRay;
class mSphere;
class mVec3;
class eContactCollector;
class eCollisionInfo;
class eDragAreaUtil;
class mPlane;

// eCompoundShape inherits eShape.
// eShape ctor stores cBase* at +0x00, vtable at +0x04, fields up to ~0x78.
// eCompoundShape ctor (0x72dd4) calls eShape::eShape, then:
//   sets vtable at +0x04
//   stores 0 at +0x80 (sub-shapes array pointer)
//   stores this at +0x84 (self-pointer / sentinel)
// GetAABB iterates sub-shapes via array at +0x80.
// GetNumSubShapes reads array count from *(array_ptr - 4).
// GetSubShape indexes into array at +0x80.
// Struct size: at least 0x88 bytes.
class eCompoundShape : public eShape {
public:
    eCompoundShape(cBase *);
    ~eCompoundShape();

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    void AssignCopy(const cBase *);
    static eCompoundShape *New(cMemPool *, cBase *);

    // virtual overrides from eShape
    void GetAABB(mBox *, const mOCS &) const;
    int CastRay(const mRay &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    int CastSphere(const mSphere &, const mCollideInfo &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const;
    void GetSweptContacts(int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *) const;
    void GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const;

    int Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eBoxShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMultiSphereShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCapsuleShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eConvexHullShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eCompoundShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;
    int Collide(const eHeightmapShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const;

    void GetInertialTensor(float, mVec3 *) const;
    float GetVolume() const;
    void GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *, mVec3 *) const;
    void GetBoxShape(const mOCS &, eBoxShape *, mOCS *) const;
    void GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const;
    int GetNumSubShapes() const;
    eShape *GetSubShape(int) const;

    // eShape base _pad00 covers offsets 0x00..0x7F
    // +0x80: sub-shapes array (pointer to cArray of eShape*)
    // +0x84: self-pointer (set to this in ctor)
    eShape **mSubShapes;        // 0x80 (inherited _pad00 is 0x80 bytes from eShape)
    eCompoundShape *mSelf;      // 0x84
};
