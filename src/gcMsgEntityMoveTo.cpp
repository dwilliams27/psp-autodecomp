class nwMsg;
class cOutStream;
class cInStream;
class nwAddress;
class mOCS;
class gcEntity;
class cTimeValue { public: int data[2]; };
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
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
    void Write(float, int, int, bool);
};

class cInStreamRef {
public:
    void Read(int &, int, bool);
    void Read(unsigned int &, int, bool);
    void Read(float &, int, int, bool);
};

struct cInStreamBits {
    unsigned char *mBuf;     // 0x0
    int mCapacity;           // 0x4
    int mBitPos;             // 0x8
    char _pad0C[6];          // 0xC
    unsigned char mOverflow; // 0x12
};

class cHandleRef {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class mBasisRef {
public:
    void GetHPR(float *, float *, float *) const;
    void SetHPR(float, float, float);
};

class cTimeValueRef {
public:
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class gcEntityRef {
public:
    bool IsLocallyControlled() const;
    void SnapTo(const mOCS &, bool);
    void LerpTo(const gcEntity *, short, cTimeValue, bool);
    void Deliver(const nwMsg &, nwConnectionHandle);
};

class gcStreamedCinematic {
public:
    static bool IsFullscreenInProgress();
};

class gcMap {
public:
    static bool IsMapLoaded(bool);
};

class gcMsgEntityMoveTo {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;       // 0x00
    int mType;           // 0x04
    int mEntityId;       // 0x08  (cHandle: ... 8 bytes)
    char _pad0C[4];      // 0x0C
    char mBasis[0x30];   // 0x10
    float mVel[3];       // 0x40
    char _pad4C[4];      // 0x4C
    int mSyncHandle;     // 0x50
    int mSeq;            // 0x54
    char mTime[4];       // 0x58
    unsigned char mFlag; // 0x5C
};

class gcMsgUpdatePlayerState {
public:
    static nwMsg *New(nwMsgBuffer &);
    void *mVTable;       // 0x00
    int mUnk4;           // 0x04
    unsigned char mFlag; // 0x08
};

extern char gcMsgEntityMoveTovirtualtable[];
extern char gcMsgUpdatePlayerStatevirtualtable[];
extern gcEntity *gEntityTable[];
extern int gGameFlags;

static nwMsgType *sEntityMoveToType;

nwMsg *gcMsgEntityMoveTo::New(nwMsgBuffer &buf) {
    buf.mOffset += 0x60;
    int *p = (int *)((char *)&buf + buf.mOffset);
    nwMsg *result = 0;
    if (p) {
        p[0] = (int)gcMsgEntityMoveTovirtualtable;
        p[2] = 0;
        p[20] = 0;
        p[22] = 0;
        result = (nwMsg *)p;
    }
    return result;
}

nwMsgType *gcMsgEntityMoveTo::GetType() const {
    if (!sEntityMoveToType) {
        sEntityMoveToType = nwMsgType::InitializeType(0x19, 0, "gcMsgEntityMoveTo", &gcMsgEntityMoveTo::New);
    }
    return sEntityMoveToType;
}

void gcMsgEntityMoveTo::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cHandleRef *)((char *)this + 8))->Write(s);
    ((cOutStreamRef *)&s)->Write((unsigned int)mType, 2, true);
    if (mType == 0) {
        float h, p, r;
        ((const mBasisRef *)((char *)this + 0x10))->GetHPR(&h, &p, &r);
        float hpr[4];
        hpr[0] = h;
        hpr[1] = p;
        hpr[2] = r;
        // Loop 1: write velocity (3 floats) at offset 0x40
        const float *v = (const float *)((char *)this + 0x40);
        for (int i = 0; i < 3; i++) {
            ((cOutStreamRef *)&s)->Write(v[i], 0xE, 0xB, false);
        }
        // Loop 2: write HPR (3 floats)
        for (int i = 0; i < 3; i++) {
            ((cOutStreamRef *)&s)->Write(hpr[i], 9, 2, true);
        }
    } else if (mType == 1) {
        ((cHandleRef *)((char *)this + 0x50))->Write(s);
        ((cOutStreamRef *)&s)->Write(mSeq, 0xA, true);
        ((cTimeValueRef *)((char *)this + 0x58))->Write(s);
        unsigned char flag = mFlag;
        cInStreamBits *bs = (cInStreamBits *)&s;
        int pos = bs->mBitPos;
        unsigned char *byte = bs->mBuf + (pos >> 3);
        int bit = pos & 7;
        bs->mBitPos = pos + 1;
        if (!bs->mOverflow) {
            if (bs->mCapacity < ((bs->mBitPos + 7) >> 3)) {
                bs->mOverflow = 1;
            }
        }
        if (bs->mOverflow == 0) {
            unsigned char b = *byte;
            *byte = (b & ~(1 << bit)) | ((flag != 0) << bit);
        }
    }
}

void gcMsgEntityMoveTo::Read(cInStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle ch) {
    cInStreamBits *bs = (cInStreamBits *)&s;
    if (!gcMap::IsMapLoaded(false) || gcStreamedCinematic::IsFullscreenInProgress()) {
        bs->mBitPos = bs->mCapacity * 8;
        return;
    }
    ((cHandleRef *)((char *)this + 8))->Read(s);
    ((cInStreamRef *)&s)->Read(*(unsigned int *)((char *)this + 4), 2, true);
    int id = mEntityId;
    gcEntity *ent = 0;
    if (id != 0) {
        gcEntity *cand = gEntityTable[id & 0xFFFF];
        if (cand && *(int *)((char *)cand + 0x30) == id) {
            ent = cand;
        }
    }
    if (!ent) {
        bs->mBitPos = bs->mCapacity * 8;
        return;
    }
    int type = mType;
    if (type == 0) {
        float *v = (float *)((char *)this + 0x40);
        float hpr[4];
        for (int i = 0; i < 3; i++) {
            ((cInStreamRef *)&s)->Read((float &)v[i], 0xE, 0xB, false);
        }
        for (int i = 0; i < 3; i++) {
            ((cInStreamRef *)&s)->Read((float &)hpr[i], 9, 2, true);
        }
        ((mBasisRef *)((char *)this + 0x10))->SetHPR(hpr[0], hpr[1], hpr[2]);
        if (((gcEntityRef *)ent)->IsLocallyControlled()) {
            ((gcEntityRef *)ent)->SnapTo(*(const mOCS *)((char *)this + 0x10), true);
        }
    } else if (type == 1) {
        ((cHandleRef *)((char *)this + 0x50))->Read(s);
        ((cInStreamRef *)&s)->Read(*(int *)((char *)this + 0x54), 0xA, true);
        ((cTimeValueRef *)((char *)this + 0x58))->Read(s);
        int pos = bs->mBitPos;
        unsigned char *byte = bs->mBuf + (pos >> 3);
        int bit = pos & 7;
        bs->mBitPos = pos + 1;
        mFlag = (*byte & (1 << bit)) != 0;
        if (((gcEntityRef *)ent)->IsLocallyControlled()) {
            int id2 = mSyncHandle;
            gcEntity *other = 0;
            if (id2 != 0) {
                gcEntity *cand = gEntityTable[id2 & 0xFFFF];
                if (cand && *(int *)((char *)cand + 0x30) == id2) {
                    other = cand;
                }
            }
            if (other) {
                gcEntity *peer = 0;
                if (id2 != 0) {
                    gcEntity *cand = gEntityTable[id2 & 0xFFFF];
                    if (cand && *(int *)((char *)cand + 0x30) == id2) {
                        peer = cand;
                    }
                }
                ((gcEntityRef *)ent)->LerpTo(peer, (short)mSeq, *(cTimeValue *)((char *)this + 0x58), mFlag);
            }
        }
    }
    int flags = gGameFlags;
    bool deliver = false;
    if (flags & 1) {
        if (flags & 4) deliver = true;
    } else {
        deliver = true;
    }
    if (deliver) {
        ((gcEntityRef *)ent)->Deliver(*(const nwMsg *)this, ch);
    }
}

// -------- gcMsgUpdatePlayerState::New --------
nwMsg *gcMsgUpdatePlayerState::New(nwMsgBuffer &buf) {
    buf.mOffset += 0xC;
    int *p = (int *)((char *)&buf + buf.mOffset);
    nwMsg *result = 0;
    if (p) {
        unsigned char z = 0;
        p[0] = (int)gcMsgUpdatePlayerStatevirtualtable;
        p[1] = 0;
        *(unsigned char *)((char *)p + 8) = z;
        result = (nwMsg *)p;
    }
    return result;
}
