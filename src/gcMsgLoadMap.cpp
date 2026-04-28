// gcMsgLoadMap — gcAll_psp.obj
//   0x002646a4 New(nwMsgBuffer &) static                       (108B)
//   0x0012418c Write(cOutStream &, ..., nwConnectionHandle) const (128B)
//   GetType is decompiled in src/gcMsgPlayCinematic.cpp (already matched).
//
// Layout (0x28 bytes):
//   0x00 vtable (= 0x389010)
//   0x04 cGUID  mGuid1   (8 bytes, init zero)
//   0x0C uint   mU       (NOT initialized by ctor; written out in Write)
//   0x10 cGUID  mGuid2   (8 bytes, init zero)
//   0x18 cGUID  mItems[2]  (16 bytes, ctored by __vec_new w/ ctor 0x245578)

class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));

struct cGUID {
    int a;
    int b;
    void Write(cOutStream &) const;
};

class cOutStreamRef {
public:
    void Write(unsigned int, int, bool);
};

class gcMsgLoadMap {
public:
    void *       mVTable;     // 0x00
    cGUID        mGuid1;      // 0x04
    unsigned int mU;          // 0x0C
    cGUID        mGuid2;      // 0x10
    cGUID        mItems[2];   // 0x18

    gcMsgLoadMap() {
        mVTable  = (void *)0x389010;
        mGuid1.a = 0;
        mGuid1.b = 0;
        mGuid2.a = 0;
        mGuid2.b = 0;
        __vec_new(&mItems[0], 2, 8, (void (*)(void *))0x245578);
    }

    static nwMsg *New(nwMsgBuffer &);
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

inline void *operator new(unsigned int, void *p) { return p; }

// -----------------------------------------------------------------------------
// 0x002646a4 — gcMsgLoadMap::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgLoadMap::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x28;
    buf.mOffset = cursor;
    gcMsgLoadMap *obj = (gcMsgLoadMap *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        new (obj) gcMsgLoadMap();
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x0012418c — gcMsgLoadMap::Write
// -----------------------------------------------------------------------------

void gcMsgLoadMap::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    mGuid1.Write(s);
    ((cOutStreamRef *)&s)->Write(mU, 0x20, true);
    mGuid2.Write(s);
    int i = 0;
    const cGUID *p = &mItems[0];
    do {
        p->Write(s);
        i++;
        p++;
    } while (i < 2);
}
