#ifndef ECOLLISION_H
#define ECOLLISION_H

class eCollisionSupport;
class eContactResult;
class eMaterialData;
class eCamera;
class mFrustum;
class eCameraBins;
template <class T> class cHandleT;

class eCollision {
public:
    static int GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *);
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
