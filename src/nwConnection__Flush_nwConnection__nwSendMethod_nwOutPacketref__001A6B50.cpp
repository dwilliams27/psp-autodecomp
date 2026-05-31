// nwConnection::Flush(nwConnection::nwSendMethod, nwOutPacket &)
// Address: 0x001a6b50, Size: 240B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionFFlush65MnwConnectiond__5MnwConnectionMnwSendMethodR6LnwOutPacket
//
// Flush the per-method outgoing block: if it holds any pending bits, bump the
// block sequence id, optionally buffer the block for reliable resend (ordered
// method == SEND_ORDERED == 2), then write it out, recompute the CRC and reset
// the stream. Split-TU: nwConnection / nwOutPacket declared locally to control
// the exact field layout SNC sees.

class nwOutPacket;

// The cOutStream embedded inside each outgoing block at block-offset 0x400.
struct nwOutStream {
    unsigned char *mBuffer;   // 0x00 — block payload pointer
    int mField04;             // 0x04
    int mBitLen;              // 0x08 — pending length in bits
    int mField0C;             // 0x0C
    int mField10;             // 0x10
    int mField14;             // 0x14 — reset to 0 on flush
    void GetCRC() const;
};

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    struct nwMsgsBlock {
        char _pad0[0x400];        // 0x000
        nwOutStream mStream;      // 0x400
        int mSeq;                 // 0x418 — block sequence id
        char _padEnd[0x428 - 0x41C];
    };

    void Flush(nwSendMethod, nwOutPacket &);
    void WriteBlock(nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *);
    void BufferOutBlock(unsigned int, int, unsigned char *);

    char _padHead[0x5C];          // 0x00
    nwMsgsBlock mBlocks[3];       // 0x5C
};

void nwConnection::Flush(nwSendMethod method, nwOutPacket &packet) {
    nwMsgsBlock &block = mBlocks[method];
    nwOutStream &stream = block.mStream;
    if (((stream.mBitLen + 7) >> 3) != 0) {
        block.mSeq = (block.mSeq + 1) & 0x3FF;
        if (method == SEND_ORDERED) {
            BufferOutBlock(block.mSeq, (stream.mBitLen + 7) >> 3, stream.mBuffer);
        }
        WriteBlock(method, packet, block.mSeq, (stream.mBitLen + 7) >> 3, stream.mBuffer);
        stream.GetCRC();
        stream.mBitLen = 0;
        stream.mField14 = 0;
    }
}
