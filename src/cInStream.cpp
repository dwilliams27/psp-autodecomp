class cInStream {
public:
    unsigned char *mData;
    int mField4;
    int mBitPos;

    void Read(int &, int, bool);
    void Read(unsigned int &, int, bool);
    void ReadBits(void *, int);
};

void cInStream::Read(int &value, int bits, bool packed) {
    int signBit = 1 << (bits - 1);
    value = 0;

    if (packed && bits >= 3) {
        unsigned char *data = mData;
        int bytePos = mBitPos;
        unsigned int byteOffset = bytePos >> 3;
        unsigned char byte = data[byteOffset];
        int *posPtr = &mBitPos;
        int pos = *posPtr;
        int mask = 1 << (pos & 7);
        *posPtr = pos + 1;
        bool small = (byte & mask) != 0;
        if (small) {
            bytePos = mBitPos;
            byte = mData[bytePos >> 3];
            pos = *posPtr;
            mask = 1 << (pos & 7);
            *posPtr = pos + 1;
            value = (byte & mask) != 0;
            return;
        }
    }

    ReadBits(&value, bits);
    if (value & signBit) {
        value |= ~(signBit - 1);
    }
}

void cInStream::Read(unsigned int &value, int bits, bool packed) {
    value = 0;

    if (packed && bits >= 3) {
        int bytePos = mBitPos;
        unsigned char *data = mData;
        int byteOffset = bytePos >> 3;
        int *posPtr = &mBitPos;
        unsigned char *bytePtr = data;
        bytePtr += byteOffset;
        unsigned char byte = *bytePtr;
        int pos = *posPtr;
        int mask = 1 << (pos & 7);
        *posPtr = pos + 1;
        __asm__ volatile("" ::: "memory");
        bool small = (byte & mask) != 0;
        if (!small) {
            goto readBits;
        }
        bytePos = mBitPos;
        byte = mData[bytePos >> 3];
        pos = *posPtr;
        mask = 1 << (pos & 7);
        *posPtr = pos + 1;
        value = (byte & mask) != 0;
        return;
    }

readBits:
    ReadBits(&value, bits);
}
