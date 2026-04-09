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

class eCollision {
public:
    static int BoxHeightmap(const eBoxShape &, const eHeightmapShape &, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int GenericConvexCollide(const eShape *, const eShape *, const mOCS &, const mOCS &, eCollisionContactInfo *);
    static int GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
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
