#ifndef EDYNAMICFLUID_H
#define EDYNAMICFLUID_H

#include "eDynamicGeom.h"

class cFile;
class cMemPool;
class eCamera;
class mFrustum;
class eCameraBins;
template <class T> class cHandleT;
class eMaterial;
class eDrawInfo;

class cTimeValue {
public:
    int mTime;
};

class eDynamicFluid : public eDynamicGeom {
public:
    eDynamicFluid(cBase *);
    ~eDynamicFluid();

    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const;
    void Draw(const eDrawInfo &) const;
    void Write(cFile &) const;
    void Update(cTimeValue);

    static cBase *New(cMemPool *, cBase *);
};

#endif
