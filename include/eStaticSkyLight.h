#ifndef ESTATICSKYLIGHT_H
#define ESTATICSKYLIGHT_H

class mVec3;
class mRay;

class eStaticSkyLight {
public:
    int GetNumStratifiedSamples(void) const;
    void GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const;

    char _pad0[0x44];
    float intensity;          // 0x44
    unsigned char colorB;     // 0x48
    unsigned char colorG;     // 0x49
    unsigned char colorR;     // 0x4A
};

#endif
