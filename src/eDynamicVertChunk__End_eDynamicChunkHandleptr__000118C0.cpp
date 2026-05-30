// eDynamicVertChunk::End(eDynamicChunkHandle *)
// Address: 0x000118c0, Size: 128B
// Obj: eAll_psp.obj
// Symbol: __0fReDynamicVertChunkDEndP6TeDynamicChunkHandle

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

    void End(eDynamicChunkHandle *h);
};

struct eDynamicVertChunkPool {
    int *buf[4];    // +0..+0xC
    int idx;        // +0x10
};
extern eDynamicVertChunkPool D_000984D0;

extern int D_0037FFD124;

void eDynamicVertChunk::End(eDynamicChunkHandle *h) {
    mBuffer = 0;
    if (h != 0) {
        int index = mPtr - ((int **)&D_000984D0)[D_000984D0.idx];
        h->mId = (unsigned short)D_0037FFD124;
        h->mCount = (unsigned short)mCount;
        h->mPacked = (h->mPacked & 0xFF000000) | (index & 0xFFFFFF);
        ((unsigned char *)h)[7] = (unsigned char)(mType & 0xFFFF);
    }
}
