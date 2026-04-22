// nwConnection: helper methods — StartConnection, BuildMessage, ReceivePing,
// TransportSend, ResendConnect.

typedef int nwSocketHandle;
typedef int nwConnectionHandle;

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
    void ResendConnect();
    void TransportSend(const nwAddress &, const nwOutPacket &);
    void ReceivePing(nwInPacket &);
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
    int mFieldCE8;              // 0xCE8
    float mSendRate;            // 0xCEC
};

// StartConnection: set state, kick off the connect handshake.
void nwConnection::StartConnection() {
    mState = 1;
    mSocket->SendConnect(*(const nwAddress *)&mAddrHeader, (const char *)&mFlag50);
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
