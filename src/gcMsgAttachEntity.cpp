// gcMsgAttachEntity — gcAll_psp.obj
//   0x001356f4 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const   (228B)
//   0x001357d8 Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)            (516B)
//   0x00285680 GetType(void) const                                                                (76B)
//   0x002856cc New(nwMsgBuffer &) static                                                          (116B)

class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
class gcEntity;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

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
    void Read(cInStream &);
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

class cInStreamRef {
public:
    void Read(int &v, int bits, bool sign);   // OOL
};

// cInStream layout: 0x00 mBuf, 0x04 mCapacity, 0x08 mBitOffset.
class cInStreamBit {
public:
    char *mBuf;          // 0x00
    int   mCapacity;     // 0x04
    int   mBitOffset;    // 0x08
};

class gcMap {
public:
    static bool IsMapLoaded(bool);
};

class gcStreamedCinematic {
public:
    static bool IsFullscreenInProgress();
};

class gcEntity {
public:
    void AttachToParent(gcEntity *parent, int slot, unsigned char flag);
};

struct cSomeGlobal {
    char _pad[0x140];
    int  mFlags;     // offset 0x140
};

extern cSomeGlobal *D_0037D7FC;
extern void *D_00038890[];

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
        int bit     = pos & 7;
        char *buf   = mBuf;
        int byteIdx = pos >> 3;
        int newPos  = pos + 1;
        unsigned char ovf = mOverflow;
        unsigned char vb  = v;
        unsigned char *p = (unsigned char *)(buf + byteIdx);
        mBitOffset = newPos;
        if (ovf == 0) {
            if (mCapacity < ((mBitOffset + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        unsigned char ck = (mOverflow == 0);
        if ((unsigned char)(ck & 0xFF)) {
            unsigned char cur = *p;
            int notMask = ~(1 << bit);
            int bv   = (vb != 0) ? 1 : 0;
            *p = (unsigned char)((cur & notMask) | (bv << bit));
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
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
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
    bool sign = true;
    int n = ((int)mShort << 16) >> 16;
    ((cOutStreamRef *)&s)->Write(n, 9, sign);
    ((cOutStreamBit *)&s)->WriteBit(mFlag);
}

// -----------------------------------------------------------------------------
// 0x001357d8 — gcMsgAttachEntity::Read
// -----------------------------------------------------------------------------

void gcMsgAttachEntity::Read(cInStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) {
    if (!gcMap::IsMapLoaded(false)) goto abort;
    {
        int x = D_0037D7FC->mFlags & 1;
        int v = 1;
        if (x != 0) v = 0;
        if (v != 0) goto abort;
    }
    if (gcStreamedCinematic::IsFullscreenInProgress()) goto abort;
    goto skip_abort;
abort:
    ((cInStreamBit *)&s)->mBitOffset = ((cInStreamBit *)&s)->mCapacity * 8;
    return;
skip_abort:

    mHandle1.Read(s);
    mHandle2.Read(s);
    {
        int rv;
        ((cInStreamRef *)&s)->Read(rv, 9, true);
        mShort = (short)rv;
    }

    {
        // Inline ReadBit for mFlag
        cInStreamBit *bs = (cInStreamBit *)&s;
        int pos0 = bs->mBitOffset;
        char *buf = bs->mBuf;
        int byteIdx = pos0 >> 3;
        unsigned char *p = (unsigned char *)(buf + byteIdx);
        int *posPtr = &bs->mBitOffset;
        int curPos = *posPtr;
        unsigned char cur = *p;
        int bit = curPos & 7;
        int newPos = curPos + 1;
        int one = 1;
        *posPtr = newPos;
        int mask = one << bit;
        unsigned char bv = (unsigned char)((cur & mask) != 0);

        int h1 = mHandle1.mIndex;
        mFlag = bv;

        // Inline AttachLookupCheck(h1) — h1==0 branch joined with NULL check
        gcEntity *e1c = 0;
        if (h1 != 0) {
            void *e = D_00038890[h1 & 0xFFFF];
            if (e != 0 && *(int *)((char *)e + 0x30) == h1) e1c = (gcEntity *)e;
        }
        if (e1c == 0) return;

        // Inline AttachLookupCheck(h2)
        int h2 = mHandle2.mIndex;
        gcEntity *e2a = 0;
        if (h2 != 0) {
            void *e = D_00038890[h2 & 0xFFFF];
            if (e != 0 && *(int *)((char *)e + 0x30) == h2) e2a = (gcEntity *)e;
        }
        if (e2a == 0) return;

        // Inline AttachLookupRaw(h1) — barrier prevents CSE with first h1 load
        __asm__ volatile("" ::: "memory");
        gcEntity *e1r = 0;
        if (h1 != 0) e1r = (gcEntity *)D_00038890[h1 & 0xFFFF];

        // Inline AttachLookupCheck(h2) again — barrier prevents CSE with first h2 lookup
        __asm__ volatile("" ::: "memory");
        gcEntity *e2b = 0;
        if (h2 != 0) {
            void *e = D_00038890[h2 & 0xFFFF];
            if (e != 0 && *(int *)((char *)e + 0x30) == h2) e2b = (gcEntity *)e;
        }

        e1r->AttachToParent(e2b, mShort, mFlag);
    }
}
