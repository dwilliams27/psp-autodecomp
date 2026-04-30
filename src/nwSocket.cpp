// nwSocket: network socket wrapper around a transport, with a fixed-size
// connection array.

typedef int nwConnectionHandle;

class nwAddress;

class nwSocketHandle {
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
    int   mHandle;    // at offset 4
};

}  // namespace

class nwTransport;

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
    void ReceiveReject(nwInPacket &, nwConnectionHandle);
};

extern "C" {
    void cStrCopy(char *, const char *);
    void nwSocket___dtor_nwSocket_void(nwSocket *, int);
    nwConnection *nwSocket__GetConnection_nwConnectionHandlestatic(nwConnectionHandle);
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
    volatile int result;
    if (idx >= 0 && idx < mMaxConnections) {
        ConnLite *c = (ConnLite *)mConnections[idx];
        if (c != 0) {
            int *p = &c->mHandle;
            return *p;
        }
    }
    result = 0;
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
void nwSocket::ReceiveReject(nwInPacket &packet, nwConnectionHandle handle) {
    int spill[2];  // sp[0]=handle spill, sp[4]=value
    spill[0] = handle;
    packet.Read(spill[1], 32, true);
    nwConnectionHandle h = spill[0];
    if (nwSocket__GetConnection_nwConnectionHandlestatic(h) != 0) {
        nwSocket__GetConnection_nwConnectionHandlestatic(h)->SetError((nwConnection::nwConnectionError)spill[1]);
    }
}
