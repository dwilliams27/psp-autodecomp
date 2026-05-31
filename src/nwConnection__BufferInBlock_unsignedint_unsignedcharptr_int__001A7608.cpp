// nwConnection::BufferInBlock(unsigned int, unsigned char *, int)
// Address: 0x001a7608, Size: 244B, obj: nwAll_psp.obj
// Symbol: __0fMnwConnectionNBufferInBlockUiPUci

extern "C" void *memcpy(void *, const void *, unsigned int);
extern "C" void *memmove(void *, const void *, unsigned int);

class nwConnection {
public:
    struct nwInBlockInfo {
        unsigned int mBlockId;   // 0x00
        short mSize;             // 0x04
        // payload follows at +8
        nwInBlockInfo *Next();
    };

    void BufferInBlock(unsigned int, unsigned char *, int);

    char _pad[0xCD4];
    int mCapacity;                  // 0xCD4
    nwInBlockInfo *mBufferBase;     // 0xCD8
    int mUsedBytes;                 // 0xCDC
};

void nwConnection::BufferInBlock(unsigned int blockId, unsigned char *data, int size) {
    int entrySize = (int)(((unsigned int)(size + 3) >> 2) * 4) + 8;
    int used = mUsedBytes;
    if (mCapacity < entrySize + used) {
        return;
    }

    nwInBlockInfo *end = (nwInBlockInfo *)((char *)mBufferBase + used);
    nwInBlockInfo *cur = mBufferBase;

    if (cur != end) {
        do {
            unsigned int id = cur->mBlockId;
            if (id == blockId) {
                return;
            }
            if (blockId < id) {
                break;
            }
            cur = cur->Next();
        } while (cur != end);
    }

    int remaining = (int)((char *)end - (char *)cur);
    if (remaining > 0) {
        memmove((char *)cur + entrySize, cur, remaining);
    }
    cur->mBlockId = blockId;
    cur->mSize = (short)size;
    memcpy((char *)cur + 8, data, size);
    mUsedBytes = mUsedBytes + entrySize;
}
