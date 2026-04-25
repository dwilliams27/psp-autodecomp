#ifndef GCMSGREQUESTLOADEDSTATE_H
#define GCMSGREQUESTLOADEDSTATE_H

class cOutStream;
class nwAddress;
#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class gcMsgRequestLoadedState {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

#endif
