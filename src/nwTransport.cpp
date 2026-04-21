// nwTransport: virtual-dispatch network transport wrapper. Thin stubs that
// forward to function pointers stored in the vtable-like table at +0x04.

extern "C" void *memset(void *, int, unsigned int);

// ---------------------------------------------------------------------------
// Internal pattern shared with nwSocket.cpp — virtual dispatch through
// 8-byte entries of { short adjust; short pad; void *fn; }.
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

// ---------------------------------------------------------------------------
class nwAddress {
public:
    int mType;
    char mData[16];
    nwAddress();
};

inline nwAddress::nwAddress() {
    mType = 0;
    memset(mData, 0, 16);
}

class nwOutPacket {
public:
    int mUnk0;
    int mUnk4;
    int mSizeBits;  // offset 0x08
};

class cInStream {};
class nwInPacket : public cInStream {
public:
    int mUnk0;
    int mUnk4;
    int mOffset;  // at +0x08
};

class nwTransport {
public:
    void Destroy(void);
    bool Send(const nwAddress &, const nwOutPacket &);
    void Receive(nwInPacket *, nwAddress *, int *);
};

// ---------------------------------------------------------------------------
void nwTransport::Destroy(void) {
    if (this != 0) {
        TransportObj *t = (TransportObj *)this;
        VtEntry *ent = (VtEntry *)((char *)t->vtable + 0x40);
        typedef void (*Fn)(void *, int);
        ((Fn)ent->fn)((char *)t + ent->adjust, 3);
    }
}

// ---------------------------------------------------------------------------
bool nwTransport::Send(const nwAddress &addr, const nwOutPacket &pkt) {
    if (((pkt.mSizeBits + 7) >> 3) == 0) {
        return true;
    }
    TransportObj *t = (TransportObj *)this;
    VtEntry *ent = (VtEntry *)((char *)t->vtable + 0x20);
    typedef bool (*Fn)(void *, const nwAddress &, const nwOutPacket &);
    return ((Fn)ent->fn)((char *)t + ent->adjust, addr, pkt);
}

// ---------------------------------------------------------------------------
void nwTransport::Receive(nwInPacket *inPkt, nwAddress *addr, int *errCode) {
    nwAddress local;
    *addr = local;
    *errCode = 0;
    inPkt->mOffset = 0;

    TransportObj *t = (TransportObj *)this;
    VtEntry *ent = (VtEntry *)((char *)t->vtable + 0x28);
    typedef void (*Fn)(void *, nwInPacket *, nwAddress *, int *);
    ((Fn)ent->fn)((char *)t + ent->adjust, inPkt, addr, errCode);
}

// ---------------------------------------------------------------------------
extern "C" int wcslen(const unsigned short *str) {
    const unsigned short *p = str;
    while (*p) p++;
    return p - str;
}

// ---------------------------------------------------------------------------
class nwMsg;
struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class gcMsgUpdateGameState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

extern char gcMsgUpdateGameStatevirtualtable[];

nwMsg *gcMsgUpdateGameState::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        char flag = 0;
        *(char **)p = gcMsgUpdateGameStatevirtualtable;
        *(char *)(p + 4) = flag;
        result = (nwMsg *)p;
    }
    return result;
}
