#ifndef ESTATICMODEL_H
#define ESTATICMODEL_H

#include "eDynamicGeom.h"  // provides eGeom

class cBase;
class cMemPool;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class eStaticModel : public eGeom {
public:
    eStaticModel(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void OnRemovedFromWorld(void);
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
};

#endif
