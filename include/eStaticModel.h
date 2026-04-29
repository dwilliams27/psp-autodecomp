#ifndef ESTATICMODEL_H
#define ESTATICMODEL_H

#include "eDynamicGeom.h"  // provides eGeom

class cBase;
class cFile;
class cMemPool;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;
struct eStaticModelNodeCullIdLOD;

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eStaticModel : public eGeom {
public:
    eStaticModel(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void PlatformReset(cMemPool *, bool);
    void OnRemovedFromWorld(void);
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;

    int mNodeCullIdCount;
    cArrayBase<eStaticModelNodeCullIdLOD> mNodeCullIdLODs;
    int mField98;
};

#endif
