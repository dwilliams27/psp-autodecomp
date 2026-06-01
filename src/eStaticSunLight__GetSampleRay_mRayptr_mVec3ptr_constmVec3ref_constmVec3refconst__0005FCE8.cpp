// src/eStaticSunLight__GetSampleRay_mRayptr_mVec3ptr_constmVec3ref_constmVec3refconst__0005FCE8.cpp
// 0x0005fce8  eStaticSunLight::GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const
//   eAll_psp.obj  size 220B
// Split-TU: class declared locally (GetSampleRay absent from src/eStaticSunLight.cpp).

class cBase;
class mVec3;
class mRay;

typedef int v4sf_t __attribute__((mode(V4SF)));

float sinf(float);
float cosf(float);

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
};

class eStaticSunLight : public eStaticLight {
public:
    void GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const;
};

#pragma control sched=2
void eStaticSunLight::GetSampleRay(mRay *ray, mVec3 *, const mVec3 &p1, const mVec3 &) const {
    float raw0 = *(const float *)((const char *)this + 0x90);
    float raw1 = *(const float *)((const char *)this + 0x94);
    float a0 = raw0 * 0.017453292f;
    float s0 = sinf(a0);
    float c0 = cosf(a0);
    float a1 = raw1 * 0.017453292f;
    float s1 = sinf(a1);
    float c1 = cosf(a1);

    *(float *)((char *)ray + 0x10) = -(c1 * s0);
    *(float *)((char *)ray + 0x14) = c0 * c1;
    *(float *)((char *)ray + 0x18) = s1;

    __asm__ volatile(
        "lv.q C120, 0x10(%0)\n"
        "vneg.t C120, C120\n"
        "sv.q C120, 0x10(%0)\n"
        :: "r"(ray) : "memory"
    );

    *(float *)((char *)ray + 0x20) = -1.0f;

    *(v4sf_t *)ray = *(const v4sf_t *)&p1;
}
