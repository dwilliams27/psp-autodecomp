// ODR-WARNING: split-TU local redeclaration of nwConnection::nwMsgsBlock.
// nwConnection::nwMsgsBlock::nwMsgsBlock(void) @ 0x001a63e4
// Emits __0o5MnwConnectionLnwMsgsBlockctv

class nwConnection {
public:
    struct nwMsgsBlock {
        char mBuffer[0x400];        // 0x000..0x3FF
        void *mSelf;                // 0x400
        unsigned int mCapacity;     // 0x404
        unsigned int mField408;     // 0x408
        unsigned int mField40C;     // 0x40C
        unsigned char mFlag410;     // 0x410
        unsigned char mFlag411;     // 0x411
        unsigned char mFlag412;     // 0x412
        char _pad413;               // 0x413
        unsigned int mField414;     // 0x414
        unsigned int mField418;     // 0x418
        unsigned int mField41C;     // 0x41C
        unsigned int mField420;     // 0x420
        char _padEnd[0x4];          // 0x424..0x428

        nwMsgsBlock();
    };
};

nwConnection::nwMsgsBlock::nwMsgsBlock() {
    unsigned char zero = 0;
    mSelf = this;
    mCapacity = 0x400;
    mField408 = 0;
    mField40C = 0;
    mFlag410 = zero;
    mFlag411 = 0;
    mFlag412 = 0;
    mField414 = 0;
    mField418 = 0;
    mField41C = 0;
    mField420 = 0;
}
