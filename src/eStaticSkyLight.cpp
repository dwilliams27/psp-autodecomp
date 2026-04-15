#include "eStaticSkyLight.h"
#include "mVec3.h"
#include "mRay.h"
#include "eBipedController.h"

int eStaticSkyLight::GetNumStratifiedSamples(void) const {
    return 8;
}

void eStaticSkyLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &normal, const mRay &ray, const mVec3 &) const {
    volatile float color[8];

    float zero, dot;
    __asm__ volatile(
        "lv.q C120, 0x10(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %1\n"
        "mtc1 $zero, %0\n"
        : "=f"(zero), "=f"(dot)
        : "r"(&ray), "r"(&normal)
        : "$a2"
    );

    if (dot > zero) {
        float scale = 1.0f / 255.0f;
        color[0] = (float)colorR * scale;
        color[1] = (float)colorG * scale;
        color[2] = (float)colorB * scale;
        float factor = dot * intensity;

        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mtv $a0, S100\n"
            "lv.q C120, 0($sp)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(factor), "r"(out)
            : "$a0", "memory"
        );
        return;
    }

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a2, %0\n"
        "mfc1 $a3, %0\n"
        "mtv $a0, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(zero), "r"(out)
        : "$a0", "$a2", "$a3", "memory"
    );
}

void eBipedController::GetVelocity(int, mVec3 *vel, mVec3 *accel) const {
    int *entry = (int *)((char *)bodyEntries + bodyIndex * 8);

    float fzero = 0.0f;
    unsigned char found = 0;
    if (entry[0] != 0) {
        if (entry[1] != 0) {
            found = 1;
        }
        found = (unsigned char)(found & 0xff);
    }

    if (found) goto found_path;

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a1, %0\n"
        "mfc1 $t0, %0\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $t0, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(fzero), "r"(vel)
        : "$a0", "$a1", "$t0", "memory"
    );
    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a1, %0\n"
        "mfc1 $a2, %0\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(fzero), "r"(accel)
        : "$a0", "$a1", "$a2", "memory"
    );
    return;

found_path:
    {
        void *body = (void *)entry[0];
        __asm__ volatile(
            "lv.q C120, 0x40(%0)\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(body), "r"(vel)
            : "memory"
        );
        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mfc1 $a1, %0\n"
            "mfc1 $a2, %0\n"
            "mtv $a0, S120\n"
            "mtv $a1, S121\n"
            "mtv $a2, S122\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(fzero), "r"(accel)
            : "$a0", "$a1", "$a2", "memory"
        );
    }
}
