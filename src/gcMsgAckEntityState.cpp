// gcMsgAckEntityState — gcAll_psp.obj
//   0x00285740 GetType(void) const                          (76B)
//   0x0028578c New(nwMsgBuffer &) static                    (80B)
// gcMsgUpdatePlayerState — gcAll_psp.obj
//   0x002857dc GetType(void) const                          (76B)
//   (gcMsgUpdatePlayerState::New is decompiled in another TU
//    — gcMsgEntityMoveTo.cpp — declared extern here.)

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class gcMsgAckEntityState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

class gcMsgUpdatePlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x00285740 — gcMsgAckEntityState::GetType
//   Lazily creates the nwMsgType singleton (id=7, sub=1).
// -----------------------------------------------------------------------------

static nwMsgType *sAckEntityStateType;

nwMsgType *gcMsgAckEntityState::GetType() const {
    if (!sAckEntityStateType) {
        sAckEntityStateType = nwMsgType::InitializeType(
            7, 1, "gcMsgAckEntityState", &gcMsgAckEntityState::New);
    }
    return sAckEntityStateType;
}

// -----------------------------------------------------------------------------
// 0x0028578c — gcMsgAckEntityState::New(nwMsgBuffer &)
//   Allocates 8 bytes from the nwMsgBuffer arena (cursor at +0x4B0) and
//   constructs a gcMsgAckEntityState at the slot. Vtable lives at 0x38AC40.
// -----------------------------------------------------------------------------

nwMsg *gcMsgAckEntityState::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 8;
    buf.mOffset = cursor;
    char *obj = (char *)&buf + cursor;
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[3];
        zb[0] = 0;
        int z = zb[0];
        unsigned int vt_hi;
        __asm__ ("lui %0, 0x39" : "=r"(vt_hi));
        zb[2] = z;
        zb[1] = z;
        void *vt = (void *)(vt_hi - 21440);
        *(void **)obj = vt;
        int v2 = zb[2];
        int *p = (int *)(obj + 4);
        *p = v2;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x002857dc — gcMsgUpdatePlayerState::GetType
//   Lazily creates the nwMsgType singleton (id=9, sub=0).
// -----------------------------------------------------------------------------

static nwMsgType *sUpdatePlayerStateType;

nwMsgType *gcMsgUpdatePlayerState::GetType() const {
    if (!sUpdatePlayerStateType) {
        sUpdatePlayerStateType = nwMsgType::InitializeType(
            9, 0, "gcMsgUpdatePlayerState", &gcMsgUpdatePlayerState::New);
    }
    return sUpdatePlayerStateType;
}
