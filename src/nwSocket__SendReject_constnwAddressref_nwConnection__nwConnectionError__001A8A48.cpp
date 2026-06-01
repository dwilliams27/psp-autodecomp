// nwSocket::SendReject — split-TU. Mirrors nwSocket::SendConnect structure:
// build a cOutStream on the stack, invoke the transport vtable entry at +0x30
// to begin a packet, write the reject id + error code, then Send.

class nwAddress;
class cOutStream;
class nwOutPacket;

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

class nwConnection {
public:
    enum nwConnectionError {
        ERROR_NONE = 0,
        ERROR_TIMEOUT = 1,
        ERROR_REJECTED = 2
    };
};

class nwSocket {
public:
    nwTransport *mTransport;        // 0x00

    void SendReject(const nwAddress &, nwConnection::nwConnectionError);
};

// ------------------------------------------------------------------
void nwSocket::SendReject(const nwAddress &addr,
                          nwConnection::nwConnectionError error) {
    struct {
        unsigned char lead[0x100];
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

    pkt.out.Write((unsigned int)1, 4, false);
    pkt.out.Write((int)error, 0x20, true);

    mTransport->Send(addr, *(const nwOutPacket *)&pkt.out);
}
