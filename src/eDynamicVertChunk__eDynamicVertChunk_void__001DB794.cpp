class eDynamicVertChunk {
public:
    void *mBuffer;   // +0
    int *mPtr;       // +4
    int mCount;      // +8
    int mType;       // +C

    eDynamicVertChunk(void);
};

eDynamicVertChunk::eDynamicVertChunk(void) {
    mBuffer = 0;
    mPtr = 0;
    mCount = 0;
    mType = 0;
}
