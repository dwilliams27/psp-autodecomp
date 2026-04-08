#ifndef GCMSGREQUESTLOADEDSTATE_H
#define GCMSGREQUESTLOADEDSTATE_H

class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class gcMsgRequestLoadedState {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

#endif
