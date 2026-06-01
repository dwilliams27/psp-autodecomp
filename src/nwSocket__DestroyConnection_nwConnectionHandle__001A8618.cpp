// nwSocket::DestroyConnection(nwConnectionHandle)
// Address: 0x001a8618, Size: 112B
// Obj: nwAll_psp.obj
//
// If the static GetConnection(handle) lookup succeeds, index the connection
// array by the low byte of the handle, and if that slot is non-null destroy
// the connection (dtor with delete flag 3) and clear the slot.

class nwConnection {
public:
    ~nwConnection();
};

class nwConnectionHandle {
public:
    int mValue;
    operator int() const { return mValue; }
};

class nwSocket {
public:
    int mPad0;                   // 0x00
    int mPad4;                   // 0x04
    int mNumConnections;         // 0x08
    nwConnection **mConnections; // 0x0C

    void DestroyConnection(nwConnectionHandle handle);
    static nwConnection *GetConnection(nwConnectionHandle handle);
};

void nwSocket::DestroyConnection(nwConnectionHandle handle) {
    if (GetConnection(handle) != 0) {
        int index = (handle.mValue & 0xFF) * 4;
        nwConnection *conn = *(nwConnection **)((char *)mConnections + index);
        if (conn != 0) {
            delete conn;
            *(nwConnection **)((char *)mConnections + index) = 0;
        }
    }
}
