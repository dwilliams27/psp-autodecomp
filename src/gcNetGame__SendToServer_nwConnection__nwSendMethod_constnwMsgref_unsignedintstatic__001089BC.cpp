// gcNetGame::SendToServer(nwConnection::nwSendMethod, const nwMsg &, unsigned int) static
// Address: 0x001089bc, Size: 116B
// Obj: gcAll_psp.obj
//
// Looks up the client server connection (handle at gClientConnection, 0xa9a3c8)
// via nwSocket::GetConnection; if present, forwards the message through
// nwConnection::Send with a null cOutStream*.

typedef int nwConnectionHandle;

class nwMsg;
class cOutStream;

class nwConnection {
public:
    enum nwSendMethod {};
    int Send(nwConnection::nwSendMethod, const nwMsg &, unsigned int, cOutStream *);
};

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

class gcNetGame {
public:
    static void SendToServer(nwConnection::nwSendMethod, const nwMsg &, unsigned int);
};

extern nwConnectionHandle gClientConnection;

void gcNetGame::SendToServer(nwConnection::nwSendMethod method, const nwMsg &msg, unsigned int ui) {
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        nwSocket::GetConnection(gClientConnection)->Send(method, msg, ui, 0);
    }
}
