// gcMsgAssignPlayerId — gcAll_psp.obj
//   0x00133940 Write(cOutStream &, ...) const     (212B)
//   0x00285064 GetType(void) const                (76B)
//   0x002850b0 New(nwMsgBuffer &) static          (124B)
//
// Layout (0x24 bytes):
//   0x00 vtable        (= 0x38AB00)
//   0x04 int  mNum1    (init -1)
//   0x08 int  mNum2    (init -1)
//   0x0C int  mField12 (init 0 via stack temp)
//   0x10 char mName[0x14]  (cStrCopy from rodata @0x36D944)

class nwMsg;
class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
};

class cHandle {
public:
    void Write(cOutStream &) const;
};

extern "C" int  cStrLength(const char *);
extern "C" void cStrCopy(char *, const char *);

class gcMsgAssignPlayerId {
public:
    void *mVTable;     // 0x00
    int   mNum1;       // 0x04
    int   mNum2;       // 0x08
    int   mField12;    // 0x0C
    char  mName[0x14]; // 0x10

    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

// -----------------------------------------------------------------------------
// 0x00285064 — gcMsgAssignPlayerId::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sAssignPlayerIdType;

nwMsgType *gcMsgAssignPlayerId::GetType() const {
    if (!sAssignPlayerIdType) {
        sAssignPlayerIdType = nwMsgType::InitializeType(
            0xB, 0, "gcMsgAssignPlayerId", &gcMsgAssignPlayerId::New);
    }
    return sAssignPlayerIdType;
}

// -----------------------------------------------------------------------------
// 0x002850b0 — gcMsgAssignPlayerId::New(nwMsgBuffer &)
//   Writes vtable at +0x00, -1 at +0x04 and +0x08, 0 at +0x0C, then cStrCopy
//   into +0x10. The +0x0C store goes through a stack temp (default-constructed
//   sub-object).
// -----------------------------------------------------------------------------

nwMsg *gcMsgAssignPlayerId::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x24;
    buf.mOffset = cursor;
    gcMsgAssignPlayerId *obj = (gcMsgAssignPlayerId *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[3];
        zb[0] = 0;
        int reuse = zb[0];
        unsigned int vt_hi;
        __asm__ ("lui %0, 0x39" : "=r"(vt_hi));
        zb[2] = reuse;
        zb[1] = reuse;
        void *vt = (void *)(vt_hi - 21760);
        int neg1 = -1;
        obj->mVTable = vt;
        obj->mNum1 = neg1;
        obj->mNum2 = neg1;
        int  v = zb[2];
        int *p = (int *)((char *)obj + 12);
        *p = v;
        unsigned int str_hi;
        __asm__ ("lui %0, 0x37" : "=r"(str_hi));
        cStrCopy(&obj->mName[0], (const char *)(str_hi - 9916));
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x00133940 — gcMsgAssignPlayerId::Write
// -----------------------------------------------------------------------------

void gcMsgAssignPlayerId::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cOutStreamRef *)&s)->Write(mNum1, 5, true);
    ((cOutStreamRef *)&s)->Write(mNum2, 5, true);
    ((cHandle *)((char *)this + 12))->Write(s);
    int len = cStrLength(&mName[0]) & 0xFFFF;
    {
        unsigned int len_arg = (unsigned int)(len & 0xFFFF);
        bool sign2 = true;
        ((cOutStreamRef *)&s)->Write(len_arg, 0x10, sign2);
    }
    for (int i = 0; i < len; i++) {
        bool sign = false;
        signed char v = mName[i];
        int iv = v;
        ((cOutStreamRef *)&s)->Write((signed char)iv, 8, sign);
    }
}
