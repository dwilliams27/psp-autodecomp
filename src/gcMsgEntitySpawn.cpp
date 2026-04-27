// gcMsgEntitySpawn — gcAll_psp.obj
//   0x001347c4 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const   (176B)
//   0x0028535c GetType(void) const                                                                (76B)
//   0x002853a8 New(nwMsgBuffer &) static                                                          (80B)

class nwMsg;
class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

inline void *operator new(unsigned int, void *p) { return p; }

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cHandle {
public:
    int mIndex;
    cHandle();
    cHandle(const cHandle &);
    void Write(cOutStream &) const;
};

inline cHandle::cHandle() {
    mIndex = 0;
}

inline cHandle::cHandle(const cHandle &other) {
    mIndex = other.mIndex;
}

class cObjectKeyRef {
public:
    int mField0;
    int mField4;
    cHandle mHandle;     // at offset 8
    cObjectKeyRef();
};

inline cObjectKeyRef::cObjectKeyRef() {
    mField0 = 0;
    mHandle.mIndex = mField0;
    mField4 = mField0;
}

class cObjectKey {
public:
    unsigned int mKey;
    cObjectKey(unsigned int);
    void Write(cOutStream &) const;
};

inline cObjectKey::cObjectKey(unsigned int k) {
    mKey = k;
}

class gcMsgEntitySpawn {
public:
    void *mVTable;       // 0x00
    cHandle mHandle;     // 0x04 (single int)
    gcMsgEntitySpawn(const cObjectKeyRef &);
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

extern char gcMsgEntitySpawnvirtualtable[];
extern void *D_00038890[];

inline gcMsgEntitySpawn::gcMsgEntitySpawn(const cObjectKeyRef &r) : mHandle(r.mHandle) {
    mVTable = gcMsgEntitySpawnvirtualtable;
}

// -----------------------------------------------------------------------------
// 0x001347c4 — gcMsgEntitySpawn::Write
//   Resolves the entity referred to by mHandle via the global handle table at
//   D_00038890, then writes the entity's cHandle (offset +0x44) and an
//   xor-derived cObjectKey (offset +0x20 ^ +0x24) to the stream.
// -----------------------------------------------------------------------------

void gcMsgEntitySpawn::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    int h = mHandle.mIndex;
    void *sm;
    if (h == 0) {
        sm = 0;
    } else {
        void *e = D_00038890[h & 0xFFFF];
        void *r = 0;
        if (e != 0) {
            if (*(int *)((char *)e + 0x30) == h) {
                r = e;
            }
        }
        sm = r;
    }
    cHandle hOut = *(const cHandle *)((char *)sm + 0x44);
    hOut.Write(s);
    int key = 0;
    if (sm != 0) {
        key = *(int *)((char *)sm + 0x20) ^ *(int *)((char *)sm + 0x24);
    }
    cObjectKey kOut((unsigned int)key);
    kOut.Write(s);
}

// -----------------------------------------------------------------------------
// 0x002853a8 — gcMsgEntitySpawn::New(nwMsgBuffer &)
//   Allocates 8 bytes from the nwMsgBuffer arena (cursor at +0x4B0) and
//   constructs a gcMsgEntitySpawn at the slot.
// -----------------------------------------------------------------------------

nwMsg *gcMsgEntitySpawn::New(nwMsgBuffer &buf) {
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
        void *vt = (void *)(vt_hi - 21632);
        *(void **)obj = vt;
        int v2 = zb[2];
        int *p = (int *)(obj + 4);
        *p = v2;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x0028535c — gcMsgEntitySpawn::GetType
//   Lazily creates the nwMsgType singleton (id=6, sub=1) for gcMsgEntitySpawn.
// -----------------------------------------------------------------------------

static nwMsgType *sEntitySpawnType;

nwMsgType *gcMsgEntitySpawn::GetType() const {
    if (!sEntitySpawnType) {
        sEntitySpawnType = nwMsgType::InitializeType(
            6, 1, "gcMsgEntitySpawn", &gcMsgEntitySpawn::New);
    }
    return sEntitySpawnType;
}
