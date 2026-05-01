// nwSocket: network socket wrapper around a transport, with a fixed-size
// connection array.

class nwAddress;
class cOutStream;
class nwOutPacket;

class nwSocketHandle {
public:
    int mValue;
};

class nwConnectionHandle {
public:
    int mValue;
};

class cTimeValue {
public:
    int mValue;
};

namespace {

struct VtEntry {
    short adjust;
    short pad;
    void *fn;
};

struct TransportObj {
    void *unk0;
    VtEntry *vtable;  // at offset 4
};

struct ConnLite {
    void *unk0;
    nwConnectionHandle mHandle;    // at offset 4
};

}  // namespace

class nwTransport {
public:
    bool Send(const nwAddress &, const nwOutPacket &);
};

class cOutStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;
    int mCRC;
    unsigned char mDirty;
    char mPad11;
    unsigned char mOverflow;
    char _pad13;
    int mCRCBitPos;

    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
};

class nwOutPacket {
public:
    int mUnk0;
    int mUnk4;
    int mSizeBits;
};

class nwConnection {
public:
    enum nwConnectionError {
        ERROR_NONE = 0,
        ERROR_TIMEOUT = 1,
        ERROR_REJECTED = 2
    };
    void SetError(nwConnectionError);
};

class cInStream {
public:
    void Read(int &, int, bool);
};

class nwInPacket : public cInStream {};

class nwSocket {
public:
    nwTransport *mTransport;        // 0x00
    int mField04;                   // 0x04
    int mMaxConnections;            // 0x08
    nwConnection **mConnections;    // 0x0C
    int mHandle;                    // 0x10
    unsigned int mField14;          // 0x14
    char mName[8];                  // 0x18
    unsigned char mFlag20;          // 0x20
    int mField24;                   // 0x24
    int mField28;                   // 0x28
    int mField2C;                   // 0x2C

    nwSocket(nwTransport *, nwSocketHandle, unsigned int, int, int);
    void Destroy(void);
    void AddressFromString(const char *, nwAddress *) const;
    nwConnectionHandle GetConnection(int) const;
    void SendConnect(const nwAddress &, const char *);
    void WriteId(cOutStream &);
    void ReceiveReject(nwInPacket &, nwConnectionHandle);
    void Update(cTimeValue);
    static void UpdateAll(cTimeValue);
    static nwConnection *GetConnection(nwConnectionHandle);
};

extern "C" {
    void cStrCopy(char *, const char *);
    int cStrLength(const char *);
    void nwSocket___dtor_nwSocket_void(nwSocket *, int);
    extern nwSocket *D_00034958[];
}

// ------------------------------------------------------------------
// Virtual dispatch through transport's vtable entry at offset +0x10.
void nwSocket::AddressFromString(const char *str, nwAddress *addr) const {
    TransportObj *t = (TransportObj *)mTransport;
    VtEntry *ent = (VtEntry *)((char *)t->vtable + 0x10);
    typedef void (*Fn)(void *, const char *, nwAddress *);
    ((Fn)ent->fn)((char *)t + ent->adjust, str, addr);
}

// ------------------------------------------------------------------
void nwSocket::Destroy(void) {
    nwSocket___dtor_nwSocket_void(this, 3);
}

// ------------------------------------------------------------------
nwConnectionHandle nwSocket::GetConnection(int idx) const {
    nwConnectionHandle result;
    if (idx >= 0 && idx < mMaxConnections) {
        ConnLite *c = (ConnLite *)mConnections[idx];
        if (c != 0) {
            nwConnectionHandle *p = &c->mHandle;
            return *p;
        }
    }
    result.mValue = 0;
    return result;
}

// ------------------------------------------------------------------
nwSocket::nwSocket(nwTransport *transport, nwSocketHandle handle,
                   unsigned int field14, int maxConn, int field04)
    : mTransport(transport),
      mField04(field04),
      mMaxConnections(maxConn),
      mConnections(0),
      mHandle(handle.mValue),
      mField14(field14),
      mFlag20(0),
      mField24(0),
      mField28(0),
      mField2C(0)
{
    cStrCopy(mName, "");
}

// ------------------------------------------------------------------
void nwSocket::SendConnect(const nwAddress &addr, const char *name) {
    struct {
        cOutStream out;
        unsigned char data[0x4B0];
        nwTransport *transport;
    } pkt;

    nwTransport *transport = mTransport;
    int zero = 0;
    pkt.out.mData = pkt.data;
    pkt.out.mCapacity = 0x4B0;
    pkt.out.mBitPos = 0;
    pkt.out.mCRC = 0;
    pkt.out.mDirty = zero;
    pkt.out.mPad11 = 0;
    pkt.out.mOverflow = 0;
    pkt.out.mCRCBitPos = 0;
    pkt.transport = transport;

    TransportObj *t = (TransportObj *)transport;
    VtEntry *ent = (VtEntry *)((char *)t->vtable + 0x30);
    typedef void (*ConnectFn)(void *, cOutStream &);
    ((ConnectFn)ent->fn)((char *)t + ent->adjust, pkt.out);

    pkt.out.Write((unsigned int)0, 4, false);
    WriteId(pkt.out);

    int length = cStrLength(name) & 0xFFFF;
    {
        unsigned int lenArg = (unsigned int)(length & 0xFFFF);
        bool sign2 = true;
        pkt.out.Write(lenArg, 0x10, sign2);
    }

    int i = 0;
    if (i < length) {
        do {
            bool sign = false;
            signed char v = name[i];
            int iv = v;
            pkt.out.Write((signed char)iv, 8, sign);
            i += 1;
        } while (i < length);
    }

    mTransport->Send(addr, *(const nwOutPacket *)&pkt.out);
}

// ------------------------------------------------------------------
void nwSocket::ReceiveReject(nwInPacket &packet, nwConnectionHandle handle) {
    int rejectedError;

    packet.Read(rejectedError, 32, true);
    if (nwSocket::GetConnection(handle) != 0) {
        nwSocket::GetConnection(handle)->SetError((nwConnection::nwConnectionError)rejectedError);
    }
}

// ------------------------------------------------------------------
void nwSocket::UpdateAll(cTimeValue dt) {
    int i = 0;
    nwSocket **socket = D_00034958;

    do {
        nwSocket *s = *socket;
        if (s != 0) {
            s->Update(dt);
        }
        i += 1;
        socket += 1;
    } while (i < 1);
}
