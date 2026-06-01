// gcNetGame::SendTo(nwConnection::nwSendMethod, int, nwConnectionHandle *, const nwMsg &, unsigned int) static
// Address: 0x00108914, Size: 168B
// Obj: gcAll_psp.obj
//
// Loops over a count of connection handles; for each handle resolves an
// nwConnection* via nwSocket::GetConnection and, if present, forwards the
// message through nwConnection::Send with a null cOutStream*. Sibling of
// gcNetGame::SendToServer (single-handle variant) and SendToAll.

class nwConnectionHandle {
    int h;
};

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
    static void SendTo(nwConnection::nwSendMethod, int, nwConnectionHandle *, const nwMsg &, unsigned int);
};

void gcNetGame::SendTo(nwConnection::nwSendMethod method, int count, nwConnectionHandle *handles, const nwMsg &msg, unsigned int ui) {
    int i = 0;
    if (i < count) {
        nwConnectionHandle *p = handles + i;
        do {
            if (nwSocket::GetConnection(*p) != 0) {
                nwSocket::GetConnection(*p)->Send(method, msg, ui, 0);
            }
            i++;
            p++;
        } while (i < count);
    }
}
