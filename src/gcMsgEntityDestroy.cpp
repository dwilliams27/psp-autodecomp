class cOutStream;
class nwAddress;

#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class cObjectKeyRef {
public:
    void Write(cOutStream &) const;
};

class gcMsgEntityDestroy {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

void gcMsgEntityDestroy::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((const cObjectKeyRef *)((char *)this + 4))->Write(s);
}
