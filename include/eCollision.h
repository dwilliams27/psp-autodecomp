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
class eCollisionContactInfo;
class eHeightmapShape;
class eMultiSphereShape;
class eShape;
class eSphereShape;
class mOCS;
struct mVec3;

class eCollision {
public:
    static int BoxHeightmap(const eBoxShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int Clip(const eCollisionSupport *, const eCollisionSupport *, eContactResult *, unsigned int);
    static int ClipCircle(const eCollisionSupport *, const eCollisionSupport *, eContactResult *, unsigned int);
    static int GenericConvexCollide(const eShape *, const eShape *, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int GetFaceCircleContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetFaceFaceContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriCircleContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriFaceContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static int GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
    static void GJKCopySimplex(int, const mVec3 *, int *, mVec3 *);
    static int ShapeMultiSphere(const eShape &, const eMultiSphereShape &, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int SphereHeightmap(const eSphereShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
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
