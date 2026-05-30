// eAudioFader::Read(cInStream &)  @ 0x0001d010 (eAll_psp.obj, 148B)
// Split-TU isolated definition. Sibling: eCameraEffectMgrEntry::Read.

class cInStream {
public:
    unsigned char *mData;   // 0x00
    int mField4;            // 0x04
    int mBitPos;            // 0x08

    void Read(float &, bool);
};

class cTimeValue {
public:
    void Read(cInStream &);
};

class eAudioFader {
public:
    unsigned char mState;   // 0x00
    char _pad01[3];         // 0x01
    float mField4;          // 0x04
    float mField8;          // 0x08
    // cTimeValue at 0x0C

    void Read(cInStream &);
};

void eAudioFader::Read(cInStream &s) {
    int bitPos;
    cInStream *stream = &s;
    bitPos = stream->mBitPos;
    unsigned char *data = stream->mData;
    int *bitPosPtr = &stream->mBitPos;
    unsigned char byte = data[bitPos >> 3];

    bitPos = *bitPosPtr;
    {
        int mask = 1 << (bitPos & 7);
        bitPos++;
        *bitPosPtr = bitPos;
        mState = (byte & mask) != 0;
    }
    s.Read(mField4, true);
    s.Read(mField8, true);
    ((cTimeValue *)((char *)this + 0xC))->Read(s);
}
