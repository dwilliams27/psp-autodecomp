// gcMsgPlayCinematic / gcMsgLoadMap — message classes from gcAll_psp.obj.
// Decompiled functions:
//   0x002644f4 gcMsgPlayCinematic::GetType(void) const         (76B)
//   0x00264540 gcMsgPlayCinematic::New(nwMsgBuffer &) static   (156B)
//   0x00123e20 gcMsgPlayCinematic::Read(...)                   (460B)
//   0x00264658 gcMsgLoadMap::GetType(void) const               (76B)

class nwMsg;
class cInStream;
class nwAddress;
class gcCinematic;
class gcStreamedCinematic;

struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

struct cGUID {
    int a;
    int b;
    void Read(cInStream &);
};

template <class T>
class cGUIDT : public cGUID {
};

class cHandle {
public:
    int mId;
    void Read(cInStream &);
};

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
    void Read(float &, bool);
};

struct cInStreamBits {
    unsigned char *mBuf;     // 0x0
    int mCapacity;            // 0x4
    int mBitPos;              // 0x8
};

class nwConnection {
public:
    int _pad[10];
    int mField28;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class gcEntity {
public:
    static void OnFullscreenCinematicStarting();
    static void OnFullscreenCinematicStarted();
};

class gcMap {
public:
    static bool IsMapLoaded(bool);
    void QueueStreamedCinematic(const cGUIDT<gcStreamedCinematic> &);
    static void PlayCinematic(gcCinematic *, cHandle, float, bool);
};

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

class gcMsgPlayCinematic {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
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
        int z2 = zb[2];
        zb[0] = 0;
        zb[2] = 0;
        zb[1] = 0;
        zb[3] = 0;
        unsigned int vt_hi;
        __asm__ ("lui %0, 0x39" : "=r"(vt_hi));
        zb[6] = z2;
        void *vt = (void *)(vt_hi - 28720);
        zb[4] = z2;
        obj->vtable = vt;
        int t = 1;
        zb[5] = 0;
        obj->mNum = t;
        int z0 = zb[0];
        zb[7] = 0;
        int *p1 = (int *)((char *)obj + 8);
        t = zb[1];
        p1[0] = z0;
        float zf = 0.0f;
        p1[1] = t;
        int b1 = 0;
        obj->mFlag = 0;
        int b2 = 0;
        *((char *)obj + 0x14) = (char)b1;
        *((char *)obj + 0x15) = (char)b2;
        int z6 = zb[6];
        int *p2 = (int *)((unsigned char *)obj + 0x18);
        p2[0] = z6;
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

// -----------------------------------------------------------------------------
// 0x00123e20 — gcMsgPlayCinematic::Read
//   Reads the play-cinematic message off the wire and dispatches:
//     * decodes a 1-bit fullscreen flag (this->field4) using cInStream::Read
//     * if cleared: only fires gcEntity::OnFullscreenCinematicStarting and
//       returns
//     * otherwise: fires Started, reads the GUID at +0x08, two cHandles
//       at +0x18 and +0x10, two inline ReadBit() calls into bytes
//       this->field14/field15, then a float into +0x1C
//     * if field14 set and the connection is alive: clear high half of the
//       connection's +0x28 field
//     * if the global map is loaded:
//          - field15 set  → QueueStreamedCinematic on the global map
//          - field15 zero → resolve the cHandle@+0x18 against the global
//                           cinematic table at 0x38890 and call PlayCinematic
// -----------------------------------------------------------------------------

void gcMsgPlayCinematic::Read(cInStream &is, nwSocketHandle, const nwAddress &,
                              nwConnectionHandle ch) {
    ((cInStreamRef *)&is)->Read(*(unsigned int *)((char *)this + 4), 1, true);
    if (*(int *)((char *)this + 4) != 0) {
    gcEntity::OnFullscreenCinematicStarted();
    ((cGUID *)((char *)this + 8))->Read(is);
    ((cHandle *)((char *)this + 0x18))->Read(is);
    ((cHandle *)((char *)this + 0x10))->Read(is);

    cInStreamBits *bs = (cInStreamBits *)&is;
    {
        __asm__ volatile("" ::: "memory");
        unsigned char *data = bs->mBuf;
        int pos = bs->mBitPos;
        int *bpPtr = &bs->mBitPos;
        unsigned char *byte = data + (pos >> 3);
        pos = *bpPtr;
        int bit = pos & 7;
        int mask = 1 << bit;
        pos = pos + 1;
        *bpPtr = pos;
        *((char *)this + 0x14) = (*byte & mask) != 0;
    }
    {
        int pos = bs->mBitPos;
        unsigned char *data = bs->mBuf;
        int *bpPtr = &bs->mBitPos;
        unsigned char *byte = data + (pos >> 3);
        pos = *bpPtr;
        int bit = pos & 7;
        pos = pos + 1;
        int mask = 1 << bit;
        __asm__ volatile("" ::: "memory");
        *bpPtr = pos;
        *((char *)this + 0x15) = (*byte & mask) != 0;
    }
    ((cInStreamRef *)&is)->Read(*(float *)((char *)this + 0x1C), true);

    if (*((unsigned char *)this + 0x14) && nwSocket::GetConnection(ch) != 0) {
        nwConnection *c = nwSocket::GetConnection(ch);
        c->mField28 = c->mField28 & 0xFFFF0000;
    }
    if (gcMap::IsMapLoaded(false)) {
        if (*((unsigned char *)this + 0x15)) {
            (*(gcMap **)0x37D7FC)->QueueStreamedCinematic(
                *(cGUIDT<gcStreamedCinematic> *)((char *)this + 8));
        } else {
            int id = *(int *)((char *)this + 0x18);
            gcCinematic *cin = 0;
            if (id != 0) {
                int idx = id & 0xFFFF;
                gcCinematic *cand = ((gcCinematic **)0x38890)[idx];
                if (cand && *(int *)((char *)cand + 0x30) == id) {
                    cin = cand;
                }
            }
            gcMap::PlayCinematic(cin, *(cHandle *)((char *)this + 0x10),
                                 *(float *)((char *)this + 0x1C), false);
        }
    }
    } else {
        gcEntity::OnFullscreenCinematicStarting();
    }
}
