#ifndef GCMSGCINEMATICENDED_H
#define GCMSGCINEMATICENDED_H

class cOutStream;
class cInStream;
class nwAddress;
class nwMsg;
class nwMsgType;
struct nwMsgBuffer;
#ifndef NW_HANDLE_TYPES_DEFINED
#define NW_HANDLE_TYPES_DEFINED
struct nwSocketHandle { int mValue; };
struct nwConnectionHandle { int mValue; };
#endif

class gcMsgCinematicEnded {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

#endif
