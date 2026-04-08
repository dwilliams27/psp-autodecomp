#ifndef EDYNAMICFLUID_H
#define EDYNAMICFLUID_H

class eCamera;
class mFrustum;
class eCameraBins;
template <class T> class cHandleT;
class eMaterial;
class eDrawInfo;

class eDynamicFluid {
public:
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const;
    void Draw(const eDrawInfo &) const;
};

#endif
