// nwConnection::Close — tear down a connection: send a final close packet
// through the transport, or notify the lobby, then clear the state field.

class nwAddress;
class nwOutPacket;
class cOutStream {
public:
    void Write(unsigned int, int, bool);
};

// A {short offset; short pad; void(*fn)(...)} dispatch record, as used by the
// transport / lobby thunk tables (mirrors nwConnectionDeleteRecord elsewhere).
struct nwCloseRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct nwLobbyRecord {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

class nwTransport {
public:
    int mUnk0;       // 0x00
    void **mTable;   // 0x04 — points to a record block (+0x30 holds the dispatch record)
};

class nwSocket {
public:
    nwTransport *mTransport;   // 0x00
};

class nwConnection {
public:
    void Close();
    void TransportSend(const nwAddress &, const nwOutPacket &);

    nwSocket *mSocket;     // 0x00
    int mHandle;           // 0x04
    int mAddrHeader;       // 0x08
    char _pad0C[0x14];     // 0x0C
    int mState;            // 0x20
};

class nwNetwork {
public:
    static void **GetLobby();
};

// The stack-built outgoing close stream: a small cOutStream header followed by
// a 1200-byte scratch buffer.
struct nwCloseStream {
    void *mBuffer;    // 0x00 — points at mScratch
    int mCapacity;    // 0x04 — 0x4b0
    int mField08;     // 0x08
    int mField0C;     // 0x0C
    unsigned char mFlag10;   // 0x10
    unsigned char mFlag11;   // 0x11
    unsigned char mFlag12;   // 0x12
    char _pad13;             // 0x13
    int mField14;     // 0x14
    char mScratch[0x4b0];  // 0x18
};

struct nwCloseFrame {
    nwCloseStream stream;  // 0x00 .. 0x4c8
    void *transport;       // 0x4c8 — spilled transport pointer
};

void nwConnection::Close() {
    int state = mState;
    if (state != 0) {
        if (state == 3) {
            if (nwNetwork::GetLobby() != 0) {
                void **lobby = nwNetwork::GetLobby();
                void *obj = *lobby;
                int handle = mHandle;
                nwLobbyRecord *rec = (nwLobbyRecord *)((char *)obj + 0x70);
                short off = rec->offset;
                void (*fn)(void *, int) = rec->fn;
                fn((char *)lobby + off, handle);
            }
        } else {
            nwCloseFrame frame;
            unsigned char flag = 0;
            nwTransport *transport = mSocket->mTransport;
            frame.stream.mBuffer = frame.stream.mScratch;
            frame.stream.mCapacity = 0x4b0;
            frame.stream.mField08 = 0;
            frame.stream.mField0C = 0;
            frame.stream.mFlag10 = flag;
            frame.stream.mFlag11 = 0;
            frame.stream.mFlag12 = 0;
            frame.stream.mField14 = 0;
            frame.transport = transport;

            void *table = transport->mTable;
            nwCloseRecord *rec = (nwCloseRecord *)((char *)table + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)transport + off, &frame.stream);

            ((cOutStream *)&frame.stream)->Write(5, 4, 0);
            TransportSend(*(const nwAddress *)&mAddrHeader, *(const nwOutPacket *)&frame.stream);
        }
        mState = 0;
    }
}
