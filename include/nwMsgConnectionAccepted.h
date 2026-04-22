#ifndef NWMSGCONNECTIONACCEPTED_H
#define NWMSGCONNECTIONACCEPTED_H

class cOutStream;
class cInStream;
class nwAddress;
class nwMsg;
class nwMsgBuffer;
class nwMsgType;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class nwMsgConnectionAccepted {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
    nwMsgType *GetType() const;
    static nwMsg *New(nwMsgBuffer &);
};

#endif
