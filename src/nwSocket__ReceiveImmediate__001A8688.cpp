// nwSocket::ReceiveImmediate(nwInPacket &, const nwAddress &, nwConnectionHandle)
// Address: 0x001a8688, Size: 148B
// Obj: nwAll_psp.obj
// Symbol: __0fInwSocketQReceiveImmediateR6KnwInPacketRC6JnwAddress6SnwConnectionHandle
//
// Validate the packet's embedded id (ReadId). If valid, round the packet's bit
// length up to the next multiple of 8 and dispatch it through the static
// nwConnection::DispatchMessage using this socket's handle. If the id is
// invalid, just set the bit length from the byte length (<<3) and bail.
// Split-TU: classes declared locally to avoid touching include/*.h.

class nwAddress;

class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

class nwInPacket : public cInStream {
public:
    int mField00;   // 0x00
    int mField04;   // 0x04
    int mField08;   // 0x08
};

class nwConnectionHandle {
public:
    int mValue;
};

class nwConnection {
public:
    static void DispatchMessage(cInStream &, int, const nwAddress &,
                                nwConnectionHandle, bool);
};

class nwSocket {
public:
    char _pad00[0x10];     // 0x00
    int mHandle;           // 0x10

    bool ReadId(cInStream &in);
    void ReceiveImmediate(nwInPacket &packet, const nwAddress &addr,
                          nwConnectionHandle handle);
};

void nwSocket::ReceiveImmediate(nwInPacket &packet, const nwAddress &addr,
                                nwConnectionHandle handle) {
    if (!ReadId(packet)) {
        packet.mField08 = packet.mField04 << 3;
        return;
    }
    packet.mField08 = (((unsigned int)(packet.mField08 + 7) >> 3) << 3);
    nwConnection::DispatchMessage(packet, mHandle, addr, handle, false);
}
