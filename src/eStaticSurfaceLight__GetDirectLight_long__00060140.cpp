// ODR-WARNING: split translation unit for a method missing from a shared header.
// Local class declarations only; do not add these to include/*.h.
//
// eStaticSurfaceLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &p2,
//                                     const mRay &ray, const mVec3 &p4) const
//
// Direct-light contribution for a surface light. Two VFPU dot products gate the
// computation; both are vdot.t (no scalar-C equivalent), moved out via mfv/mtc1
// (the __builtin_vstof idiom). Either dot failing the sign test yields a zero
// vector built with the SNC mtc1/mfc1/mtv/sv.q construction pattern. Otherwise a
// scalar attenuation modulates the light colour (3 bytes / 255) into the output.

typedef int v4sf_t __attribute__((mode(V4SF)));
typedef int vs_t __attribute__((mode(VS)));

struct mVec3;
struct mRay;

class eStaticSurfaceLight {
public:
    void GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &,
                        const mVec3 &) const;
};

#pragma control sched=1
void eStaticSurfaceLight::GetDirectLight(mVec3 *out, const mVec3 &,
                                         const mVec3 &p2, const mRay &ray,
                                         const mVec3 &p4) const {
    vs_t s1;
    __asm__(
        "lv.q   C120, 0x10(%1)\n"
        "lv.q   C130, 0(%2)\n"
        "vdot.t %0, C120, C130\n"
        : "=x"(s1)
        : "r"(&ray), "r"(&p2));
    float d1 = __builtin_vstof(s1);

    vs_t s2;
    __asm__(
        "lv.q   C130, 0(%1)\n"
        "vdot.t %0, C120, C130\n"
        : "=x"(s2)
        : "r"(&p4));
    float d2 = __builtin_vstof(s2);

    float nd2 = -d2;
    float scalar;
    float zero = 0.0f;
    if (d1 <= zero || nd2 <= zero) {
        int z0, z1, z2;
        __asm__ volatile("mfc1 %0, %1" : "=r"(z0) : "f"(zero));
        __asm__ volatile("mfc1 %0, %1" : "=r"(z1) : "f"(zero));
        __asm__ volatile("mfc1 %0, %1" : "=r"(z2) : "f"(zero));
        __asm__ volatile(
            "mtv  %0, S120\n"
            "mtv  %1, S121\n"
            "mtv  %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :
            : "r"(z0), "r"(z1), "r"(z2), "r"(out)
            : "memory");
        return;
    }
    scalar = d1 * nd2;
    {
        float t = *(const float *)((const char *)&ray + 0x20);
        float denom = (t * 3.1415927f) * t;
        __asm__ volatile("" ::: "memory");
        float dup28312 = denom;
        scalar = scalar / dup28312;
        scalar = scalar * 100.0f;

        float inv255 = 0.0039215689f;
        float frame[8];
        { unsigned char c = *(const unsigned char *)((const char *)this + 0x4a);
          frame[4] = (float)(int)c * inv255; }
        { unsigned char c = *(const unsigned char *)((const char *)this + 0x49);
          frame[5] = (float)(int)c * inv255; }
        { unsigned char c = *(const unsigned char *)((const char *)this + 0x48);
          frame[6] = (float)c * inv255; }

        float intensity = *(const float *)((const char *)this + 0x44);
        scalar = intensity * scalar;

        vs_t sscale = __builtin_ftovs(scalar);
        __asm__(
            "lv.q   C120, 0x10(%1)\n"
            "vscl.t C120, C120, %0\n"
            "sv.q   C120, 0(%2)\n"
            :
            : "x"(sscale), "r"(frame), "r"(out)
            : "memory");
    }
}
#pragma control sched=2
