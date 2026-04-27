// gcMsgUpdateEntityState / gcMsgUpdateEntityStateSimulated — gcAll_psp.obj
//   0x00285488 gcMsgUpdateEntityState::GetType(void) const          (76B)
//   0x002854d4 gcMsgUpdateEntityState::New(nwMsgBuffer &) static    (88B)
//   0x0028552c gcMsgUpdateEntityStateSimulated::GetType(void) const (76B)

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

class gcMsgUpdateEntityState {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

class gcMsgUpdateEntityStateSimulated {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x00285488 — gcMsgUpdateEntityState::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sUpdateEntityStateType;

nwMsgType *gcMsgUpdateEntityState::GetType() const {
    if (!sUpdateEntityStateType) {
        sUpdateEntityStateType = nwMsgType::InitializeType(
            0x11, 1, "gcMsgUpdateEntityState", &gcMsgUpdateEntityState::New);
    }
    return sUpdateEntityStateType;
}

// -----------------------------------------------------------------------------
// 0x002854d4 — gcMsgUpdateEntityState::New(nwMsgBuffer &)
//   12-byte object: vtable @ +0x00 (0x38ABC0), int @ +0x04 (0), byte @ +0x08 (0).
//   Compiler spills three zero ints to sp[0], sp[4], sp[8] interleaved with
//   the stores; emulate via a volatile stack array.
// -----------------------------------------------------------------------------

struct gcMsgUpdateEntityState_layout {
    void *vtable;   // 0x00
    int   mNum;     // 0x04
    char  mFlag;    // 0x08
};

nwMsg *gcMsgUpdateEntityState::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0xC;
    buf.mOffset = cursor;
    gcMsgUpdateEntityState_layout *obj =
        (gcMsgUpdateEntityState_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[3];
        zb[0] = 0;
        int z = zb[0];
        // SNC scheduler keeps lui+addiu for the vtable pointer adjacent unless
        // we materialize the high half separately; this lets sw $a1,8(sp) land
        // between them, matching the original schedule.
        unsigned int vt_hi;
        __asm__ ("lui %0, 0x39" : "=r"(vt_hi));
        zb[2] = z;
        void *vt = (void *)(vt_hi - 21568);
        zb[1] = z;
        obj->vtable = vt;
        int v2 = zb[2];
        int *p = (int *)((char *)obj + 4);
        char zb_byte = 0;
        p[0] = v2;
        *((char *)obj + 8) = zb_byte;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x0028552c — gcMsgUpdateEntityStateSimulated::GetType
//   New() (0x285578) is defined in another TU; declared as static member here.
// -----------------------------------------------------------------------------

static nwMsgType *sUpdateEntityStateSimulatedType;

nwMsgType *gcMsgUpdateEntityStateSimulated::GetType() const {
    if (!sUpdateEntityStateSimulatedType) {
        sUpdateEntityStateSimulatedType = nwMsgType::InitializeType(
            0x17, 1, "gcMsgUpdateEntityStateSimulated",
            &gcMsgUpdateEntityStateSimulated::New);
    }
    return sUpdateEntityStateSimulatedType;
}
