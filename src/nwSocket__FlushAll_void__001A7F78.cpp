// nwSocket::FlushAll(void)
// Address: 0x001a7f78, Size: 116B
// Obj: nwAll_psp.obj
//
// Iterates the connection array; flushes each non-null connection.

class nwConnection {
public:
    void FlushAll();
};

class nwSocket {
public:
    int mPad0;                   // 0x00
    int mPad4;                   // 0x04
    int mNumConnections;         // 0x08
    nwConnection **mConnections; // 0x0C

    void FlushAll();
};

void nwSocket::FlushAll() {
    for (int i = 0; i < mNumConnections; i++) {
        nwConnection *conn = mConnections[i];
        if (conn != 0) {
            conn->FlushAll();
        }
    }
}
