// ODR-WARNING: split-TU local redeclaration of nwConnection.
// nwConnection::nwConnection(nwSocket*, nwConnectionHandle, int, const nwAddress&,
//   void (*)(nwConnectionHandle, nwConnection::nwConnectionError)) @ 0x001a6420
// Emits __0oMnwConnectionctP6InwSocket6SnwConnectionHandleiRC6JnwAddressPF6SnwConnectionHandle65MnwConnectioni__5MnwConnectionRnwConnectionError_v

class nwConnectionHandle {
public:
    int mHandle;
};

class nwSocket;

class nwAddress {
public:
    int mType;        // 0x00
    int mData[4];     // 0x04..0x14
};

class nwConnection {
public:
    enum nwConnectionError {
        ERROR_NONE = 0
    };

    typedef void (*ErrorCb)(nwConnectionHandle, nwConnectionError);

    nwConnection(nwSocket *, nwConnectionHandle, int, const nwAddress &, ErrorCb);

    nwSocket *mSocket;              // 0x00
    nwConnectionHandle mHandle;     // 0x04
    int mAddrType;                  // 0x08
    int mAddrData[4];               // 0x0C..0x18
    int mField1C;                   // 0x1C
    int mField20;                   // 0x20
    int mField24;                   // 0x24
    int mField28;                   // 0x28
    int mField2C;                   // 0x2C
    int mField30;                   // 0x30
    int mField34;                   // 0x34
    int mField38;                   // 0x38
    int mField3C;                   // 0x3C
    int mField40;                   // 0x40
    int mField44;                   // 0x44
    int mField48;                   // 0x48
    float mField4C;                 // 0x4C
    unsigned char mFlag50;          // 0x50
    char _pad51[3];                 // 0x51
    char _pad54[4];                 // 0x54
    ErrorCb mErrorCallback;         // 0x58
    char mBlocks[3 * 0x428];        // 0x5C
    int mFieldCD4;                  // 0xCD4
    int mFieldCD8;                  // 0xCD8
    int mFieldCDC;                  // 0xCDC
    int mFieldCE0;                  // 0xCE0
    int mFieldCE4;                  // 0xCE4
    int mFieldCE8;                  // 0xCE8
    float mFieldCEC;                // 0xCEC
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern void nwMsgsBlockCtor(void *);

nwConnection::nwConnection(nwSocket *socket, nwConnectionHandle handle, int param3,
                           const nwAddress &addr, ErrorCb cb) {
    mSocket = socket;
    mHandle.mHandle = handle.mHandle;
    mAddrType = addr.mType;
    const int *asrc = &addr.mType;
    for (int i = 0; i < 4; i++) {
        mAddrData[i] = asrc[i + 1];
    }
    mField1C = 0;
    mField20 = 0;
    mField24 = 0;
    mField28 = 0;
    mField2C = 0;
    mField30 = 0;
    mField34 = 0;
    mField38 = 0;
    mField3C = 0;
    mField40 = 0;
    mField44 = -1;
    mField48 = 0;
    mField4C = 0.0f;
    mErrorCallback = cb;
    __vec_new(mBlocks, 3, 0x428, nwMsgsBlockCtor);
    mFieldCD4 = param3;
    mFieldCD8 = 0;
    mFieldCDC = 0;
    mFieldCE0 = 0;
    mFieldCE4 = 0;
    mFieldCE8 = 0;
    mFieldCEC = 0.0f;
    mFlag50 = 0;
}
