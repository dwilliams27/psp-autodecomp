#ifndef ESTATICSKYLIGHT_H
#define ESTATICSKYLIGHT_H

#include "eStaticLight.h"

class cBase;
class cMemPool;
class cFile;
class mVec3;
class mRay;

class eStaticSkyLight : public eStaticLight {
public:
    eStaticSkyLight(cBase *);
    ~eStaticSkyLight();

    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    int GetNumStratifiedSamples(void) const;
    void GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const;

    char _pad0[0x44];
    float intensity;          // 0x44
    unsigned char colorB;     // 0x48
    unsigned char colorG;     // 0x49
    unsigned char colorR;     // 0x4A
};

#endif
