class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(int, int, bool);
};

class cInStreamRef {
public:
    void Read(int &, int, bool);
};

class gcMsgConnected {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;
    signed char mState;
};

class gcMsgAckPlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

extern char gcMsgConnectedvirtualtable[];
extern char gcMsgAckPlayerStatevirtualtable[];
extern int gConnectedState;

static nwMsgType *sConnectedType;

nwMsg *gcMsgConnected::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = gcMsgConnectedvirtualtable;
        *(char *)(p + 4) = 0;
        result = (nwMsg *)p;
    }
    return result;
}

nwMsgType *gcMsgConnected::GetType() const {
    if (!sConnectedType) {
        sConnectedType = nwMsgType::InitializeType(0x13, 0, "gcMsgConnected", &gcMsgConnected::New);
    }
    return sConnectedType;
}

void gcMsgConnected::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cOutStreamRef *)&s)->Write((int)mState, 8, true);
}

void gcMsgConnected::Read(cInStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) {
    int value;
    ((cInStreamRef *)&s)->Read(value, 8, true);
    mState = (char)value;
    gConnectedState = mState;
}

nwMsg *gcMsgAckPlayerState::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(char **)p = gcMsgAckPlayerStatevirtualtable;
        *(int *)(p + 4) = 0;
        result = (nwMsg *)p;
    }
    return result;
}
