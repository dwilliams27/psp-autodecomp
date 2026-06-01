extern "C" void *memcpy(void *, const void *, unsigned int);

class cInStream {
public:
    unsigned char *mData;
    int mField4;
    int mBitPos;

    void ReadRaw(int, void *);
};

void cInStream::ReadRaw(int count, void *dst) {
    unsigned char *src = mData;
    int aligned = (int)((unsigned int)(mBitPos + 7) >> 3) << 3;
    unsigned char *srcPtr = src + (aligned >> 3);
    mBitPos = aligned + (count << 3);
    memcpy(dst, srcPtr, count);
}
