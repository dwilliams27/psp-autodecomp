#include "gcGame.h"

struct nwConnection {
    char _pad[0x20];
    int status;
};

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
    static nwConnection *GetConnection(nwConnectionHandle);
    void FlushAll(void);
};

extern gcGame *gpGame;
extern nwConnectionHandle gClientConnection;

void gcNetGame::SetMaxConnections(int n) {
    *(int *)0x37D860 = n;
}

int gcNetGame::GetMaxConnections(void) {
    return *(int *)0x37D860;
}

void gcNetGame::ResetAllNetworkConnections(void) {
    int i = 0;
    do {
        gpGame->ResetNetConnection(i);
        i++;
    } while (i < 1);
}

void gcNetGame::SendAllBufferedMessages(void) {
    nwSocketHandleStruct handle = nwNetwork::GetGameSocket();
    nwSocket *sock = nwSocket::GetSocket(handle);
    if (sock) {
        nwSocketHandleStruct handle2 = nwNetwork::GetGameSocket();
        nwSocket *sock2 = nwSocket::GetSocket(handle2);
        sock2->FlushAll();
    }
}

int gcNetGame::IsClientConnected(void) {
    int result = 0;
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        if (nwSocket::GetConnection(gClientConnection)->status == 2) {
            result = 1;
        }
    }
    return result & 0xFF;
}
