// eAudioFader::Write(cOutStream &) const  @ 0x0001cf38 (eAll_psp.obj, 216B)
// Split-TU isolated definition. Sibling: eAudioFader::Read (0x0001d010).

class cOutStream {
public:
    unsigned char *mData;   // 0x00
    int mCapacity;          // 0x04
    int mBitPos;            // 0x08
    int mCRC;               // 0x0C
    unsigned char mDirty;   // 0x10
    char _pad11;            // 0x11
    unsigned char mOverflow;// 0x12

    void Write(float, bool);
};

class cTimeValue {
public:
    void Write(cOutStream &) const;
};

class eAudioFader {
public:
    unsigned char mState;   // 0x00
    char _pad01[3];         // 0x01
    float mField4;          // 0x04
    float mField8;          // 0x08
    // cTimeValue at 0x0C

    void Write(cOutStream &) const;
};

void eAudioFader::Write(cOutStream &s) const {
    int value = mState & 0xFF;
    int bitPos = s.mBitPos;
    int bit = bitPos & 7;
    unsigned char *ptr = s.mData + (bitPos >> 3);
    s.mBitPos = bitPos + 1;

    if (s.mOverflow == 0 && s.mCapacity < ((s.mBitPos + 7) >> 3)) {
        s.mOverflow = 1;
    }
    if ((s.mOverflow == 0) & 0xFF) {
        *ptr = (*ptr & ~(1 << bit)) | ((value != 0) << bit);
    }
    s.Write(mField4, true);
    s.Write(mField8, true);
    ((cTimeValue *)((char *)this + 0xC))->Write(s);
}
