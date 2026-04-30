// gcMsgUpdateGameState / gcMsgUpdateMapState — message classes from gcAll_psp.obj.
// Decompiled functions:
//   0x001306c4 gcMsgUpdateGameState::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)  (208B)
//   0x0027e070 gcMsgUpdateGameState::GetType(void) const   (76B)
//   0x00284ef8 gcMsgUpdateMapState::GetType(void) const    (76B)

class nwMsg;
class cInStream;
class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

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

struct cOutStreamBits {
    unsigned char *mBuf;     // 0x00
    int mCapacity;           // 0x04
    int mBitPos;             // 0x08
    int mCRC;                // 0x0C
    unsigned char mDirty;    // 0x10
    unsigned char mPad11;    // 0x11
    unsigned char mOverflow; // 0x12
};

class cGUID {
public:
    void Write(cOutStream &) const;
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
    char _pad4[0xC0 - 4];
    struct VTEntry { short thisOff; short pad; void (*fn)(void *, gcReplicationVisitor *); } *mDispatch;
};

class gcMsgUpdateGameState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    void *mVTable;       // 0x00
    bool mFlag;          // 0x04
};

class gcMsgUpdateMapState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    void *mVTable;       // 0x00
    bool mFlag;          // 0x04
    char _pad5[3];
    cGUID mGuid;         // 0x08
};

// -----------------------------------------------------------------------------
// 0x001306c4 — gcMsgUpdateGameState::Read
//   Reads one bit from the cInStream into mFlag, then constructs a stack
//   gcReplicationVisitor (mode=0x802, byte-swap-disabled), wires it to the
//   network stream via SetNetStream, and dispatches a virtual method at
//   offset 0xC0 of the global gcReplicationManager (at 0x37D854).
// -----------------------------------------------------------------------------
void gcMsgUpdateGameState::Read(cInStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    (void)sock;
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

// -----------------------------------------------------------------------------
// 0x00130598 — gcMsgUpdateGameState::Write
// -----------------------------------------------------------------------------
void gcMsgUpdateGameState::Write(cOutStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) const {
    (void)sock;

    cOutStreamBits *bs = (cOutStreamBits *)&stream;
    int flag = mFlag & 0xFF;
    int pos = bs->mBitPos;
    int bit = pos & 7;
    unsigned char *data = bs->mBuf;
    unsigned char *byte = data + (pos >> 3);
    pos = pos + 1;
    unsigned char overflow = bs->mOverflow;
    bs->mBitPos = pos;


    if (overflow == 0) {
        int newPos = ((cOutStreamBits *)&stream)->mBitPos;
        int size = ((cOutStreamBits *)&stream)->mCapacity;
        if (size < ((newPos + 7) >> 3)) {
            ((cOutStreamBits *)&stream)->mOverflow = 1;
        }
    }

    if ((unsigned char)(((cOutStreamBits *)&stream)->mOverflow == 0)) {
        unsigned char cur = *byte;
        int mask = ~(1 << bit);
        int bitValue = (flag != 0);
        *byte = (unsigned char)((cur & mask) | (bitValue << bit));
    }

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
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

// -----------------------------------------------------------------------------
// 0x0013341c — gcMsgUpdateMapState::Write
// -----------------------------------------------------------------------------
void gcMsgUpdateMapState::Write(cOutStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) const {
    (void)sock;

    mGuid.Write(stream);
    int flag = mFlag & 0xFF;
    int pos = ((cOutStreamBits *)&stream)->mBitPos;
    unsigned char *data = ((cOutStreamBits *)&stream)->mBuf;
    int bit = pos & 7;
    unsigned char *byte = data + (pos >> 3);
    pos = pos + 1;
    unsigned char overflow = ((cOutStreamBits *)&stream)->mOverflow;
    ((cOutStreamBits *)&stream)->mBitPos = pos;


    if (overflow == 0) {
        int newPos = ((cOutStreamBits *)&stream)->mBitPos;
        int capacity = ((cOutStreamBits *)&stream)->mCapacity;
        int size = capacity;
        if (size < ((newPos + 7) >> 3)) {
            ((cOutStreamBits *)&stream)->mOverflow = 1;
        }
    }

    if ((unsigned char)(((cOutStreamBits *)&stream)->mOverflow == 0)) {
        unsigned char cur = *byte;
        int mask = ~(1 << bit);
        int bitValue = (flag != 0);
        *byte = (unsigned char)((cur & mask) | (bitValue << bit));
    }

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetStream(&stream, conn, mFlag, true);

    gcReplicationManager *mgr = *(gcReplicationManager **)0x37D7FC;
    if (mgr) {
        char *vtable = *(char **)((char *)mgr + 4);
        struct Entry { short thisOff; short pad; void (*fn)(void *, gcReplicationVisitor *); };
        Entry *entry = (Entry *)(vtable + 0xD0);
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
