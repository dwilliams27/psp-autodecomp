// eLightGridSample.cpp — decompiled from eAll_psp.obj

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

#pragma control sched=2
