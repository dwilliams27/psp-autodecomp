#include "cOutStream.h"

class cInStream {
public:
    void Read(unsigned int &, int, bool);
    void Read(float &, bool);
};

class cHandle {
public:
    unsigned int mKey;

    void Read(cInStream &);
    void Write(cOutStream &) const;
};

struct gcEventStackHandle {
    cHandle mHandle;
    unsigned int mBits;
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

class gcEventStackData {
public:
    unsigned int mWord0;
    float mFloat4;
    float mFloats8[4];
    float mFloats18[9];
    unsigned int mWord3C;
    unsigned int mWords40[6];
    gcEventStackHandle mHandles[2];

    void Write(cOutStream &) const;
    void Read(cInStream &);
};

void gcEventStackData::Write(cOutStream &stream) const {
    int i = 0;
    const float *value = (const float *)((const char *)this + 8);
    const gcEventStackHandle *handle = (const gcEventStackHandle *)((const char *)this + 0x58);
    do {
        stream.Write(*value, true);
        i++;
        value++;
    } while (i < 4);

    int j = 0;
    do {
        handle->mHandle.Write(stream);
        stream.Write(handle->mBits, 0x20, true);
        j++;
        handle++;
    } while (j < 2);
}

void gcEventStackData::Read(cInStream &stream) {
    gcEventStackData temp;

    temp.mWord0 = 0;
    temp.mFloat4 = 0.0f;
    temp.mWord3C = 0;
    __vec_new(&temp.mHandles[0], 2, 8, (void (*)(void *))0x2275F0);
    temp.mFloats8[0] = 0.0f;
    temp.mFloats8[1] = 0.0f;
    temp.mFloats8[2] = 0.0f;
    temp.mFloats8[3] = 0.0f;
    temp.mFloats18[0] = 0.0f;
    temp.mFloats18[1] = 0.0f;
    temp.mFloats18[2] = 0.0f;
    temp.mFloats18[3] = 0.0f;
    temp.mFloats18[4] = 0.0f;
    temp.mWords40[0] = 0;
    temp.mWords40[1] = 0;
    temp.mWords40[2] = 0;
    temp.mWords40[3] = 0;
    temp.mWords40[4] = 0;
    temp.mWords40[5] = 0;

    mWord0 = temp.mWord0;
    mFloat4 = temp.mFloat4;

    for (int i = 0; i < 4; i++) {
        mFloats8[i] = temp.mFloats8[i];
    }

    for (int i = 0; i < 9; i++) {
        mFloats18[i] = temp.mFloats18[i];
    }

    mWord3C = temp.mWord3C;

    for (int i = 0; i < 6; i++) {
        mWords40[i] = temp.mWords40[i];
    }

    {
        unsigned int i = 0;
        unsigned int *dst = (unsigned int *)((char *)this + 0x58);
        unsigned int *src = (unsigned int *)((char *)&temp + 0x58);
        do {
            unsigned int value0 = src[0];
            unsigned int *src1 = src + 1;
            dst[0] = value0;
            unsigned int value1 = src1[0];
            __asm__ volatile("" ::: "memory");
            unsigned int *dst1 = (unsigned int *)((char *)dst + 4);
            i++;
            dst1[0] = value1;
            dst += 2;
            src += 2;
        } while (i < 2U);
    }

    int i = 0;
    float *dstFloat = mFloats8;
    do {
        stream.Read(*dstFloat, true);
        i++;
        dstFloat++;
    } while (i < 4);

    i = 0;
    gcEventStackHandle *readHandle = mHandles;
    do {
        readHandle->mHandle.Read(stream);
        stream.Read(readHandle->mBits, 0x20, true);
        i++;
        readHandle++;
    } while (i < 2);
}
