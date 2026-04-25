#ifndef ECOLLISION_H
#define ECOLLISION_H

class eCollisionSupport;
class eContactResult;
class eMaterialData;
class eCamera;
class mFrustum;
class eCameraBins;
template <class T> class cHandleT;

class eBoxShape;
class eCapsuleShape;
class eCollisionContactInfo;
class eCompoundShape;
class eConvexHullShape;
class eHeightmapShape;
class eMeshShape;
class eMultiSphereShape;
class eShape;
class eSphereShape;
class mOCS;
struct mVec3;

class eCollision {
public:
    static int BoxBox(const eBoxShape &, const eBoxShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxSphere(const eBoxShape &, const eSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxMultiSphere(const eBoxShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxCapsule(const eBoxShape &, const eCapsuleShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxConvexHull(const eBoxShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxCompound(const eBoxShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxMesh(const eBoxShape &, const eMeshShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int BoxHeightmap(const eBoxShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CapsuleCapsule(const eCapsuleShape &, const eCapsuleShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CapsuleConvexHull(const eCapsuleShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CapsuleCompound(const eCapsuleShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CapsuleMesh(const eCapsuleShape &, const eMeshShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CapsuleHeightmap(const eCapsuleShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CompoundCompound(const eCompoundShape &, const eCompoundShape &, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CompoundMesh(const eCompoundShape &, const eMeshShape &, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int CompoundHeightmap(const eCompoundShape &, const eHeightmapShape &, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int ConvexHullHeightmap(const eConvexHullShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int Clip(const eCollisionSupport *, const eCollisionSupport *, eContactResult *, unsigned int);
    static int ClipCircle(const eCollisionSupport *, const eCollisionSupport *, eContactResult *, unsigned int);
    static int GenericConvexCollide(const eShape *, const eShape *, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int GetFaceCircleContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetFaceFaceContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriCircleContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriFaceContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static void GJKCopySimplex(int, const mVec3 *, int *, mVec3 *);
    static int MultiSphereCapsule(const eMultiSphereShape &, const eCapsuleShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int MultiSphereCompound(const eMultiSphereShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int MultiSphereConvexHull(const eMultiSphereShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int MultiSphereHeightmap(const eMultiSphereShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int MultiSphereMesh(const eMultiSphereShape &, const eMeshShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int ShapeCompound(const eShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int ShapeHeightmap(const eShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int ShapeMesh(const eShape &, const eMeshShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int ShapeMultiSphere(const eShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereCapsule(const eSphereShape &, const eCapsuleShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereCompound(const eSphereShape &, const eCompoundShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereConvexHull(const eSphereShape &, const eConvexHullShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereHeightmap(const eSphereShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereMesh(const eSphereShape &, const eMeshShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereMultiSphere(const eSphereShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereSphere(const eSphereShape &, const eSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
};

class eMaterial {
public:
    void PlatformFree(void);
    void CopyMaterialData(eMaterialData *);
};

class eCameraEffectGeom {
public:
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const;
};


#endif
