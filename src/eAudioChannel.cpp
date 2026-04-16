#include "eAudioChannel.h"
#include "mOCS.h"

extern int *gSpeakerConfigTable[];

void eAudioChannel::CalcPanning(int numSpeakers, const mOCS *speakers, float *output) const {
    float localPos[4] __attribute__((aligned(16)));

    int config = mSpeakerConfig;
    int index = config & 0xFFFF;
    int *entry;

    if (config != 0) {
        int *tableEntry = gSpeakerConfigTable[index];
        int *result = 0;
        if (tableEntry != 0) {
            if (*(int *)((char *)tableEntry + 0x30) == config) {
                result = tableEntry;
            }
        }
        entry = result;
    } else {
        entry = 0;
    }

    bool is3D = (bool)((mFlags & 0x80) != 0);
    if (!is3D) goto default_panning;
    if (entry == 0) goto default_panning;
    if (mListenerPtr == 0) goto default_panning;

    {
        float zero = 0.0f;
        output[3] = zero;
        output[2] = zero;
        output[1] = zero;
        output[0] = zero;

        for (int i = 0; i < numSpeakers; i++, speakers++) {
            float one = 1.0f;
            int listener = mListenerPtr;

            __asm__ volatile(
                "lv.q C120, 0x10(%0)\n"
                "lv.q C000, 0x0(%1)\n"
                "lv.q C010, 0x10(%1)\n"
                "lv.q C020, 0x20(%1)\n"
                "lv.q C030, 0x30(%1)\n"
                ".word 0x60838606\n"
                ".word 0xf1068007\n"
                ".word 0xfba70000\n"
                : : "r"((void *)listener), "r"((const void *)speakers)
                : "memory"
            );

            float maxRange = *(float *)((char *)entry + 0x54);
            float minRange = *(float *)((char *)entry + 0x58);

            float distance;
            __asm__ volatile(
                ".word 0x64878704\n"
                ".word 0xd0160404\n"
                ".word 0x486a0004\n"
                "mtc1 $t2, %0\n"
                : "=f"(distance)
                : : "$t2"
            );

            if (distance <= maxRange) {
                float atten = (maxRange - distance) / (maxRange - minRange);
                float clamped;
                if (!(atten <= zero)) {
                    clamped = atten;
                    if (!(clamped < one)) {
                        clamped = one;
                    }
                } else {
                    clamped = zero;
                }

                float twoMax = maxRange + maxRange;
                float localY = localPos[1];
                twoMax = twoMax * twoMax;
                float inverseSq = one / twoMax;
                float yPlus = localY + maxRange;
                float localX = localPos[0];
                float scaledAtten = clamped * inverseSq;
                float leftDist = maxRange - localX;

                float frontFactor = scaledAtten * yPlus;
                float rightDist = localX + maxRange;
                float yMinus = maxRange - localY;
                float rearFactor = scaledAtten * yMinus;

                float fl = frontFactor * leftDist;
                if (!(output[0] <= fl)) fl = output[0];
                float fr = frontFactor * rightDist;
                output[0] = fl;

                float prevOut1 = output[1];
                if (!(prevOut1 <= fr)) fr = prevOut1;
                float rl = rearFactor * leftDist;
                float prevOut2 = output[2];
                output[1] = fr;

                if (!(prevOut2 <= rl)) rl = prevOut2;
                float rr = rearFactor * rightDist;
                output[2] = rl;

                float prevOut3 = output[3];
                if (!(prevOut3 <= rr)) rr = prevOut3;
                output[3] = rr;
            }
        }
    }
    return;

default_panning:
    output[3] = 0.25f;
    output[2] = 0.25f;
    output[1] = 0.25f;
    output[0] = 0.25f;
}

// ============================================================
// gcCamera::gcEntityFollowState::gcEntityFollowState(void)
// ============================================================

class gcCamera {
public:
    struct gcEntityFollowState {
        int f00;                // 0x00
        int f04;                // 0x04
        unsigned char f08;      // 0x08
        unsigned char f09;      // 0x09
        unsigned char f0A;      // 0x0A
        char _p0B;
        float f0C;              // 0x0C
        float f10;              // 0x10
        float f14;              // 0x14
        float f18;              // 0x18
        float f1C;              // 0x1C
        float f20;              // 0x20
        float f24;              // 0x24
        float f28;              // 0x28
        float f2C;              // 0x2C
        float f30;              // 0x30
        float f34;              // 0x34
        float f38;              // 0x38
        float f3C;              // 0x3C
        float f40;              // 0x40
        float f44;              // 0x44
        float f48;              // 0x48
        float f4C;              // 0x4C
        float f50;              // 0x50
        float f54;              // 0x54
        float f58;              // 0x58
        float f5C;              // 0x5C
        char _q60[48];          // 0x60-0x8F
        float f90, f94, f98;    // 0x90
        char _p9C[4];
        char _qA0[48];          // 0xA0-0xCF
        float fD0, fD4, fD8;    // 0xD0
        char _pDC[4];
        char _qE0[48];          // 0xE0-0x10F
        float f110, f114, f118; // 0x110
        char _p11C[4];
        char _q120[48];         // 0x120-0x14F
        float f150, f154, f158; // 0x150
        char _p15C[4];
        char _q160[16];         // 0x160-0x16F

        gcEntityFollowState();
    };
};

#define VFPU_ZQ(off) \
    __asm__ volatile( \
        "mfc1 $a1, %0\n" "mfc1 $a2, %0\n" "mfc1 $a3, %0\n" \
        ".word 0x48e50006\n" ".word 0x48e60026\n" ".word 0x48e70046\n" \
        "sv.q C120, " #off "(%1)\n" \
        : : "f"(z), "r"((void *)this) : "$a1", "$a2", "$a3", "memory" \
    )

gcCamera::gcEntityFollowState::gcEntityFollowState() {
    float z = 0.0f;
    float one = 1.0f;
    float big = 100000.0f;
    float pt2 = 0.2f;

    f00 = 0;
    f04 = -1;
    f08 = 0;
    f09 = 0;
    f0A = 0;
    f0C = z;
    f10 = z;
    f18 = one;
    f14 = z;
    f1C = one;
    f24 = z;
    f20 = big;
    f28 = z;
    f30 = one;
    f2C = z;
    f38 = big;
    f34 = one;
    f3C = pt2;
    f40 = z;
    f48 = z;
    f44 = z;
    f4C = z;
    f50 = z;
    f54 = one;
    f58 = one;
    f5C = big;

    VFPU_ZQ(0x60);
    VFPU_ZQ(0x70);
    VFPU_ZQ(0x80);

    f94 = one;
    f90 = one;
    f98 = big;

    VFPU_ZQ(0xA0);
    VFPU_ZQ(0xB0);
    VFPU_ZQ(0xC0);

    fD4 = one;
    fD0 = one;
    fD8 = big;

    VFPU_ZQ(0xE0);
    VFPU_ZQ(0xF0);
    VFPU_ZQ(0x100);

    f114 = one;
    f110 = one;
    f118 = big;

    VFPU_ZQ(0x120);
    VFPU_ZQ(0x130);
    VFPU_ZQ(0x140);

    f154 = one;
    f150 = one;
    f158 = big;

    VFPU_ZQ(0x160);
}
