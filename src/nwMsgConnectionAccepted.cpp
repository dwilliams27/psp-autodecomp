#include "nwMsgConnectionAccepted.h"

// ============================================================
// Function 1: nwMsgConnectionAccepted::Read @ 0x001a8e00
// ============================================================
struct nwConnectionStub {
    void OnConnectionAccepted();
};

class nwSocket {
public:
    static nwConnectionStub *GetConnection(nwConnectionHandle);
};

void nwMsgConnectionAccepted::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
}

void nwMsgConnectionAccepted::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle handle) {
    volatile nwConnectionHandle vols[2];
    nwConnectionHandle h = vols[1];
    vols[1] = handle;
    if (nwSocket::GetConnection(h) != 0) {
        nwSocket::GetConnection(h)->OnConnectionAccepted();
    }
}

// ============================================================
// Function 2: nwMsgConnectionAccepted::GetType @ 0x0036b944
// ============================================================
class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));
};

static nwMsgType *sConnectionAcceptedType;

nwMsgType *nwMsgConnectionAccepted::GetType() const {
    if (!sConnectionAcceptedType) {
        sConnectionAcceptedType = nwMsgType::InitializeType(1, 0, "nwMsgConnectionAccepted", &nwMsgConnectionAccepted::New);
    }
    return sConnectionAcceptedType;
}

// ============================================================
// Function 3: cListSubscriber::GetName @ 0x001c66a0
// ============================================================
extern "C" char *cStrCopy(char *, const char *);

class cBase;

struct cListSubscriberCastInfo {
    short mOffset;                       // +0
    short _pad;                          // +2
    cBase *(*mCast)(cBase *);            // +4
};

struct cListSubscriberTypeInfo {
    char _pad[8];                        // +0
    cListSubscriberCastInfo mCast;       // +8
};

struct cBaseWithName {
    char _pad[12];
    const char *mName;                   // +12
};

class cListSubscriber {
public:
    void *mOwner;                        // +0
    cListSubscriberTypeInfo *mTypeInfo;  // +4
    void GetName(char *) const;
};

void cListSubscriber::GetName(char *dst) const {
    cListSubscriberCastInfo *ci = &mTypeInfo->mCast;
    cBase *obj = ci->mCast((cBase *)((char *)this + ci->mOffset));
    cStrCopy(dst, ((cBaseWithName *)obj)->mName);
}

// ============================================================
// Function 4: gcBoolSet::AssignCopy @ 0x00239d88
// Function 5: gcFloatSet::AssignCopy @ 0x0024932c
// ============================================================
class cBase {
public:
    void *_parent;
    void *_vtable;
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class gcNamedSetName {
public:
    int mIndex;
};

class gcNamedSet : public cBase {
public:
    gcNamedSetName mName;                // +8
};

class gcBoolSet : public gcNamedSet {
public:
    cArrayBase<bool> mArray;             // +12
    void AssignCopy(const cBase *);
};

class gcFloatSet : public gcNamedSet {
public:
    cArrayBase<float> mArray;            // +12
    void AssignCopy(const cBase *);
};

template <class T> T dcast(const cBase *);
template <> gcBoolSet *dcast<gcBoolSet *>(const cBase *);
template <> gcFloatSet *dcast<gcFloatSet *>(const cBase *);

void gcBoolSet::AssignCopy(const cBase *base) {
    gcBoolSet *src = dcast<gcBoolSet *>(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}

void gcFloatSet::AssignCopy(const cBase *base) {
    gcFloatSet *src = dcast<gcFloatSet *>(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}
