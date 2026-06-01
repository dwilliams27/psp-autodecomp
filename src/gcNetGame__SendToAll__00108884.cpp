// gcNetGame::SendToAll(nwConnection::nwSendMethod, const nwMsg &, unsigned int, nwConnectionHandle) static
// Address: 0x00108884, Size: 144B
// Obj: gcAll_psp.obj
//
// Static forwarder: fetch the game socket handle via nwNetwork::GetGameSocket,
// resolve it to an nwSocket* via nwSocket::GetSocket; if present, forward the
// message to nwSocket::SendToAll with the connection handle.

class nwSocketHandle {
    int h;
};

class nwConnectionHandle {
    int h;
};

class nwMsg;

class nwConnection {
public:
    enum nwSendMethod {};
};

class nwSocket {
public:
    static nwSocket *GetSocket(nwSocketHandle);
    void SendToAll(nwConnection::nwSendMethod, const nwMsg &, unsigned int, nwConnectionHandle);
};

class nwNetwork {
public:
    static nwSocketHandle GetGameSocket();
};

class gcNetGame {
public:
    static void SendToAll(nwConnection::nwSendMethod, const nwMsg &, unsigned int, nwConnectionHandle);
};

void gcNetGame::SendToAll(nwConnection::nwSendMethod method, const nwMsg &msg, unsigned int ui, nwConnectionHandle handle) {
    nwSocketHandle h1 = nwNetwork::GetGameSocket();
    if (nwSocket::GetSocket(h1) != 0) {
        nwSocketHandle h2 = nwNetwork::GetGameSocket();
        nwSocket::GetSocket(h2)->SendToAll(method, msg, ui, handle);
    }
}
