extern "C" void *memcpy(void *, const void *, unsigned int);

class cOutStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;
    int mCRC;
    unsigned char mDirty;
    char _pad11;
    unsigned char mOverflow;
    char _pad13;
    int mCRCBitPos;

    void WriteRaw(int, const void *);
};

void cOutStream::WriteRaw(int count, const void *src) {
    int bitPos = mBitPos;
    int aligned = (int)(((unsigned int)(bitPos + 7) >> 3) << 3) - bitPos;
    int dup8149 = aligned;
    int newBitPos = bitPos + dup8149;
    mBitPos = newBitPos;
    unsigned char overflow = mOverflow;
    if (overflow == 0) {
        if (mCapacity < (((int)(mBitPos + 7)) >> 3)) {
            mOverflow = 1;
        }
        overflow = mOverflow;
    }
    int canWrite = (overflow == 0);
    if (!(canWrite & 0xff)) {
        return;
    }

    int bitPos2 = mBitPos;
    unsigned char *base = mData;
    int byteIndex = bitPos2 >> 3;
    unsigned char *dst = base + byteIndex;
    bitPos2 += (count << 3);
    mBitPos = bitPos2;
    if (overflow == 0) {
        if (mCapacity < (((int)(mBitPos + 7)) >> 3)) {
            mOverflow = 1;
        }
        overflow = mOverflow;
    }
    int canWrite2 = (overflow == 0);
    if (canWrite2 & 0xff) {
        memcpy(dst, src, count);
    }
}
