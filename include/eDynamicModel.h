#ifndef EDYNAMICMODEL_H
#define EDYNAMICMODEL_H

#include "eDynamicGeom.h"

class cBase;
class cMemPool;
class cFile;
class cTimeValue;
class cType;
class eDynamicMesh;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mSphere;
class mRay;
class mCollideHit;
class eSkin;
template <class T> class cHandleT;
struct mOCS;

class eDynamicModel : public eDynamicGeom {
public:
    eDynamicModel(cBase *);
    eDynamicModel &operator=(const eDynamicModel &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void PlatformRead(cFile &, cMemPool *);
    void OnRemovedFromWorld(void);
    bool NeedsSkinning(const eDynamicMesh *, int, int *) const;
    void *GetCurrentPhysicsController(void) const;
    void *GetCurrentPhysicsController(void);
    void GetColliderToWorld(int, mOCS *) const;
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
    void Write(cFile &) const;
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void *GetAnimationState(void) const;
    void ClearPartialAnimationController(int);
    int GetSkinIndex(void) const;
    void SetSkin(cHandleT<eSkin>, int, int, cTimeValue);
    void SetMaterialSet(int, cTimeValue);
    void SetSurfaceSet(int);
};

#endif
