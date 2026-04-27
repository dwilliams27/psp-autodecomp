// gcMsgUpdateRegionState — gcAll_psp.obj
//   0x002859c4 GetType(void) const               (76B)
//   0x00285a10 New(nwMsgBuffer &) static         (104B)
//
// Layout (0x10 bytes):
//   0x00 vtable
//   0x04 char    mB
//   0x08 8 bytes default-initialised via stack temp

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class gcMsgUpdateRegionState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
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
        zb[3] = v1;
        void *vt = (void *)0x38ACC0;
        char b = 0;
        obj->vtable = vt;
        obj->mB = b;
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
