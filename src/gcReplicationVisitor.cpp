#include "gcCinematic.h"
#include "gcReplicationVisitor.h"

class cInStream {
public:
    void Read(unsigned int &, int, bool);
};

extern unsigned short gcReplicationGroup_s_nUpdateCount;

struct gcRepEntry {
    char pad;              // offset 0
    signed char priority;  // offset 1
    unsigned char age;     // offset 2
};

struct BitStream {
    char *data;         // 0x00
    int size;           // 0x04
    int bitPos;         // 0x08
    int field_0C;       // 0x0C
    unsigned char flag10;  // 0x10
    unsigned char flag11;  // 0x11
    unsigned char overflow;// 0x12
    unsigned char pad13;   // 0x13
    int field_14;       // 0x14
};

void gcReplicationVisitor::EndRead(void) {
    mReadActive = 0;
}

void gcReplicationVisitor::SetNetStream(cInStream *stream, nwConnectionHandle handle, bool writeEnabled, bool byteSwap) {
    mInStream = stream;
    int netConn = -1;
    if (nwSocket::GetConnection(handle)) {
        netConn = handle.mHandle & 0xFF;
    }
    mNetConnection = netConn;
    mWriteEnabled = writeEnabled;
    mByteSwap = byteSwap;
}

void gcReplicationVisitor::SetNetStream(cOutStream *stream, nwConnectionHandle handle, bool writeEnabled, bool byteSwap) {
    mOutStream = stream;
    *(char *)((char *)stream + 0x11) = 0;
    int netConn = -1;
    if (nwSocket::GetConnection(handle)) {
        netConn = handle.mHandle & 0xFF;
    }
    mNetConnection = netConn;
    mWriteEnabled = writeEnabled;
    mByteSwap = byteSwap;
}

int gcReplicationVisitor::BeginRead(gcReplicationGroup *group) {
    BitStream *stream = (BitStream *)mInStream;
    int bp0 = stream->bitPos;
    char *data = stream->data;
    char *bytePtr = data + (bp0 >> 3);
    int *bitPosPtr = &stream->bitPos;
    int bp = *bitPosPtr;
    int bitOff = bp & 7;
    unsigned char byte = *(unsigned char *)bytePtr;
    *bitPosPtr = bp + 1;
    int mask = 1 << bitOff;
    int dirty = byte & mask;
    dirty = (dirty != 0);

    if (!dirty) {
        return 0;
    }

    if (mMode == 0x1002) {
        int conn = mNetConnection;
        gcReplicationGroup *groupSave = group;
        unsigned short oldVer = *(unsigned short *)((char *)group + conn * 2 + 4);
        unsigned int readVal;
        ((cInStream *)mInStream)->Read(readVal, 8, true);
        int newByte = readVal & 0xFF;
        conn = mNetConnection;
        unsigned short *verPtr = (unsigned short *)((char *)groupSave + conn * 2 + 4);
        if (mByteSwap) {
            oldVer = (oldVer & 0xFF00) | newByte;
        } else {
            oldVer = (oldVer & 0xFF) | (newByte << 8);
        }
        if (*verPtr != oldVer) {
            *verPtr = 0;
        }
        return 0;
    }

    if (mMode == 0x802) {
        *((char *)group + mNetConnection + 2) = (char)gcReplicationGroup_s_nUpdateCount;
    }

    mReadActive = (int)group;
    return (int)mInStream;
}

cOutStream *gcReplicationVisitor::BeginWrite(gcReplicationGroup *group, int maxAge) {
    int write;

    if (mMode == 0x401) {
        *(unsigned short *)((char *)group + mNetConnection * 2 + 4) = 0;
        *((char *)group + mNetConnection + 2) = 0;
        return 0;
    }

    unsigned char updateCount = (unsigned char)gcReplicationGroup_s_nUpdateCount;
    write = 1;

    if (mMode == 0x201) {
        gcRepEntry *entry = (gcRepEntry *)((char *)group + mNetConnection);
        if (((updateCount - entry->age) & 0xFF) < 31) {
            write = 0;
        } else {
            entry->age = updateCount;
        }
    } else if (mMode == 0x101) {
        if (!mWriteEnabled) {
            int conn = mNetConnection;
            unsigned short version = *(unsigned short *)((char *)group + conn * 2 + 4);
            if (version != 0) {
                gcRepEntry *entry = (gcRepEntry *)((char *)group + conn);
                if (((updateCount - entry->age) & 0xFF) >= maxAge) {
                    unsigned char idx = *(unsigned char *)group;
                    if (*(unsigned char *)((char *)group + idx + 2) != updateCount) {
                        // write stays 1
                    } else if (*(unsigned short *)((char *)group + idx * 2 + 4) != version) {
                        // write stays 1
                    } else if (entry->priority > 0) {
                        // write stays 1
                    } else {
                        write = 0;
                    }
                } else {
                    write = 0;
                }
            }
        }
    }

    // Bit write section
    BitStream *os = (BitStream *)mOutStream;
    int bitPos = os->bitPos;
    char *data = os->data;
    int bitOff = bitPos & 7;
    int byteOff = bitPos >> 3;
    os->bitPos = bitPos + 1;
    char *bytePtr = data + byteOff;
    int writeFlag = write & 0xFF;

    if (os->overflow == 0) {
        int newBitPos = os->bitPos;
        int size = os->size;
        if (size < ((newBitPos + 7) >> 3)) {
            os->overflow = 1;
        }
    }

    if ((unsigned char)(os->overflow == 0)) {
        unsigned char curByte = *(unsigned char *)bytePtr;
        int mask = 1 << bitOff;
        int writeBit = (writeFlag != 0);
        *bytePtr = (curByte & ~mask) | (writeBit << bitOff);
    }

    if (write) {
        BitStream *outStream = (BitStream *)mOutStream;
        mReadActive = (int)group;
        mStreamPos = outStream->bitPos;
        outStream->flag10 = 1;
        outStream->field_14 = outStream->bitPos;

        outStream = (BitStream *)mOutStream;
        outStream->field_0C = 0;
        outStream->field_14 = outStream->bitPos;
        return mOutStream;
    }
    return 0;
}

gcRegionBase &gcRegionBase::operator=(const gcRegionBase &other) {
    return *this;
}

gcLoadingScreen &gcLoadingScreen::operator=(const gcLoadingScreen &other) {
    return *this;
}

gcMap &gcMap::operator=(const gcMap &other) {
    return *this;
}

gcGame &gcGame::operator=(const gcGame &other) {
    return *this;
}
