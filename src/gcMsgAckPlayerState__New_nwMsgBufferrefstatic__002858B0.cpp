// gcMsgAckPlayerState::New(nwMsgBuffer &) static @ 0x002858b0
// Symbol: __0fTgcMsgAckPlayerStateDNewR6LnwMsgBufferT
//
// Static message-allocator. Bumps the nwMsgBuffer cursor (mOffset @ 0x4B0)
// by 8, computes the slot pointer at (&buf + mOffset), and if non-null
// installs the gcMsgAckPlayerState vtable and zeroes the following word.
//
// Sibling gcMsgConnected::New (0x00285178, matched) is byte-identical
// except it stores the trailing zero as a byte (sb) where this stores a
// word (sw). Split-TU local class decl so gcMsgConnected.cpp is untouched.

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class gcMsgAckPlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

extern char gcMsgAckPlayerStatevirtualtable[];

struct gcMsgAckPlayerStateObj {
    char *mVTable;   // 0x0
    int   mField;    // 0x4
};

nwMsg *gcMsgAckPlayerState::New(nwMsgBuffer &buf) {
    buf.mOffset += 8;
    gcMsgAckPlayerStateObj *p = (gcMsgAckPlayerStateObj *)((char *)&buf + buf.mOffset);
    nwMsg *result = 0;
    if (p) {
        p->mVTable = gcMsgAckPlayerStatevirtualtable;
        p->mField = 0;
        result = (nwMsg *)p;
    }
    return result;
}
