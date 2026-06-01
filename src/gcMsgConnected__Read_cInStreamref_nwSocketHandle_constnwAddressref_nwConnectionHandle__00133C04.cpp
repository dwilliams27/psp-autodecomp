// ODR-WARNING: split-TU isolated reconstruction of gcMsgConnected::Read.
// 0x00133c04 gcMsgConnected::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) (80B)
// Class declared locally to avoid touching shared TUs / headers.

class cInStream { public: void Read(int &, int, bool); };
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class gcMsgConnected {
public:
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    void *mVTable;       // 0x00
    signed char mField;  // 0x04
};

void gcMsgConnected::Read(cInStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) {
    (void)sock;
    (void)conn;
    int value;
    bool flag = true;
    stream.Read(value, 8, flag);
    mField = (signed char)value;
    *(int *)0x37D85C = mField;
}
