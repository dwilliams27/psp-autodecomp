#include "mVec3.h"

class eDynamicChunkHandle {
public:
    unsigned short mId;      // +0
    unsigned short mCount;   // +2
    unsigned int mPacked;    // +4 (low 24 bits = buffer offset index, byte 7 = type)
};

class eDynamicVertChunk {
public:
    void *mBuffer;   // +0
    int *mPtr;       // +4
    int mCount;      // +8
    int mType;       // +C

    eDynamicVertChunk(const eDynamicChunkHandle &h);
    void BeginQuads(int count, float f, const mVec3 &v);
    void BeginStrip(int count, float f, const mVec3 &v);
    void BeginLineList(int count, float f, const mVec3 &v);
    void BeginPositionTriList(int count, float f, const mVec3 &v);
    void Begin(void);
};

struct eDynamicVertChunkPool {
    int *buf[4];    // +0..+0xC
    int idx;        // +0x10
};
extern eDynamicVertChunkPool D_000984D0;

eDynamicVertChunk::eDynamicVertChunk(const eDynamicChunkHandle &h) {
    mBuffer = 0;
    mPtr = ((int **)&D_000984D0)[D_000984D0.idx] + (h.mPacked & 0xFFFFFF);
    mCount = h.mCount;
    mType = ((const unsigned char *)&h)[7];
}

void eDynamicVertChunk::BeginQuads(int count, float f, const mVec3 &v) {
    mCount = count * 4;
    mType = 0;
    Begin();
}

void eDynamicVertChunk::BeginStrip(int count, float f, const mVec3 &v) {
    mCount = count;
    mType = 1;
    Begin();
}

void eDynamicVertChunk::BeginLineList(int count, float f, const mVec3 &v) {
    mCount = count * 2;
    mType = 4;
    Begin();
}

void eDynamicVertChunk::BeginPositionTriList(int count, float f, const mVec3 &v) {
    mCount = count * 3;
    mType = 5;
    Begin();
}
