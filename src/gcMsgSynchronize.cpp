class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));
};

class cOutStreamRef {
public:
    void Write(unsigned int, int, bool);
};

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
};

extern "C" void cGetRandomSeed(unsigned int *, unsigned int *);
extern "C" void cSetRandomSeed(unsigned int, unsigned int);

class gcMsgSynchronize {
public:
    void *mVTable;              // 0x00
    unsigned int mSeed1;        // 0x04
    unsigned int mSeed2;        // 0x08

    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

extern char gcMsgSynchronizevirtualtable[];

static nwMsgType *sSynchronizeType;

nwMsg *gcMsgSynchronize::New(nwMsgBuffer &buf) {
    buf.mOffset += 12;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        ((int *)p)[0] = (int)gcMsgSynchronizevirtualtable;
        ((int *)p)[1] = 0;
        ((int *)p)[2] = 0;
        result = (nwMsg *)p;
    }
    return result;
}

nwMsgType *gcMsgSynchronize::GetType() const {
    if (!sSynchronizeType) {
        sSynchronizeType = nwMsgType::InitializeType(0x22, 0, "gcMsgSynchronize", &gcMsgSynchronize::New);
    }
    return sSynchronizeType;
}

void gcMsgSynchronize::Write(cOutStream &s, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) const {
    volatile nwSocketHandle vol_sock = sock;
    volatile nwConnectionHandle vol_conn = conn;
    unsigned int seed1;
    unsigned int seed2;
    (void)vol_sock; (void)vol_conn;
    cGetRandomSeed(&seed1, &seed2);
    ((cOutStreamRef *)&s)->Write(seed1, 0x20, true);
    ((cOutStreamRef *)&s)->Write(seed2, 0x20, true);
}

void gcMsgSynchronize::Read(cInStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) {
    ((cInStreamRef *)&s)->Read(mSeed1, 0x20, true);
    ((cInStreamRef *)&s)->Read(mSeed2, 0x20, true);
    cSetRandomSeed(mSeed1, mSeed2);
}

// cCopyMemory16 — cAll_psp.obj — VFPU 16-byte chunk copy
typedef int cCopyMemory16_v4sf_t __attribute__((mode(V4SF)));

void cCopyMemory16(void *dst, const void *src, int size) {
    cCopyMemory16_v4sf_t *d = (cCopyMemory16_v4sf_t *)dst;
    const cCopyMemory16_v4sf_t *s = (const cCopyMemory16_v4sf_t *)src;
    char *end = (char *)dst + size;
    if ((char *)d < end) {
        do {
            *d++ = *s++;
        } while ((char *)d < end);
    }
}
