// gcMsgPlayCinematic / gcMsgLoadMap — message classes from gcAll_psp.obj.
// Decompiled functions:
//   0x002644f4 gcMsgPlayCinematic::GetType(void) const         (76B)
//   0x00264540 gcMsgPlayCinematic::New(nwMsgBuffer &) static   (156B)
//   0x00264658 gcMsgLoadMap::GetType(void) const               (76B)

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

class gcMsgPlayCinematic {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

class gcMsgLoadMap {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x00264540 — gcMsgPlayCinematic::New(nwMsgBuffer &)
//   Allocates a 32-byte object from the nwMsgBuffer arena (cursor at +0x4B0)
//   and runs a constructor that:
//     - sets vtable at +0x00 to 0x388FD0
//     - sets +0x04 = 1
//     - default-initializes 8-byte sub-object at +0x08 and 4-byte at +0x18
//       via stack temporaries (this is what produces the sw/lw round-trip)
//     - zeros bytes at +0x14 and +0x15
//     - zeros +0x10
//     - sets +0x1C = 0.0f
// -----------------------------------------------------------------------------

struct gcMsgPlayCinematic_layout {
    void *vtable;       // 0x00
    int mNum;           // 0x04
    int mSub1a;         // 0x08
    int mSub1b;         // 0x0C
    int mFlag;          // 0x10
    char mB1;           // 0x14
    char mB2;           // 0x15
    char _pad[2];       // 0x16
    int mSub2;          // 0x18
    float mF;           // 0x1C
};

nwMsg *gcMsgPlayCinematic::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x20;
    buf.mOffset = cursor;
    gcMsgPlayCinematic_layout *obj =
        (gcMsgPlayCinematic_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[8];
        zb[0] = 0;
        zb[2] = 0;
        zb[1] = 0;
        int reuse = zb[2];
        zb[3] = 0;
        zb[6] = reuse;
        void *vt = (void *)0x388FD0;
        zb[4] = reuse;
        obj->vtable = vt;
        int one = 1;
        zb[5] = 0;
        obj->mNum = one;
        int *p1 = (int *)((char *)obj + 8);
        zb[7] = 0;
        int *p2 = (int *)((char *)obj + 0x18);
        int v1 = zb[1];
        int v0 = zb[0];
        float zf = 0.0f;
        p1[1] = v1;
        p1[0] = v0;
        obj->mFlag = 0;
        char b1 = 0;
        char b2 = 0;
        *((char *)obj + 0x14) = b1;
        int v6 = zb[6];
        *((char *)obj + 0x15) = b2;
        p2[0] = v6;
        obj->mF = zf;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x002644f4 — gcMsgPlayCinematic::GetType
//   Lazily creates the nwMsgType singleton (id=0x1D) for gcMsgPlayCinematic.
// -----------------------------------------------------------------------------

static nwMsgType *sPlayCinematicType;

nwMsgType *gcMsgPlayCinematic::GetType() const {
    if (!sPlayCinematicType) {
        sPlayCinematicType = nwMsgType::InitializeType(
            0x1D, 0, "gcMsgPlayCinematic", &gcMsgPlayCinematic::New);
    }
    return sPlayCinematicType;
}

// -----------------------------------------------------------------------------
// 0x00264658 — gcMsgLoadMap::GetType
//   Lazily creates the nwMsgType singleton (id=0x0C) for gcMsgLoadMap.
//   gcMsgLoadMap::New (0x002646A4) is decompiled in another TU — declared
//   as an extern static member; the .o relocation will be linker-resolved.
// -----------------------------------------------------------------------------

static nwMsgType *sLoadMapType;

nwMsgType *gcMsgLoadMap::GetType() const {
    if (!sLoadMapType) {
        sLoadMapType = nwMsgType::InitializeType(
            0xC, 0, "gcMsgLoadMap", &gcMsgLoadMap::New);
    }
    return sLoadMapType;
}
