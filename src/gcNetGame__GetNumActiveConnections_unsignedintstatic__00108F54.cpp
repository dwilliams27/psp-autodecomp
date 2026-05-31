#include "gcGame.h"

struct nwConnection {
    char _pad[0x20];
    int status;
};

class nwSocketHandleT {
public:
    int mHandle;
};

class nwSocket {
public:
    int GetMaxConnections(void) const;
    nwConnectionHandle GetConnection(int) const;
    static nwConnection *GetConnection(nwConnectionHandle);
    static nwSocket *GetSocket(nwSocketHandleT);
};

class nwNetwork {
public:
    static nwSocketHandleT GetGameSocket(void);
};

extern nwConnectionHandle gClientConnection;

int gcNetGame::GetNumActiveConnections(unsigned int) {
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        return 1;
    }

    int count = 0;
    nwSocketHandleT sock = nwNetwork::GetGameSocket();
    if (nwSocket::GetSocket(sock) != 0) {
        for (int i = 0; i < nwSocket::GetSocket(sock)->GetMaxConnections(); i++) {
            nwConnectionHandle h = nwSocket::GetSocket(sock)->GetConnection(i);
            nwConnection *c = nwSocket::GetConnection(h);
            if (c != 0 && c->status != 0) {
                count++;
            }
        }
    }
    return count;
}
