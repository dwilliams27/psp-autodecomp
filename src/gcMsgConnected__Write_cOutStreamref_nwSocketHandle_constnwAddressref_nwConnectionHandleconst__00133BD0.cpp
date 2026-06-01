// ODR-WARNING: split-TU isolated reconstruction of gcMsgConnected::Write.
// 0x00133bd0 gcMsgConnected::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const (52B)
// Class declared locally to avoid touching shared TUs / headers.

class cOutStream { public: void Write(int, int, bool); };
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class gcMsgConnected {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;

    void *mVTable;       // 0x00
    signed char mField;  // 0x04
};

void gcMsgConnected::Write(cOutStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) const {
    (void)sock;
    (void)conn;
    bool sign = true;
    signed char v = mField;
    int iv = v;
    stream.Write((signed char)iv, 8, sign);
}
