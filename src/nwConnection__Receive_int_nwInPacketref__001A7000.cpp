// nwConnection::Receive(int, nwInPacket &)
// Address: 0x001a7000, Size: 252B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionHReceiveiR6KnwInPacket
//
// Dispatch an incoming packet by message type. If the connection is not yet
// established (mField20 == 0) we only reset mField08 from the handle and bail.
// Otherwise a switch on the message type routes to the per-type Receive*
// handlers; types 7/8/9 map to the three send methods of ReceiveMessages.
// Split-TU: class declared locally to avoid touching include/*.h.

class nwInPacket {
public:
    int mField00;   // 0x00
    int mField04;   // 0x04
    int mField08;   // 0x08
};

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    enum nwConnectionError {
        ERROR_NONE = 0
    };

    int Receive(int, nwInPacket &);
    void ReceiveAck(nwInPacket &);
    void ReceivePing(nwInPacket &);
    void ReceivePingReply(nwInPacket &);
    void ReceiveMessages(nwSendMethod, nwInPacket &);
    void SetError(nwConnectionError);

    int mField00;        // 0x00
    int mHandle;         // 0x04
    int mField08;        // 0x08
    char _pad0C[0x14];   // 0x0C
    int mField20;        // 0x20
    char _pad24[0x08];   // 0x24
    int mField2C;        // 0x2C
    char _pad30[0x04];   // 0x30
    int mField34;        // 0x34
};

int nwConnection::Receive(int type, nwInPacket &packet) {
    if (mField20 == 0) {
        packet.mField08 = packet.mField04 << 3;
        return 0;
    }

    switch (type) {
    case 3:
        ReceivePing(packet);
        break;
    case 4:
        ReceivePingReply(packet);
        break;
    case 5:
        ReceiveAck(packet);
        break;
    case 6:
        SetError((nwConnectionError)6);
        break;
    case 7:
    case 8:
    case 9:
        ReceiveMessages((nwSendMethod)(type - 7), packet);
        break;
    default:
        return 0;
    }

    int *src = &mField2C;
    int *dst = &mField34;
    int result = 1;
    *dst = *src;
    return result;
}
