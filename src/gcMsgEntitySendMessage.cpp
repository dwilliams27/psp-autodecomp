// gcMsgEntitySendMessage — gcAll_psp.obj
//   0x001342b4 Write(cOutStream &, ...) const           (92B)
//   0x00285234 GetType(void) const                       (76B)
//   0x00285280 New(nwMsgBuffer &) static                 (220B)

class nwMsg;
class cOutStream;
class nwAddress;
class gcEnumeration;
class gcEnumerationEntry;
class gcEventParams;
class gcEventStackData;
template <class A, class B> class cHandlePairT;
template <class A> class cSubHandleT;

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
    void Write(unsigned int, int, bool);
};

class cHandle {
public:
    void Write(cOutStream &) const;
};

class gcMsgEntitySendMessage {
public:
    void *mVTable;          // 0x00
    int   mHandle[2];       // 0x04 (cHandle, 8 bytes)
    char  mEventStack[0x80]; // 0x0C..0x8B (gcEventStackData starting at 0x14)
    // 0x10 has int written second (m_unk10)
    // 0x14 starts gcEventStackData (size 0x68 = up through 0x7B)
    // 0x7C starts gcEventParams (size ?)
    // Total class size implied by New: 0x98

    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    static void WriteEventData(cOutStream &,
                               const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &,
                               const gcEventParams &,
                               const gcEventStackData &);
};

// -----------------------------------------------------------------------------
// 0x00285234 — gcMsgEntitySendMessage::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sEntitySendMessageType;

nwMsgType *gcMsgEntitySendMessage::GetType() const {
    if (!sEntitySendMessageType) {
        sEntitySendMessageType = nwMsgType::InitializeType(
            0x18, 0, "gcMsgEntitySendMessage", &gcMsgEntitySendMessage::New);
    }
    return sEntitySendMessageType;
}

// -----------------------------------------------------------------------------
// 0x001342b4 — gcMsgEntitySendMessage::Write
// -----------------------------------------------------------------------------

void gcMsgEntitySendMessage::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((const cHandle *)((const char *)this + 4))->Write(s);
    ((cOutStreamRef *)&s)->Write(*(unsigned int *)((const char *)this + 0x10), 2, true);
    gcMsgEntitySendMessage::WriteEventData(
        s,
        *(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > *)((const char *)this + 8),
        *(const gcEventParams *)((const char *)this + 0x7C),
        *(const gcEventStackData *)((const char *)this + 0x14));
}

// -----------------------------------------------------------------------------
// 0x00285280 — gcMsgEntitySendMessage::New(nwMsgBuffer &)
//   Allocates 0x98 bytes from the nwMsgBuffer arena and constructs the object.
//   Calls __vec_new at +0x6C for an array of 2 elements of size 8 (ctor 0x2275F0).
// -----------------------------------------------------------------------------

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void gcMsgEntitySendMessage_ctor_2275F0(void *);

nwMsg *gcMsgEntitySendMessage::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x98;
    buf.mOffset = cursor;
    int *p = (int *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (p != 0) {
        float zf = 0.0f;
        float *fp = (float *)p;
        p[0] = 0x38AB60;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[5] = 0;
        fp[6] = zf;
        p[20] = 0;
        __vec_new((char *)p + 0x6C, 2, 8, &gcMsgEntitySendMessage_ctor_2275F0);
        fp[7] = zf;
        fp[8] = zf;
        fp[9] = zf;
        fp[10] = zf;
        fp[11] = zf;
        fp[12] = zf;
        fp[13] = zf;
        fp[14] = zf;
        fp[15] = zf;
        p[21] = 0;
        p[22] = 0;
        p[23] = 0;
        p[24] = 0;
        p[25] = 0;
        p[26] = 0;
        p[35] = 0;
        p[36] = 0;
        *((unsigned char *)p + 0x94) = 1;
        p[31] = 0;
        p[32] = 0;
        p[33] = 0;
        p[34] = 0;
        result = (nwMsg *)p;
    }
    return result;
}
