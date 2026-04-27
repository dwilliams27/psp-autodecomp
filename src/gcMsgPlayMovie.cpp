// gcMsgPlayMovie — gcAll_psp.obj
//   0x002858e4  GetType(void) const                                                                (76B)
//   0x00285930  New(nwMsgBuffer &) static                                                          (148B)
//   0x0013661c  Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const   (252B)

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
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class cName {
public:
    void Write(cOutStream &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cOutStream &) const;
};

class cTimeValueRef {
public:
    void Write(cOutStream &) const;
};

class cOutStreamFloat {
public:
    void Write(float, bool);
};

// Inline single-bit writer (matches the inlined cOutStream::Write(bool) pattern).
class cOutStreamBit {
public:
    char *mBuf;          // 0x00
    int   mCapacity;     // 0x04
    int   mBitOffset;    // 0x08
    char  _pad0C[6];     // 0x0C..0x11
    unsigned char mOverflow;  // 0x12

    void WriteBit(unsigned char v) {
        int pos     = mBitOffset;
        int bit     = pos & 7;
        char *buf   = mBuf;
        int byteIdx = pos >> 3;
        int newPos  = pos + 1;
        unsigned char *p = (unsigned char *)(buf + byteIdx);
        unsigned char ovf = mOverflow;
        unsigned char vb  = v;
        mBitOffset = newPos;
        if (ovf == 0) {
            if (mCapacity < ((mBitOffset + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        unsigned char ck = (mOverflow == 0);
        if ((unsigned char)(ck & 0xFF)) {
            unsigned char cur = *p;
            int mask = ~(1 << bit);
            int bv   = (vb != 0) ? 1 : 0;
            *p = (unsigned char)((cur & mask) | (bv << bit));
        }
    }
};

struct gcMsgPlayMovie_layout {
    void *vtable;            // 0x00
    char  name[20];          // 0x04..0x17
    union {
        struct {
            short nameH1;        // 0x18
            unsigned short nameH2; // 0x1A
        };
        int nameWord;            // 0x18 (alias)
    };
    unsigned char flag1C;    // 0x1C
    char  pad1D[3];          // 0x1D..0x1F
    int   field20;           // 0x20
    int   field24;           // 0x24
    float float28;           // 0x28  (NOT zeroed by ::New — set externally)
    int   time2C;            // 0x2C  (cTimeValue, 4B)
    int   field30;           // 0x30
};

class gcMsgPlayMovie {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

extern char gcMsgPlayMovievirtualtable[];
extern const char gcMsgPlayMovie_typename[];

// -----------------------------------------------------------------------------
// 0x00285930 — gcMsgPlayMovie::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgPlayMovie::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x34;
    buf.mOffset = cursor;
    gcMsgPlayMovie_layout *obj =
        (gcMsgPlayMovie_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        struct LocalScratch {
            char b[20];
            union {
                struct {
                    short h1;
                    unsigned short h2;
                };
                int as_int;
            };
        };
        LocalScratch s;
        s.h1 = 0;
        s.h2 = 0;
        s.b[0] = 0;
        obj->vtable = gcMsgPlayMovievirtualtable;
        for (int i = 0; i < 20; i++) {
            obj->name[i] = s.b[i];
        }
        short v_h1 = s.h1;
        unsigned short v_h2 = s.h2;
        obj->nameH1 = v_h1;
        int v_w = s.as_int;
        obj->nameH2 = v_h2;
        unsigned char zero_byte = 0;
        obj->nameWord = v_w;
        obj->flag1C = zero_byte;
        obj->field20 = 0;
        obj->field24 = 0;
        obj->time2C = 0;
        obj->field30 = 0;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x002858e4 — gcMsgPlayMovie::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sPlayMovieType;

nwMsgType *gcMsgPlayMovie::GetType() const {
    if (!sPlayMovieType) {
        sPlayMovieType = nwMsgType::InitializeType(
            5, 0, gcMsgPlayMovie_typename, &gcMsgPlayMovie::New);
    }
    return sPlayMovieType;
}

// -----------------------------------------------------------------------------
// 0x0013661c — gcMsgPlayMovie::Write
// -----------------------------------------------------------------------------

void gcMsgPlayMovie::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((const cName *)((const char *)this + 4))->Write(s);
    ((cOutStreamBit *)&s)->WriteBit(*((const unsigned char *)this + 0x1C));
    ((const cHandle *)((const char *)this + 0x20))->Write(s);
    ((const cHandle *)((const char *)this + 0x24))->Write(s);
    ((cOutStreamFloat *)&s)->Write(*(const float *)((const char *)this + 0x28), true);
    ((const cTimeValueRef *)((const char *)this + 0x2C))->Write(s);
    ((const cHandle *)((const char *)this + 0x30))->Write(s);
}
