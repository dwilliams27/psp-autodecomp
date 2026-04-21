#ifndef GCMSGCINEMATICENDED_H
#define GCMSGCINEMATICENDED_H

class cOutStream;
class cInStream;
class nwAddress;
class nwMsg;
class nwMsgType;
struct nwMsgBuffer;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class gcMsgCinematicEnded {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

#endif
