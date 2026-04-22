#pragma once

class cTimeValue;
class cInStream;
class cOutStream;
class nwAddress;
class nwMsg;
class nwSocket;
class nwInPacket;
class nwOutPacket;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

// nwConnection: network connection object.
// Constructor (0x1a6420) takes (nwSocket*, nwConnectionHandle, int, const nwAddress&,
//   void (*)(nwConnectionHandle, nwConnection::nwConnectionError)).
// Ctor field inits:
//   +0x00: nwSocket* (socket)
//   +0x04: nwConnectionHandle (handle)
//   +0x08: int (from address arg)
//   +0x0C..0x18: nwAddress fields (4 ints copied from arg, 16 bytes)
//   +0x1C..0x3C: zeroed (8 ints)
//   +0x40: int = 0
//   +0x44: int = -1
//   +0x48: int = 0
//   +0x4C: float = 0.0 (ping/RTT)
//   +0x50: byte = 0 (flag)
//   +0x58: void* = error callback
//   +0x5C..0xCD4: nwMsgsBlock array (3 blocks of 0x428 bytes each, initialized by subroutine)
//   +0xCD4: void* (error callback stored again)
//   +0xCD8..0xCEC: zeroed fields
//   +0xCEC: float = 0.0 (send rate)
// UpdatePing (0x1a69e8) reads:
//   +0x2C: int (seq number / tick count)
//   +0x40: int (last ack tick)
//   +0x48: int (accumulated ping samples)
//   +0x4C: float (smoothed ping)
// Struct size: at least 0xCF0 bytes
class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    enum nwConnectionError {
        ERROR_NONE = 0,
        ERROR_TIMEOUT = 1,
        ERROR_REJECTED = 2
    };

    struct nwMsgsBlock {
        nwMsgsBlock();
        char _pad0[0x41C];
        unsigned int mLastSeq;  // 0x41C
        char _padEnd[0x8];      // 0x420..0x428
    };

    struct nwOutBlockInfo {
        void Next();
        int Ack(unsigned int, unsigned int) const;
    };

    struct nwInBlockInfo {
        void Next();
    };

    nwConnection(nwSocket *, nwConnectionHandle, int, const nwAddress &, void (*)(nwConnectionHandle, nwConnectionError));
    ~nwConnection();

    void Send(nwSendMethod, const nwMsg &, unsigned int, cOutStream *);
    void Close();
    void FlushAll();
    int GetQuality() const;
    void SetError(nwConnectionError);
    void Connect(const char *, bool);
    void StartConnection();
    void Accept();
    void OnConnectionAccepted();
    void AllocateBuffers();
    void Update(cTimeValue);
    void UpdatePing(cTimeValue);
    void UpdateSendRate(cTimeValue);
    void Flush(nwSendMethod, nwOutPacket &);
    void Flush(nwSendMethod);
    void WriteBlock(nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *);
    void SendAckAndPing(nwOutPacket &);
    void BufferOutBlock(unsigned int, int, unsigned char *);
    void ResendReliable(nwOutPacket &, int);
    void Receive(int, nwInPacket &);
    void ReceiveAck(nwInPacket &);
    void BufferInBlock(unsigned int, unsigned char *, int);
    void ResendConnect();
    void TransportSend(const nwAddress &, const nwOutPacket &);
    bool IsBlockIdOld(unsigned int blockId, nwSendMethod method) const;

    static void DispatchMessages(unsigned char *, int, nwSocketHandle, const nwAddress &, nwConnectionHandle, bool);
    static void DispatchMessage(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle, bool);
    static void BuildMessage(const nwMsg &, cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    nwSocket *mSocket;              // 0x00
    int mHandle;                    // 0x04
    int mField08;                   // 0x08
    char _padAddress[0x10];         // 0x0C (nwAddress, 16 bytes)
    char _pad1C[0x10];             // 0x1C
    int mSeqNumber;                 // 0x2C
    char _pad30[0x10];             // 0x30
    int mLastAckTick;               // 0x40
    int mField44;                   // 0x44
    int mPingSamples;               // 0x48
    float mSmoothedPing;            // 0x4C
    unsigned char mFlag50;          // 0x50
    char _pad51[3];                 // 0x51
    char _pad54[4];                 // 0x54
    void *mErrorCallback;           // 0x58
    nwMsgsBlock mBlocks[3];         // 0x5C (3 * 0x428 = 0xC78 bytes)
    void *mErrorCallback2;          // 0xCD4
    int mFieldCD8;                  // 0xCD8
    int mFieldCDC;                  // 0xCDC
    int mFieldCE0;                  // 0xCE0
    int mFieldCE4;                  // 0xCE4
    int mFieldCE8;                  // 0xCE8
    float mSendRate;                // 0xCEC
};
