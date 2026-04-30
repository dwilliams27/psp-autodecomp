// nwConnection: helper methods — StartConnection, BuildMessage, ReceivePing,
// TransportSend, ResendConnect.

class nwSocketHandle {
public:
    int mHandle;
};
class nwConnectionHandle {
public:
    int mHandle;
};

class cTimeValue {
public:
    int mTime;
};

class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

class cOutStream {
public:
    char _pad00[0x11];      // 0x00..0x11
    unsigned char mFlag11;  // 0x11 — flag updated by BuildMessage
    char _pad12[0x10];
};

class nwAddress {
public:
    int mType;        // 0x00
    char mData[16];   // 0x04..0x14
};

class nwOutPacket {
public:
    int mUnk0;        // 0x00
    int mUnk4;        // 0x04
    int mSizeBits;    // 0x08
};

class nwInPacket : public cInStream {};

class nwMsg {
public:
    virtual void m0();
    virtual void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

class nwTransport {
public:
    int Send(const nwAddress &, const nwOutPacket &);
};

class nwSocket {
public:
    nwTransport *mTransport;   // 0x00
    void SendConnect(const nwAddress &, const char *);
};

extern int D_0009F984;

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE  = 1,
        SEND_ORDERED   = 2
    };
    enum nwConnectionError {
        ERROR_NONE     = 0,
        ERROR_TIMEOUT  = 1,
        ERROR_REJECTED = 2
    };

    void StartConnection();
    ~nwConnection();
    void ResendConnect();
    void TransportSend(const nwAddress &, const nwOutPacket &);
    void ReceivePing(nwInPacket &);
    void ReceivePingReply(nwInPacket &);
    int Accept();
    void Send(nwSendMethod, const nwMsg &, unsigned int, cOutStream *);
    int AllocateBuffers();
    float GetQuality() const;
    void UpdateSendRate(cTimeValue);
    void SetError(nwConnectionError);
    static unsigned char BuildMessage(const nwMsg &, cOutStream &, nwSocketHandle,
                                      const nwAddress &, nwConnectionHandle);

    nwSocket *mSocket;          // 0x00
    int mHandle;                // 0x04
    int mAddrHeader;            // 0x08 — first int of address arg (&this+8 passed as nwAddress&)
    int mAddrData[4];           // 0x0C..0x1C
    int mField1C;               // 0x1C
    int mState;                 // 0x20
    int mField24;               // 0x24
    int mField28;               // 0x28
    int mSeqNumber;             // 0x2C
    int mField30;               // 0x30
    int mField34;               // 0x34
    int mField38;               // 0x38
    int mField3C;               // 0x3C
    int mLastAckTick;           // 0x40
    int mField44;               // 0x44
    int mPingSamples;           // 0x48
    float mSmoothedPing;        // 0x4C
    unsigned char mFlag50;      // 0x50
    char _pad51[7];             // 0x51..0x57
    void *mErrorCallback;       // 0x58
    char mBlocks[0xC78];        // 0x5C..0xCD4
    void *mErrorCallback2;      // 0xCD4
    int mFieldCD8;              // 0xCD8
    int mFieldCDC;              // 0xCDC
    int mFieldCE0;              // 0xCE0
    int mFieldCE4;              // 0xCE4
    unsigned int mFieldCE8;     // 0xCE8
    float mSendRate;            // 0xCEC

    static void operator delete(void *p);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct nwConnectionDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct nwConnectionDeleteRecord4 {
    short offset;
    short pad;
    void (*fn)(void *, void *, void *, short);
};

extern "C" void free(void *);

inline void nwConnection::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        nwConnectionDeleteRecord *rec =
            (nwConnectionDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

// StartConnection: set state, kick off the connect handshake.
void nwConnection::StartConnection() {
    mState = 1;
    mSocket->SendConnect(*(const nwAddress *)&mAddrHeader, (const char *)&mFlag50);
}

nwConnection::~nwConnection() {
    if (mFieldCD8 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr((void *)mFieldCD8);
        char *block = ((char **)pool)[9];
        nwConnectionDeleteRecord4 *rec =
            (nwConnectionDeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *, void *, short) = rec->fn;
        fn(block + off, (void *)mFieldCD8, (void *)fn, off);
        mFieldCD8 = 0;
    }
    if (mFieldCE0 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr((void *)mFieldCE0);
        char *block = ((char **)pool)[9];
        nwConnectionDeleteRecord4 *rec =
            (nwConnectionDeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *, void *, short) = rec->fn;
        fn(block + off, (void *)mFieldCE0, (void *)fn, off);
        mFieldCE0 = 0;
    }
}

// BuildMessage: write the message body through nwMsg's virtual serialize.
unsigned char nwConnection::BuildMessage(const nwMsg &msg, cOutStream &out,
                                         nwSocketHandle h, const nwAddress &addr,
                                         nwConnectionHandle ch) {
    out.mFlag11 = 1;
    const_cast<nwMsg &>(msg).Write(out, h, addr, ch);
    return out.mFlag11;
}

// ReceivePing: read incoming ping tick value and scale it into mField44.
void nwConnection::ReceivePing(nwInPacket &packet) {
    unsigned int spill[2];
    packet.Read(spill[1], 12, false);
    spill[0] = spill[1] * 10;
    int *p = &mField44;
    *p = spill[0];
}

// TransportSend: update the bytes-sent counter and hand the packet to transport.
void nwConnection::TransportSend(const nwAddress &addr, const nwOutPacket &pkt) {
    mFieldCE8 += (pkt.mSizeBits + 7) >> 3;
    if (!mSocket->mTransport->Send(addr, pkt)) {
        SetError((nwConnectionError)6);
    }
}

// ReceivePingReply: read the pong tick and compute round-trip in ticks.
void nwConnection::ReceivePingReply(nwInPacket &packet) {
    unsigned int spill[2];
    packet.Read(spill[1], 12, false);
    spill[0] = (mSeqNumber - spill[1] * 10) & 0xFFF;
    int *pPing = &mPingSamples;
    *pPing = spill[0];
    int *pSeq = &mSeqNumber;
    int *pAck = &mLastAckTick;
    *pAck = *pSeq;
}

// Accept: kick the connection into accepted state and ack with a header packet.
int nwConnection::Accept() {
    struct {
        char body[256];
        int vtbl;
    } msg;
    int result;
    if (AllocateBuffers()) {
        mState = 2;
        msg.vtbl = 0x38d9f8;
        Send(SEND_ORDERED, *(const nwMsg *)&msg.vtbl, 0, 0);
        int *pSeq = &mSeqNumber;
        int *pAck = &mLastAckTick;
        result = 1;
        *pAck = *pSeq;
        return result;
    }
    return 0;
}

// GetQuality: map smoothed ping into [0, 1] connection quality.
float nwConnection::GetQuality() const {
    float diff = mSmoothedPing - 0.25f;
    float zero = 0.0f;
    float range = 0.6f - 0.25f;
    float a;
    float one;
    if (diff < zero) {
        one = 1.0f;
        a = zero;
    } else {
        one = 1.0f;
        a = diff;
    }
    if (!(a / range <= one)) {
        return one - one;
    }
    float b;
    if (diff < zero) {
        b = zero;
    } else {
        b = diff;
    }
    return one - b / range;
}

// UpdateSendRate: low-pass filter the bytes-per-tick send rate.
void nwConnection::UpdateSendRate(cTimeValue tv) {
    unsigned int ce8 = mFieldCE8;
    float rate = mSendRate;
    float instant = (float)ce8 / (*(float *)0x36C800 * (float)tv.mTime);
    float alpha = 0.985f;
    if (!(instant <= rate)) alpha = 0.965f;
    mFieldCE8 = 0;
    mSendRate = rate * alpha + instant * (1.0f - alpha);
}

// ResendConnect: if we've waited long enough, resend the connect packet.
void nwConnection::ResendConnect() {
    int threshold = D_0009F984;
    if (threshold < mSeqNumber - mField30) {
        mSocket->SendConnect(*(const nwAddress *)&mAddrHeader, (const char *)&mFlag50);
        int *pSeq = &mSeqNumber;
        int *pField30 = &mField30;
        *pField30 = *pSeq;
    }
}
