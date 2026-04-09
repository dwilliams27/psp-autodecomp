#include "gcGame.h"

struct nwSocketHandleStruct {
    int handle;
};

class nwNetwork {
public:
    static nwSocketHandleStruct GetGameSocket(void);
};

class nwSocket {
public:
    static nwSocket *GetSocket(nwSocketHandleStruct);
    void FlushAll(void);
};

void gcNetGame::SendAllBufferedMessages(void) {
    nwSocketHandleStruct handle = nwNetwork::GetGameSocket();
    nwSocket *sock = nwSocket::GetSocket(handle);
    if (sock) {
        nwSocketHandleStruct handle2 = nwNetwork::GetGameSocket();
        nwSocket *sock2 = nwSocket::GetSocket(handle2);
        sock2->FlushAll();
    }
}
