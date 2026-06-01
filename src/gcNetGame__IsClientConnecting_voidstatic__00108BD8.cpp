class nwConnectionHandle {
public:
    int mHandle;
};

struct nwConnection {
    char _pad[0x20];
    int status;
};

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

class gcNetGame {
public:
    static int IsClientConnecting(void);
};

extern nwConnectionHandle gClientConnection;

int gcNetGame::IsClientConnecting(void) {
    int result = 0;
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        if (nwSocket::GetConnection(gClientConnection)->status == 1 ||
            nwSocket::GetConnection(gClientConnection)->status == 3) {
            result = 1;
        }
    }
    return result & 0xFF;
}
