#include "mPlane.h"

class eVideoPlatform {
public:
    static void SetShadowInfo(float, float, const mPlane &);
};

typedef int v4sf_t __attribute__((mode(V4SF)));

void eVideoPlatform::SetShadowInfo(float fNear, float fFar, const mPlane &plane) {
    *(float *)0x37D35C = fNear;
    *(float *)0x37D360 = fFar;

    float wv = plane.w;
    register float *dst asm("$a0") = (float *)&plane;
    __asm__ volatile(
        "lv.q C120, 0(%0)\n"
        "lui %0, 0xa\n"
        "sv.q C120, -0x7c78(%0)\n"
        "addiu %0, %0, -31864\n"
        : "+r"(dst)
        :
        : "memory");
    dst[3] = wv;
}
