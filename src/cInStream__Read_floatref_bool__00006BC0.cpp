class cInStream {
public:
    unsigned char *mData;
    int mField4;
    int mBitPos;

    void Read(float &, bool);
    void ReadBits(void *, int);
};

void cInStream::Read(float &value, bool packed) {
    if (packed) {
        int bytePos = mBitPos;
        int *posPtr = &mBitPos;
        int byteOffset = bytePos >> 3;
        unsigned char byte = *((mData) + byteOffset);
        int pos = *posPtr;
        int mask = 1 << (pos & 7);
        *posPtr = pos + 1;
        __asm__ volatile("" ::: "memory");
        bool small = (byte & mask) != 0;
        if (small) {
            bytePos = mBitPos;
            byte = mData[bytePos >> 3];
            pos = *posPtr;
            *posPtr = pos + 1;
            mask = 1 << (pos & 7);
            int bit = (byte & mask) != 0;
            value = (float)bit;
            return;
        }
    }

    ReadBits(&value, 0x20);
}
