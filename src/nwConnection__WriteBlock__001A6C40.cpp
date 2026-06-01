// nwConnection::WriteBlock(nwConnection::nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *)
// Address: 0x001a6c40, Size: 144B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionKWriteBlock65MnwConnectiond__5MnwConnectionMnwSendMethodR6LnwOutPacketUiTDPUc
//
// Write a single outgoing block header + payload into the packet's bit stream:
//   Write(method + 7, 4 bits)   -- block kind/method tag
//   Write(seq,        10 bits)  -- block sequence id
//   Write(len,        10 bits)  -- payload byte length
//   WriteRaw(len, data)         -- raw payload bytes
// Split-TU: nwConnection / nwOutPacket declared locally. nwOutPacket is the
// bit-output stream the block is serialized into (cOutStream-compatible).

class nwOutPacket {
public:
    void Write(unsigned int, int, bool);
    void WriteRaw(int, const void *);
};

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    void WriteBlock(nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *);
};

void nwConnection::WriteBlock(nwSendMethod method, nwOutPacket &packet,
                              unsigned int seq, unsigned int len,
                              unsigned char *data) {
    packet.Write((unsigned int)(method + 7), 4, false);
    packet.Write(seq, 0xA, false);
    packet.Write(len, 0xA, false);
    packet.WriteRaw((int)len, data);
}
