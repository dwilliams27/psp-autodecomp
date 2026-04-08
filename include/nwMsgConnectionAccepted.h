#ifndef NWMSGCONNECTIONACCEPTED_H
#define NWMSGCONNECTIONACCEPTED_H

class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class nwMsgConnectionAccepted {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

#endif
