// gcMsgRequestPlayerId — gcAll_psp.obj
//   0x001336a4 Write(cOutStream &, ...) const     (224B)
//   0x00284fac GetType(void) const                (76B)
//   0x00284ff8 New(nwMsgBuffer &) static          (108B)
//
// Layout (0x24 bytes):
//   0x00 vtable
//   0x04 int    mNum    (init -1)
//   0x08 char   mName[0x14]  (cStrCopy from rodata @0x36D944)
//   0x1C float  mF      (init 0.0f)
//   0x20 uint   mU      (init 0)

class nwMsg;
class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
    void Write(float, bool);
};

extern "C" int  cStrLength(const char *);
extern "C" void cStrCopy(char *, const char *, int);

class gcMsgRequestPlayerId {
public:
    void *       mVTable;     // 0x00
    int          mNum;        // 0x04
    signed char  mName[0x14]; // 0x08
    float        mF;          // 0x1C
    unsigned int mU;          // 0x20

    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

// -----------------------------------------------------------------------------
// 0x00284fac — gcMsgRequestPlayerId::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sRequestPlayerIdType;

nwMsgType *gcMsgRequestPlayerId::GetType() const {
    if (!sRequestPlayerIdType) {
        sRequestPlayerIdType = nwMsgType::InitializeType(
            0xA, 0, "gcMsgRequestPlayerId", &gcMsgRequestPlayerId::New);
    }
    return sRequestPlayerIdType;
}

// -----------------------------------------------------------------------------
// 0x00284ff8 — gcMsgRequestPlayerId::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgRequestPlayerId::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x24;
    buf.mOffset = cursor;
    gcMsgRequestPlayerId *obj = (gcMsgRequestPlayerId *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        obj->mVTable = (void *)0x38AAE0;
        obj->mNum    = -1;
        obj->mF      = 0.0f;
        obj->mU      = 0;
        cStrCopy((char *)&obj->mName[0], (const char *)0x36D944, 0x14);
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x001336a4 — gcMsgRequestPlayerId::Write
// -----------------------------------------------------------------------------

void gcMsgRequestPlayerId::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cOutStreamRef *)&s)->Write(mNum, 5, true);
    int len = cStrLength((const char *)&mName[0]) & 0xFFFF;
    {
        unsigned int len_arg = (unsigned int)(len & 0xFFFF);
        bool sign2 = true;
        ((cOutStreamRef *)&s)->Write(len_arg, 0x10, sign2);
    }
    for (int i = 0; i < len; i++) {
        bool sign = false;
        signed char v = mName[i];
        int iv = v;
        ((cOutStreamRef *)&s)->Write((signed char)iv, 8, sign);
    }
    ((cOutStreamRef *)&s)->Write(mF, true);
    ((cOutStreamRef *)&s)->Write((unsigned int)mU, 0x20, true);
}
