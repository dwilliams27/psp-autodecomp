#ifndef EHEIGHTMAP_H
#define EHEIGHTMAP_H

#include "eDynamicGeom.h"

class cBase;
class cMemPool;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class cFile;

class eHeightmap : public eGeom {
public:
    eHeightmap(cBase *);
    static eHeightmap *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void PlatformFree(void);
    int GetSurface(int) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const;
    void __attribute__((naked)) CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
};

#endif
