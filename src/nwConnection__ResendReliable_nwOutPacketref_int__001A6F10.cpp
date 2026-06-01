// nwConnection::ResendReliable(nwOutPacket &, int)
// Address: 0x001a6f10, Size: 240B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionOResendReliableR6LnwOutPacketi
//
// Split-TU: class declared locally to avoid touching include/*.h.
// Iterates the reliable out-block list [mOutBlocks, mOutBlocks+mOutBlocksLen),
// resending blocks whose ack age has exceeded the configured threshold.

class nwOutPacket;

extern int D_0009F984;

class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    struct nwOutBlockInfo {
        unsigned int mSeq;       // 0x00
        unsigned short mField4;  // 0x04
        unsigned short mResend;  // 0x06
        int mAckTick;            // 0x08
        nwOutBlockInfo *Next();
    };

    void WriteBlock(nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *);
    void ResendReliable(nwOutPacket &, int);

    char _pad00[0x2C];
    int mSeqNumber;              // 0x2C
    char _pad30[0xCE0 - 0x30];
    nwOutBlockInfo *mOutBlocks;  // 0xCE0
    int mOutBlocksLen;           // 0xCE4
};

void nwConnection::ResendReliable(nwOutPacket &packet, int budget) {
    nwOutBlockInfo *end = (nwOutBlockInfo *)((char *)mOutBlocks + mOutBlocksLen);
    nwOutBlockInfo *p = mOutBlocks;
    if (p != end) {
        do {
            if ((mSeqNumber - p->mAckTick) >= D_0009F984) {
                int sz = p->mField4 + 3;
                if (budget >= sz) {
                    WriteBlock(SEND_ORDERED, packet, p->mSeq, p->mField4,
                               (unsigned char *)p + 12);
                    int *seqPtr = &mSeqNumber;
                    int *ackPtr = &p->mAckTick;
                    *ackPtr = *seqPtr;
                    budget -= sz;
                    p->mResend = (unsigned short)(p->mResend + 1);
                }
            }
            p = p->Next();
        } while (p != end);
    }
}
