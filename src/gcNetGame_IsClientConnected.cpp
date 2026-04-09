#include "gcGame.h"

struct nwConnection {
    char _pad[0x20];
    int status;
};

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

extern nwConnectionHandle gClientConnection;

int gcNetGame::IsClientConnected(void) {
    int result = 0;
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        if (nwSocket::GetConnection(gClientConnection)->status == 2) {
            result = 1;
        }
    }
    return result & 0xFF;
}
