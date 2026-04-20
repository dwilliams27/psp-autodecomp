extern "C" void *memset(void *, int, unsigned int);

class nwMsg;
class nwMsgBuffer;

class nwMsgType {
public:
    void Initialize(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *, nwMsg *(*)(nwMsgBuffer &));

    unsigned char mType;                       // 0x00
    unsigned char mSubType;                    // 0x01
    const char *mName;                         // 0x04
    nwMsg *(*mFactory)(nwMsgBuffer &);         // 0x08
};

class nwConnection {
public:
    struct nwOutBlockInfo {
        unsigned int mField0;                  // 0x00
        unsigned short mField4;                // 0x04
        unsigned short mField6;                // 0x06
        void *Next();
    };
    struct nwInBlockInfo {
        unsigned int mField0;                  // 0x00
        unsigned short mField4;                // 0x04
        unsigned short mField6;                // 0x06
        void *Next();
    };

    void OnConnectionAccepted();

    char _pad00[0x20];
    int mState;                                // 0x20
    char _pad24[0x08];
    int mSeqNumber;                            // 0x2C
    char _pad30[0x10];
    int mLastAckTick;                          // 0x40
};

static unsigned char sMsgTypeTableInit;
static nwMsgType *sMsgTypeTablePtr;
static nwMsgType sMsgTypeTable[64];

void nwMsgType::Initialize(unsigned char type, unsigned char subType, const char *name, nwMsg *(*factory)(nwMsgBuffer &)) {
    mName = name;
    mType = type;
    mFactory = factory;
    mSubType = subType;
}

nwMsgType *nwMsgType::InitializeType(unsigned char type, unsigned char subType, const char *name, nwMsg *(*factory)(nwMsgBuffer &)) {
    if (!sMsgTypeTableInit) {
        memset(sMsgTypeTable, 0, sizeof(sMsgTypeTable));
        sMsgTypeTablePtr = sMsgTypeTable;
        sMsgTypeTableInit = 1;
    }
    if (sMsgTypeTablePtr[type].mType != type) {
        sMsgTypeTablePtr[type].Initialize(type, subType, name, factory);
    }
    return &sMsgTypeTablePtr[type];
}

void nwConnection::OnConnectionAccepted() {
    mState = 2;
    int *src = &mSeqNumber;
    int *dst = &mLastAckTick;
    *dst = *src;
}

void *nwConnection::nwOutBlockInfo::Next() {
    unsigned int sz = (unsigned int)mField4 + 3;
    return (char *)this + ((sz >> 2) << 2) + 12;
}

void *nwConnection::nwInBlockInfo::Next() {
    unsigned int sz = (unsigned int)mField4 + 3;
    return (char *)this + ((sz >> 2) << 2) + 8;
}
