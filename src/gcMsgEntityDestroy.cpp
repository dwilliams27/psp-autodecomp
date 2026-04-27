class cOutStream;
class nwAddress;
class nwMsg;

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
    void Write(cOutStream &) const;
};

inline cObjectKeyRef::cObjectKeyRef() {
    mIdx = 0;
    mField4 = 0;
    mField8 = 0;
}

class gcMsgEntityDestroy {
public:
    void *mVTable;
    int mKey;
    gcMsgEntityDestroy(const cObjectKeyRef &);
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

extern char gcMsgEntityDestroyvirtualtable[];

inline gcMsgEntityDestroy::gcMsgEntityDestroy(const cObjectKeyRef &r) {
    mVTable = gcMsgEntityDestroyvirtualtable;
    mKey = 0;
    (void)r;
}

void gcMsgEntityDestroy::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((const cObjectKeyRef *)((char *)this + 4))->Write(s);
}

nwMsg *gcMsgEntityDestroy::New(nwMsgBuffer &buf) {
    int cur = buf.mOffset + 8;
    buf.mOffset = cur;
    char *obj = (char *)&buf + cur;
    nwMsg *result = 0;
    if (obj != 0) {
        cObjectKeyRef tmp;
        new (obj) gcMsgEntityDestroy(tmp);
        result = (nwMsg *)obj;
    }
    return result;
}

static nwMsgType *sEntityDestroyType;

nwMsgType *gcMsgEntityDestroy::GetType() const {
    if (!sEntityDestroyType) {
        sEntityDestroyType = nwMsgType::InitializeType(
            0x16, 0, "gcMsgEntityDestroy", &gcMsgEntityDestroy::New);
    }
    return sEntityDestroyType;
}
