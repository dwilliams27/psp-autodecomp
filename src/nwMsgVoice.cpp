// nwMsgVoice — nwAll_psp.obj
//   0x0036b808 GetType(void) const  (76B)

class nwMsg;
class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

extern "C" void *memcpy(void *, const void *, unsigned int);

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class nwMsgVoice {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    void *mVTable;
    char mField4;
    unsigned short mSize;
    char mData[0x200];
};

class cOutStream {
public:
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
    void WriteRaw(int, const void *);
};

extern char nwConfigPSPvirtualtable[];

// -----------------------------------------------------------------------------
// 0x0036b808 — nwMsgVoice::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sVoiceType;

nwMsgType *nwMsgVoice::GetType() const {
    if (!sVoiceType) {
        sVoiceType = nwMsgType::InitializeType(
            3, 0, "nwMsgVoice", &nwMsgVoice::New);
    }
    return sVoiceType;
}

// -----------------------------------------------------------------------------
// 0x001a59c4 — nwMsgVoice::Write
// -----------------------------------------------------------------------------

void nwMsgVoice::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    register int one __asm__("$18") = 1;
    int value = mField4 << 24;
    value >>= 24;
    s.Write(value, 4, one);
    unsigned int sizeBits = mSize & 0xffff;
    s.Write(sizeBits, 10, one);
    s.WriteRaw(mSize, mData);
}

// -----------------------------------------------------------------------------
// 0x0036b854 — nwMsgVoice::New
// -----------------------------------------------------------------------------

nwMsg *nwMsgVoice::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x208;
    buf.mOffset = cursor;
    nwMsgVoice *obj = (nwMsgVoice *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        void *vt = nwConfigPSPvirtualtable;
        int z4 = 0;
        int z6 = 0;
        obj->mVTable = vt;
        obj->mField4 = z4;
        obj->mSize = z6;
        memcpy(obj->mData, 0, z6);
        result = (nwMsg *)obj;
    }
    return result;
}
