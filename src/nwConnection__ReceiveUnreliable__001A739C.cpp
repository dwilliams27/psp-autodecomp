// nwConnection::ReceiveUnreliable(nwConnection::nwSendMethod, unsigned int, unsigned char *, int)
// Address: 0x001a739c, Size: 188B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionRReceiveUnreliable65MnwConnectiond__5MnwConnectionMnwSendMethodUiPUci
//
// If the block id is not stale, fold the gap between the last-seen sequence and
// this block id into a pending counter, advance the last-seen sequence, and
// hand the raw bytes to the static DispatchMessages dispatcher.
// Split-TU: class declared locally to avoid touching include/*.h.

typedef int nwSocketHandle;
typedef int nwConnectionHandle;
class nwAddress;

class nwSocket {
public:
    char _pad00[0x10];   // 0x00
    nwSocketHandle mHandle;  // 0x10
};

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    struct nwMsgsBlock {
        char _pad0[0x41C];      // 0x000
        unsigned int mLastSeq;  // 0x41C
        int mPending;           // 0x420
        char _padEnd[0x4];      // 0x424 (element stride 0x428)
    };

    void ReceiveUnreliable(nwSendMethod, unsigned int, unsigned char *, int);
    bool IsBlockIdOld(unsigned int, nwSendMethod) const;
    static void DispatchMessages(unsigned char *, int, nwSocketHandle,
                                 const nwAddress &, nwConnectionHandle, bool);

    nwSocket *mSocket;      // 0x00
    int mHandle;            // 0x04
    char mAddress[0x54];    // 0x08
    nwMsgsBlock mBlocks[3]; // 0x5C
};

void nwConnection::ReceiveUnreliable(nwSendMethod method, unsigned int blockId,
                                     unsigned char *data, int size) {
    if (!IsBlockIdOld(blockId, method)) {
        mBlocks[method].mPending += (int)(((blockId - mBlocks[method].mLastSeq) & 0x3FF) - 1);
        mBlocks[method].mLastSeq = blockId;
        DispatchMessages(data, size, mSocket->mHandle,
                         *(const nwAddress *)&mAddress[0], mHandle, false);
    }
}
