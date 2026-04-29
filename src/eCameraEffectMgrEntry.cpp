// eCameraEffectMgrEntry methods (eAll_psp.obj)
//   0x0002d47c  Write(cOutStream &) const  (244B)
//   0x0002d570  Read(cInStream &)          (176B)

#include "cOutStream.h"

class cInStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;

    void Read(float &, int, int, bool);
};

class cTimeValue {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class cHandle {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class cOutStreamBit {
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

    __attribute__((always_inline)) void WriteBit(unsigned char value) {
        int pos = mBitPos;
        int bit = pos & 7;
        unsigned char *ptr = mData + (pos >> 3);
        int nextPos = pos + 1;
        unsigned char overflow = mOverflow;
        unsigned char bitValue = value;
        mBitPos = nextPos;
        if (overflow == 0) {
            if (mCapacity < ((mBitPos + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        unsigned char canWrite = (mOverflow == 0);
        if ((unsigned char)(canWrite & 0xFF)) {
            unsigned char byte = *ptr;
            *ptr = (unsigned char)((byte & ~(1 << bit)) | (((bitValue != 0) ? 1 : 0) << bit));
        }
    }
};

class eCameraEffectMgrEntry {
public:
    unsigned char mActive;  // 0x00
    char _pad01[3];         // 0x01
    float mCurrent;         // 0x04
    float mTarget;          // 0x08
    int mDuration;          // 0x0C
    int mHandle;            // 0x10

    void Write(cOutStream &) const;
    void Read(cInStream &);
};

void eCameraEffectMgrEntry::Write(cOutStream &s) const {
    ((cOutStreamBit *)&s)->WriteBit(mActive);
    s.Write(mCurrent, 2, 0xA, true);
    s.Write(mTarget, 2, 0xA, true);
    ((const cTimeValue *)((const char *)this + 0xC))->Write(s);
    ((const cHandle *)((const char *)this + 0x10))->Write(s);
}

void eCameraEffectMgrEntry::Read(cInStream &s) {
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
        mActive = (byte & mask) != 0;
    }
    s.Read(mCurrent, 2, 0xA, true);
    s.Read(mTarget, 2, 0xA, true);
    ((cTimeValue *)((char *)this + 0xC))->Read(s);
    ((cHandle *)((char *)this + 0x10))->Read(s);
}
