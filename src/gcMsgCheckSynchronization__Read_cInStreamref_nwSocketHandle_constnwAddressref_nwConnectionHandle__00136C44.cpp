// ODR-WARNING: split-TU isolated reconstruction of gcMsgCheckSynchronization::Read.
// 0x00136c44 gcMsgCheckSynchronization::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) (56B)
// Class declared locally to avoid touching shared TUs / headers.
// NOTE: src/gcMsgCheckSynchronization.cpp already contains a Read body for this
// same symbol (line 78) — that is a duplicate-symbol conflict in the shared TU.
// Per hard-isolation rules this file is the standalone reconstruction; the shared
// TU copy is reported, not modified.

class nwAddress;
class cInStream;

class nwSocketHandle {
public:
    int mHandle;
};

class nwConnectionHandle {
public:
    int mHandle;
};

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
};

extern "C" int cRand(void);

class gcMsgCheckSynchronization {
public:
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

void gcMsgCheckSynchronization::Read(cInStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) {
    unsigned int tmp;
    ((cInStreamRef *)&s)->Read(tmp, 0x20, true);
    cRand();
}
