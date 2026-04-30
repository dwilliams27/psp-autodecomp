// eLightGridSample.cpp — decompiled from eAll_psp.obj

typedef int v4sf_t __attribute__((mode(V4SF)));

struct mVec3 {
    union {
        v4sf_t v;
        float f[4];
    };
    mVec3() {}
    mVec3(const mVec3 &);
    mVec3(float, float, float);
};

class cWriteBlock {
public:
    int _data[2];
    void Write(int, const short *);
};

extern "C" void cWriteBlock_WriteUShortRaw(cWriteBlock *, unsigned int)
    asm("__0fLcWriteBlockFWriteUs");

struct eLightGridSample {
    short mValues[4];

    void Write(cWriteBlock &) const;
    mVec3 GetPos(void) const;
};

#pragma control sched=1

// ── eLightGridSample::Write(cWriteBlock &) const @ 0x00054b44 ──
void eLightGridSample::Write(cWriteBlock &wb) const {
    wb.Write(3, mValues);
    int blue = mValues[3] & 0x1F;
    __asm__ volatile("" : "+r"(blue));
    cWriteBlock_WriteUShortRaw(&wb, blue);
    int greenPacked = mValues[3] & 0x7E0;
    int green = (int)((unsigned int)greenPacked >> 5);
    __asm__ volatile("" : "+r"(green));
    cWriteBlock_WriteUShortRaw(&wb, green);
    int redPacked = mValues[3] & 0xF800;
    int red = (int)((unsigned int)redPacked >> 11);
    __asm__ volatile("" : "+r"(red));
    cWriteBlock_WriteUShortRaw(&wb, red);
}

// ── eLightGridSample::GetPos(void) const @ 0x00201bd0 ──
mVec3 eLightGridSample::GetPos(void) const {
    mVec3 result;
    float x = (float)mValues[0] / 7.0f;
    float y = (float)mValues[1] / 7.0f;
    float z = (float)mValues[2] / 7.0f;
    int ix;
    int iy;
    int iz;
    __asm__ volatile("mfc1 %0, %1" : "=r"(ix) : "f"(x));
    __asm__ volatile("mfc1 %0, %1" : "=r"(iy) : "f"(y));
    __asm__ volatile("mfc1 %0, %1" : "=r"(iz) : "f"(z));
    // The unreachable addu anchors the hidden return pointer in $a1; the
    // byte-matched body ends at the sv.q in the jr delay slot.
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "jr $ra\n"
        "sv.q C120, 0($a1)\n"
        "addu $zero, %3, $zero\n"
        ".set pop\n"
        :: "r"(ix), "r"(iy), "r"(iz), "r"(&result)
        : "memory");
    return result;
}

#pragma control sched=2
