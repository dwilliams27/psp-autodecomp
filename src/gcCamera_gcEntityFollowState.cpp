// ODR-WARNING: split-TU local redeclaration for gcCamera::gcEntityFollowState::Write only.
// Do not include this class definition from other translation units.

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

    void Write(int, int, bool);
    void Write(float, bool);
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
};

class cHandle {
public:
    void Write(cOutStream &) const;
};

class gcCamera {
public:
    class gcScalarMotion {
    public:
        void Write(cOutStream &) const;
    };

    class gcHPRMotion {
    public:
        void Write(cOutStream &) const;
    };

    class gcEntityFollowState {
    public:
        void Write(cOutStream &) const;
    };
};

void gcCamera::gcEntityFollowState::Write(cOutStream &s) const {
    register int one asm("$18") = 1;
    register const float *fp asm("$21") = (const float *)((const char *)this + 0x160);

    ((const cHandle *)this)->Write(s);
    s.Write(*(const int *)((const char *)this + 0x04), 0x20, true);

    {
        unsigned char v = *(const unsigned char *)((const char *)this + 0x08);
        int pos = s.mBitPos;
        int bit = pos & 7;
        unsigned char *p = s.mData + (pos >> 3);
        unsigned char ovf = s.mOverflow;
        s.mBitPos = pos + 1;
        unsigned char canWrite;
        if (ovf == 0) {
            canWrite = (unsigned char)(ovf == 0);
            if (s.mCapacity < ((s.mBitPos + 7) >> 3)) {
                s.mOverflow = one;
                goto reload1;
            }
        } else {
        reload1:
            canWrite = (unsigned char)(s.mOverflow == 0);
        }
        if (canWrite & 0xFF) {
            unsigned char cur = *p;
            int mask = one << bit;
            int bv = (v != 0) ? 1 : 0;
            *p = (unsigned char)((cur & ~mask) | (bv << bit));
        }
    }

    ((const gcCamera::gcScalarMotion *)((const char *)this + 0x0C))->Write(s);
    ((const gcCamera::gcScalarMotion *)((const char *)this + 0x24))->Write(s);
    s.Write(*(const float *)((const char *)this + 0x3C), true);
    ((const gcCamera::gcScalarMotion *)((const char *)this + 0x48))->Write(s);
    s.Write(*(const float *)((const char *)this + 0x44), true);
    ((const gcCamera::gcHPRMotion *)((const char *)this + 0x60))->Write(s);
    ((const gcCamera::gcHPRMotion *)((const char *)this + 0xA0))->Write(s);
    ((const gcCamera::gcHPRMotion *)((const char *)this + 0xE0))->Write(s);
    ((const gcCamera::gcHPRMotion *)((const char *)this + 0x120))->Write(s);

    {
        register bool t asm("$19") = true;
        int i = 0;
        do {
            s.Write(*fp, t);
            i++;
            fp++;
        } while (i < 3);
    }

    {
        cOutStreamBit *bs = (cOutStreamBit *)&s;
        unsigned char v = *(const unsigned char *)((const char *)this + 0x09);
        int pos = bs->mBitPos;
        int bit = pos & 7;
        unsigned char *p = bs->mData + (pos >> 3);
        unsigned char ovf = bs->mOverflow;
        bs->mBitPos = pos + 1;
        unsigned char canWrite;
        if (ovf == 0) {
            canWrite = (unsigned char)(ovf == 0);
            if (bs->mCapacity < ((bs->mBitPos + 7) >> 3)) {
                bs->mOverflow = one;
                goto reload2;
            }
        } else {
        reload2:
            canWrite = (unsigned char)(bs->mOverflow == 0);
        }
        if (canWrite & 0xFF) {
            unsigned char cur = *p;
            int mask = one << bit;
            int bv = (v != 0) ? 1 : 0;
            *p = (unsigned char)((cur & ~mask) | (bv << bit));
        }
    }

    {
        cOutStreamBit *bs = (cOutStreamBit *)&s;
        unsigned char v = *(const unsigned char *)((const char *)this + 0x0A);
        int pos = bs->mBitPos;
        int bit = pos & 7;
        unsigned char *p = bs->mData + (pos >> 3);
        unsigned char ovf = bs->mOverflow;
        bs->mBitPos = pos + 1;
        unsigned char canWrite;
        if (ovf == 0) {
            canWrite = (unsigned char)(ovf == 0);
            if (bs->mCapacity < ((bs->mBitPos + 7) >> 3)) {
                bs->mOverflow = one;
                goto reload3;
            }
        } else {
        reload3:
            canWrite = (unsigned char)(bs->mOverflow == 0);
        }
        if (canWrite & 0xFF) {
            unsigned char cur = *p;
            int mask = one << bit;
            int bv = (v != 0) ? 1 : 0;
            *p = (unsigned char)((cur & ~mask) | (bv << bit));
        }
    }

    {
        cOutStreamBit *bs = (cOutStreamBit *)&s;
        unsigned char v = *(const unsigned char *)((const char *)this + 0x08);
        int pos = bs->mBitPos;
        int bit = pos & 7;
        unsigned char *p = bs->mData + (pos >> 3);
        unsigned char ovf = bs->mOverflow;
        bs->mBitPos = pos + 1;
        unsigned char canWrite;
        if (ovf == 0) {
            canWrite = (unsigned char)(ovf == 0);
            if (bs->mCapacity < ((bs->mBitPos + 7) >> 3)) {
                bs->mOverflow = one;
                goto reload4;
            }
        } else {
        reload4:
            canWrite = (unsigned char)(bs->mOverflow == 0);
        }
        if (canWrite & 0xFF) {
            unsigned char cur = *p;
            int mask = one << bit;
            int bv = (v != 0) ? 1 : 0;
            *p = (unsigned char)((cur & ~mask) | (bv << bit));
        }
    }
}
