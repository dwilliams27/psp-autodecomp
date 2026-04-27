// gcMsgAttachEntity — gcAll_psp.obj
//   0x001356f4 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const   (228B)
//   0x00285680 GetType(void) const                                                                (76B)
//   0x002856cc New(nwMsgBuffer &) static                                                          (116B)

class nwMsg;
class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

inline void *operator new(unsigned int, void *p) { return p; }

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cHandle {
public:
    int mIndex;
    cHandle();
    cHandle(const cHandle &);
    void Write(cOutStream &) const;
};

inline cHandle::cHandle() { mIndex = 0; }
inline cHandle::cHandle(const cHandle &o) { mIndex = o.mIndex; }

class cObjectKeyRef {
public:
    int     mField0;
    int     mField4;
    cHandle mHandle;     // 0x08
    cObjectKeyRef();
};

inline cObjectKeyRef::cObjectKeyRef() {
    mField0       = 0;
    mHandle.mIndex = mField0;
    mField4       = mField0;
}

class cOutStreamRef {
public:
    void Write(int v, int bits, bool sign);   // OOL
};

// Stream layout: 0x00 mBuf, 0x04 mCapacity, 0x08 mBitOffset, 0x12 mOverflow.
// Write(bool) is an inline single-bit writer.
class cOutStreamBit {
public:
    char *mBuf;          // 0x00
    int   mCapacity;     // 0x04
    int   mBitOffset;    // 0x08
    char  _pad0C[6];     // 0x0C..0x11
    unsigned char mOverflow;  // 0x12

    void WriteBit(unsigned char v) {
        int pos     = mBitOffset;
        char *buf   = mBuf;
        int bit     = pos & 7;
        int byteIdx = pos >> 3;
        int newPos  = pos + 1;
        unsigned char *p = (unsigned char *)(buf + byteIdx);
        unsigned char ovf = mOverflow;
        unsigned char vb  = v;
        mBitOffset = newPos;
        if (ovf == 0) {
            if (mCapacity < ((mBitOffset + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        unsigned char ck = (mOverflow == 0);
        if ((unsigned char)(ck & 0xFF)) {
            unsigned char cur = *p;
            int mask = 1 << bit;
            int bv   = (vb != 0) ? 1 : 0;
            *p = (unsigned char)((cur & ~mask) | (bv << bit));
        }
    }
};

// gcMsgAttachEntity — has TWO base subobjects with their own vtable, then derived
// adds cHandle2, short, bool. Layout (0x14 bytes, allocated via nwMsgBuffer):
//   0x00 vtable                                (base: 0x38ABC0, derived: 0x38AC20)
//   0x04 cHandle (mHandle1)
//   0x08 byte    (base flag)
//   0x0C cHandle (mHandle2)
//   0x10 short   (mShort)
//   0x12 bool    (mFlag)

class gcMsgAttachEntity {
public:
    void *        mVTable;     // 0x00
    cHandle       mHandle1;    // 0x04
    unsigned char mFlag1;      // 0x08
    char          _pad09[3];
    cHandle       mHandle2;    // 0x0C
    short         mShort;      // 0x10
    unsigned char mFlag;       // 0x12
    char          _pad13[1];

    gcMsgAttachEntity(const cObjectKeyRef &r);

    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

inline gcMsgAttachEntity::gcMsgAttachEntity(const cObjectKeyRef &r)
    : mHandle1(r.mHandle), mHandle2() {
    mVTable = (void *)0x38ABC0;
    mFlag1  = 0;
    mVTable = (void *)0x38AC20;
    mShort  = -1;
    mFlag   = 0;
}

// -----------------------------------------------------------------------------
// 0x002856cc — gcMsgAttachEntity::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgAttachEntity::New(nwMsgBuffer &buf) {
    int cur = buf.mOffset + 0x14;
    buf.mOffset = cur;
    char *obj = (char *)&buf + cur;
    nwMsg *result = 0;
    if (obj != 0) {
        cObjectKeyRef tmp;
        __asm__ volatile ("" ::: "memory");
        new (obj) gcMsgAttachEntity(tmp);
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x00285680 — gcMsgAttachEntity::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sAttachEntityType;

nwMsgType *gcMsgAttachEntity::GetType() const {
    if (!sAttachEntityType) {
        sAttachEntityType = nwMsgType::InitializeType(
            0x1A, 0, "gcMsgAttachEntity", &gcMsgAttachEntity::New);
    }
    return sAttachEntityType;
}

// -----------------------------------------------------------------------------
// 0x001356f4 — gcMsgAttachEntity::Write
// -----------------------------------------------------------------------------

void gcMsgAttachEntity::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    mHandle1.Write(s);
    mHandle2.Write(s);
    int n = ((int)mShort << 16) >> 16;
    ((cOutStreamRef *)&s)->Write(n, 9, true);
    ((cOutStreamBit *)&s)->WriteBit(mFlag);
}
