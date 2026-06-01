// nwConnection::ReceiveAck(nwInPacket &) — read ack sequence, walk reliable
// out-block list, drop blocks whose id has been acked (compacting the buffer).
// Split-TU: nwConnection declared locally so this is the only file touched.

class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

class nwInPacket : public cInStream {};

class nwConnection {
public:
    class nwOutBlockInfo {
    public:
        unsigned char *Next();
        bool Ack(unsigned int, unsigned int) const;

        unsigned int mId;       // 0x00
        unsigned short mSize;   // 0x04
    };

    void ReceiveAck(nwInPacket &);

    char _pad00[0xCC4];                 // 0x000..0xCC4
    unsigned int mAckId;                // 0xCC4
    char _padCC8[0x18];                 // 0xCC8..0xCE0
    unsigned char *mBlockStart;         // 0xCE0
    int mBlockUsed;                     // 0xCE4
};

extern "C" void *memmove(void *, const void *, unsigned int);

void nwConnection::ReceiveAck(nwInPacket &packet) {
    unsigned int ackSeq;
    packet.Read(ackSeq, 0xA, false);

    unsigned char *end = mBlockStart + mBlockUsed;
    nwOutBlockInfo *cur = (nwOutBlockInfo *)mBlockStart;
    while ((unsigned char *)cur != end) {
        unsigned char *next = cur->Next();
        if (cur->Ack(ackSeq, mAckId)) {
            unsigned int blockBytes =
                ((((unsigned int)cur->mSize + 3) >> 2) * 4) + 0xC;
            memmove(cur, next, (unsigned int)(end - next));
            end -= blockBytes;
            mBlockUsed -= blockBytes;
        } else {
            cur = (nwOutBlockInfo *)next;
        }
    }
}
