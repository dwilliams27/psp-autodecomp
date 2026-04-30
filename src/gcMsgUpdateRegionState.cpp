// gcMsgUpdateRegionState — gcAll_psp.obj
//   0x00136880 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const
//   0x002859c4 GetType(void) const               (76B)
//   0x00285a10 New(nwMsgBuffer &) static         (104B)
//
// Layout (0x10 bytes):
//   0x00 vtable
//   0x04 char    mB
//   0x08 8 bytes default-initialised via stack temp

class nwMsg;
class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cGUID {
public:
    void Write(cOutStream &) const;
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

template <class T>
class cGUIDT {
public:
    int mA;
    int mB;
};

class gcRegion;

class gcMap {
public:
    gcRegion *FindLoadedRegion(const cGUIDT<gcRegion> &) const;
};

class gcMsgUpdateRegionState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

extern char gcMsgUpdateRegionStatevirtualtable[];

struct gcMsgUpdateRegionState_layout {
    void *vtable;   // 0x00
    char  mB;       // 0x04
    char  _pad[3];
    int   mSubA;    // 0x08
    int   mSubB;    // 0x0C
};

// -----------------------------------------------------------------------------
// 0x00285a10 — gcMsgUpdateRegionState::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgUpdateRegionState::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x10;
    buf.mOffset = cursor;
    gcMsgUpdateRegionState_layout *obj =
        (gcMsgUpdateRegionState_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[4];
        zb[0] = 0;
        zb[1] = 0;
        int v0 = zb[0];
        int v1 = zb[1];
        zb[2] = v0;
        __asm__ volatile(
            "lui $a1, 0x39\n\t"
            "sw %1, 12($sp)\n\t"
            "addiu $a1, $a1, -21312\n\t"
            "ori $a3, $0, 0\n\t"
            "sw $a1, 0(%0)\n\t"
            "sb $a3, 4(%0)"
            :
            : "r"(obj), "r"(v1)
            : "$a1", "$a3", "memory"
        );
        int v2 = zb[2];
        int *p = (int *)((char *)obj + 8);
        int v3 = zb[3];
        p[0] = v2;
        p[1] = v3;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x00136880 — gcMsgUpdateRegionState::Write
// -----------------------------------------------------------------------------

void gcMsgUpdateRegionState::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle conn) const {
    const cGUIDT<gcRegion> *guid = (const cGUIDT<gcRegion> *)((const char *)this + 8);
    ((const cGUID *)guid)->Write(s);
    ((cOutStreamBit *)&s)->WriteBit(*(const unsigned char *)((const char *)this + 4));

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    int localConn = conn.mValue;
    __asm__ volatile("" : "+r"(localConn));
    visitor.mReadActive = 0;
    unsigned char flag = *(const unsigned char *)((const char *)this + 4);
    visitor.mWriteEnabled = true;
    __0fUgcReplicationVisitorMSetNetStreamP6KcOutStream6SnwConnectionHandlebTD(
        &visitor, &s, localConn, flag, true);

    gcRegion *region = (*(gcMap **)0x37D7FC)->FindLoadedRegion(*guid);
    if (region != 0) {
        char *type = *(char **)((char *)region + 4);
        gcTypeDispatchEntry *entry = (gcTypeDispatchEntry *)(type + 0xD0);
        entry->fn((char *)region + entry->offset, &visitor);
    }
}

// -----------------------------------------------------------------------------
// 0x002859c4 — gcMsgUpdateRegionState::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sUpdateRegionStateType;

nwMsgType *gcMsgUpdateRegionState::GetType() const {
    if (!sUpdateRegionStateType) {
        sUpdateRegionStateType = nwMsgType::InitializeType(
            0x10, 0, "gcMsgUpdateRegionState", &gcMsgUpdateRegionState::New);
    }
    return sUpdateRegionStateType;
}
