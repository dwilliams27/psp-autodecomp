// ODR-WARNING: split-TU isolated reconstruction of gcMsgUpdateGameState::Read.
// 0x001306c4 gcMsgUpdateGameState::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)  (208B)
// Class declared locally to avoid touching shared TUs / headers.

class cInStream;
class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

struct cInStreamBits {
    unsigned char *mBuf;     // 0x0
    int mCapacity;           // 0x4
    int mBitPos;             // 0x8
};

class gcReplicationVisitor {
public:
    int mMode;                // 0x00
    void *mOutStream;         // 0x04
    void *mInStream;          // 0x08
    int mStreamPos;           // 0x0C
    int mNetConnection;       // 0x10
    int mReadActive;          // 0x14
    bool mWriteEnabled;       // 0x18
    bool mByteSwap;           // 0x19

    void SetNetStream(cInStream *, nwConnectionHandle, bool, bool);
    void SetNetStream(cOutStream *, nwConnectionHandle, bool, bool);
};

class gcReplicationManager {
public:
    void *mVtable;
};

class gcMsgUpdateGameState {
public:
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;       // 0x00
    bool mFlag;          // 0x04
};

void gcMsgUpdateGameState::Read(cInStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    (void)sock;
    cInStreamBits *bs = (cInStreamBits *)&stream;
    unsigned char *data = bs->mBuf;
    int pos = bs->mBitPos;
    unsigned char *byte = data + (pos >> 3);
    int *bpPtr = &bs->mBitPos;
    pos = *bpPtr;
    unsigned char b = *byte;
    int bit = pos & 7;
    int mask = 1 << bit;
    pos = pos + 1;
    *bpPtr = pos;
    mFlag = (b & mask) != 0;

    gcReplicationVisitor visitor;
    visitor.mMode = 0x802;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetStream(&stream, conn, mFlag, true);

    gcReplicationManager *mgr = *(gcReplicationManager **)0x37D854;
    if (mgr) {
        char *vtable = *(char **)((char *)mgr + 4);
        struct Entry { short thisOff; short pad; void (*fn)(void *, gcReplicationVisitor *); };
        Entry *entry = (Entry *)(vtable + 0xC0);
        short off = entry->thisOff;
        void (*fn)(void *, gcReplicationVisitor *) = entry->fn;
        fn((char *)mgr + off, &visitor);
    }
}
