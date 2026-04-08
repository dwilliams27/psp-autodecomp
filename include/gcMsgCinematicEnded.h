#ifndef GCMSGCINEMATICENDED_H
#define GCMSGCINEMATICENDED_H

class cOutStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

class gcMsgCinematicEnded {
public:
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
};

#endif
