// nwConnection::Connect(const char *, bool)
// Address: 0x001a6640, Size: 220B
// Symbol: __0fMnwConnectionHConnectPCcb
//
// Split-TU: nwConnection layout matches src/nwConnection.cpp.

class nwAddress;
class nwSocket;

extern "C" int cIRand(int);
void cStrCopy(char *, const char *, int);

// Lobby dispatch record at (*lobby + 0x68): {short offset; short pad; fn}
struct nwLobbyDispatchRecord {
    short offset;
    short pad;
    int (*fn)(void *, int, short, void *);
};

class nwConnection {
public:
    void StartConnection();
    int AllocateBuffers();
    int Connect(const char *, bool);

    nwSocket *mSocket;          // 0x00
    int mHandle;                // 0x04
    int mAddrHeader;            // 0x08
    int mAddrData[4];           // 0x0C..0x1B
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
};

extern char **nwNetwork__GetLobby();

int nwConnection::Connect(const char *name, bool join) {
    if (!AllocateBuffers()) {
        return 0;
    }
    cStrCopy((char *)&mFlag50, name, 8);
    char **lobby = nwNetwork__GetLobby();
    if (join && lobby != 0) {
        mState = 3;
        mField24 = cIRand(mState) | 1;
        char *vt = *lobby;
        nwLobbyDispatchRecord *rec = (nwLobbyDispatchRecord *)(vt + 0x68);
        short off = rec->offset;
        int (*fn)(void *, int, short, void *) = rec->fn;
        return fn((char *)lobby + off, mHandle, off, (void *)fn);
    }
    StartConnection();
    return 1;
}
