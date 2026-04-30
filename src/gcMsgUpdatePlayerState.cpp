// gcMsgUpdatePlayerState — gcAll_psp.obj
//   0x00136058 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const

class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class cOutStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;
    int mCRC;
    unsigned char mDirty;
    char _pad11;
    unsigned char mOverflow;
    char _pad13;
    int mCRCBitPos;

    void Write(unsigned int, int, bool);
};

class cOutStreamBit {
public:
    unsigned char *mBuf;
    int mCapacity;
    int mBitOffset;
    char _pad0C[6];
    unsigned char mOverflow;

    __attribute__((always_inline)) void WriteBit(unsigned char v) {
        int pos = mBitOffset;
        int bit = pos & 7;
        unsigned char *buf = mBuf;
        int byteIdx = pos >> 3;
        int newPos = pos + 1;
        unsigned char *p = buf + byteIdx;
        unsigned char ovf = mOverflow;
        unsigned char vb = v;
        mBitOffset = newPos;
        if (ovf == 0) {
            if (mCapacity < ((mBitOffset + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        unsigned char ck = (mOverflow == 0);
        if ((unsigned char)(ck & 0xFF)) {
            unsigned char cur = *p;
            int mask = ~(1 << bit);
            int bv = (vb != 0) ? 1 : 0;
            *p = (unsigned char)((cur & mask) | (bv << bit));
        }
    }
};

class gcReplicationVisitor {
public:
    int mMode;
    cOutStream *mOutStream;
    void *mInStream;
    int mStreamPos;
    int mNetConnection;
    int mReadActive;
    bool mWriteEnabled;
    bool mByteSwap;

};

extern "C" void __0fUgcReplicationVisitorMSetNetStreamP6KcOutStream6SnwConnectionHandlebTD(
    gcReplicationVisitor *, cOutStream *, int, unsigned char, bool);

struct gcTypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, gcReplicationVisitor *);
};

class gcMsgUpdatePlayerState {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    void *mVTable;
    int mPlayerIndex;
    unsigned char mFlag;
};

void gcMsgUpdatePlayerState::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle conn) const {
    ((cOutStreamBit *)&s)->WriteBit(mFlag);
    s.Write((unsigned int)mPlayerIndex, 4, true);

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    int localConn = conn.mValue;
    __asm__ volatile("" : "+r"(localConn));
    visitor.mReadActive = 0;
    unsigned char flag = mFlag;
    visitor.mWriteEnabled = true;
    __0fUgcReplicationVisitorMSetNetStreamP6KcOutStream6SnwConnectionHandlebTD(
        &visitor, &s, localConn, flag, true);

    int index = mPlayerIndex;
    void *player = 0;
    void *playerCheck;
    if (index >= 0) {
        if (index < 8) {
            player = *(char **)0x37D87C + index * 68;
        }
        goto check_player;
    } else {
check_player:
        playerCheck = player;
    }
    if (playerCheck != 0) {
        char *type = *(char **)((char *)playerCheck + 4);
        gcTypeDispatchEntry *entry = (gcTypeDispatchEntry *)(type + 0x70);
        entry->fn((char *)player + entry->offset, &visitor);
    }
}
