// gcMsgUpdateGameState / gcMsgUpdateMapState — message classes from gcAll_psp.obj.
// Decompiled functions:
//   0x001306c4 gcMsgUpdateGameState::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)  (208B)
//   0x0027e070 gcMsgUpdateGameState::GetType(void) const   (76B)
//   0x00284ef8 gcMsgUpdateMapState::GetType(void) const    (76B)

class nwMsg;
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
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

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
};

class gcReplicationManager {
public:
    void *mVtable;
    char _pad4[0xC0 - 4];
    struct VTEntry { short thisOff; short pad; void (*fn)(void *, gcReplicationVisitor *); } *mDispatch;
};

class gcMsgUpdateGameState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;       // 0x00
    bool mFlag;          // 0x04
};

class gcMsgUpdateMapState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x001306c4 — gcMsgUpdateGameState::Read
//   Reads one bit from the cInStream into mFlag, then constructs a stack
//   gcReplicationVisitor (mode=0x802, byte-swap-disabled), wires it to the
//   network stream via SetNetStream, and dispatches a virtual method at
//   offset 0xC0 of the global gcReplicationManager (at 0x37D854).
// -----------------------------------------------------------------------------
void gcMsgUpdateGameState::Read(cInStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    volatile nwSocketHandle vol_sock = sock;
    volatile nwConnectionHandle vol_conn = conn;
    (void)vol_sock;
    cInStreamBits *bs = (cInStreamBits *)&stream;
    int pos = bs->mBitPos;
    unsigned char *data = bs->mBuf;
    int *bpPtr = &bs->mBitPos;
    unsigned char *byte = data + (pos >> 3);
    pos = *bpPtr;
    int bit = pos & 7;
    int mask = 1 << bit;
    pos = pos + 1;
    *bpPtr = pos;
    mFlag = (*byte & mask) != 0;

    nwConnectionHandle local_conn = vol_conn;
    gcReplicationVisitor visitor;
    visitor.mMode = 0x802;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetStream(&stream, local_conn, mFlag, true);

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

// -----------------------------------------------------------------------------
// 0x0027e070 — gcMsgUpdateGameState::GetType
//   Lazily creates the nwMsgType singleton (id=0x0E).
// -----------------------------------------------------------------------------

static nwMsgType *sUpdateGameStateType;

nwMsgType *gcMsgUpdateGameState::GetType() const {
    if (!sUpdateGameStateType) {
        sUpdateGameStateType = nwMsgType::InitializeType(
            0x0E, 0, "gcMsgUpdateGameState", &gcMsgUpdateGameState::New);
    }
    return sUpdateGameStateType;
}

// -----------------------------------------------------------------------------
// 0x00284ef8 — gcMsgUpdateMapState::GetType
//   Lazily creates the nwMsgType singleton (id=0x0F).
// -----------------------------------------------------------------------------

static nwMsgType *sUpdateMapStateType;

nwMsgType *gcMsgUpdateMapState::GetType() const {
    if (!sUpdateMapStateType) {
        sUpdateMapStateType = nwMsgType::InitializeType(
            0x0F, 0, "gcMsgUpdateMapState", &gcMsgUpdateMapState::New);
    }
    return sUpdateMapStateType;
}
