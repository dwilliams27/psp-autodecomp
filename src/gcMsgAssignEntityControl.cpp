// gcMsgAssignEntityControl — gcAll_psp.obj
//   0x00135558 Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const  (108B)
//   0x002855e4 GetType(void) const                                                               (76B)
//   0x00285630 New(nwMsgBuffer &) static                                                         (80B)

class nwMsg;
class nwAddress;

class cOutStream {
public:
    void Write(int, int, bool);
};

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

class cObjectKeyRef {
public:
    int mIdx;
    int mField4;
    int mField8;
    cObjectKeyRef();
};

inline cObjectKeyRef::cObjectKeyRef() {
    mIdx = 0;
    mField8 = mIdx;
    mField4 = mIdx;
}

class cHandle {
public:
    int mIndex;
    void Write(cOutStream &) const;
};

class gcMsgAssignEntityControl {
public:
    void *mVTable;       // 0x00
    cHandle mHandle;     // 0x04
    gcMsgAssignEntityControl(const cObjectKeyRef &);
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

extern char gcMsgAssignEntityControlvirtualtable[];
extern void *D_00038890[];

inline gcMsgAssignEntityControl::gcMsgAssignEntityControl(const cObjectKeyRef &r) {
    mVTable = gcMsgAssignEntityControlvirtualtable;
    mHandle.mIndex = r.mField8;
}

// -----------------------------------------------------------------------------
// 0x00135558 — gcMsgAssignEntityControl::Write
//   Writes the object's cHandle (offset +0x04), then resolves the handle via
//   the global handle table at D_00038890 and writes a single signed byte at
//   offset +0x68 of the resolved entry as 8 bits with sign flag set.
// -----------------------------------------------------------------------------

void gcMsgAssignEntityControl::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    mHandle.Write(s);
    void *e = 0;
    int h = mHandle.mIndex;
    if (h != 0) {
        e = D_00038890[h & 0xFFFF];
    }
    s.Write(*(signed char *)((char *)e + 0x68), 8, 1);
}

// -----------------------------------------------------------------------------
// 0x002855e4 — gcMsgAssignEntityControl::GetType
//   Lazily creates the nwMsgType singleton (id=0x14, sub=0).
// -----------------------------------------------------------------------------

static nwMsgType *sAssignEntityControlType;

nwMsgType *gcMsgAssignEntityControl::GetType() const {
    if (!sAssignEntityControlType) {
        sAssignEntityControlType = nwMsgType::InitializeType(
            0x14, 0, "gcMsgAssignEntityControl",
            &gcMsgAssignEntityControl::New);
    }
    return sAssignEntityControlType;
}

// -----------------------------------------------------------------------------
// 0x00285630 — gcMsgAssignEntityControl::New(nwMsgBuffer &)
//   Allocates 8 bytes from the nwMsgBuffer arena (cursor at +0x4B0) and
//   constructs a gcMsgAssignEntityControl at the slot using a default
//   cObjectKeyRef temporary.
// -----------------------------------------------------------------------------

nwMsg *gcMsgAssignEntityControl::New(nwMsgBuffer &buf) {
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
        void *vt = (void *)(vt_hi - 21504);
        *(void **)obj = vt;
        int v2 = zb[2];
        int *p = (int *)(obj + 4);
        *p = v2;
        result = (nwMsg *)obj;
    }
    return result;
}
