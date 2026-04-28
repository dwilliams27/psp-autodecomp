// gcMsgLoadRegions - gcAll_psp.obj
//   0x00264898 New(nwMsgBuffer &) static
//   0x00124d10 Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)

class nwMsg;
class cInStream;
class nwAddress;
class gcRegion;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));

struct cGUID {
    int a;
    int b;
    void Read(cInStream &);
};

template <class T>
class cGUIDT : public cGUID {
};

struct cInStreamBits {
    unsigned char *mBuf;
    int mCapacity;
    int mBitPos;
};

class gcEvent;

class gcMap {
public:
    char _pad[0x140];
    int mFlags;

    void LoadRegionsBackground(const cGUIDT<gcRegion> *, const gcEvent *,
                               const cGUIDT<gcRegion> *, bool, cGUIDT<gcRegion>);
};

class gcMsgLoadRegions {
public:
    void *mVTable;
    cGUIDT<gcRegion> mRegions[2];
    bool mFlag;

    static nwMsg *New(nwMsgBuffer &);
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

nwMsg *gcMsgLoadRegions::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x18;
    buf.mOffset = cursor;
    gcMsgLoadRegions *obj = (gcMsgLoadRegions *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        obj->mVTable = (void *)0x389070;
        __vec_new(&obj->mRegions[0], 2, 8, (void (*)(void *))0x245578);
        obj->mFlag = false;
        result = (nwMsg *)obj;
    }
    return result;
}

void gcMsgLoadRegions::Read(cInStream &stream, nwSocketHandle,
                            const nwAddress &, nwConnectionHandle) {
    int i = 0;
    cGUIDT<gcRegion> *region = (cGUIDT<gcRegion> *)((char *)this + 4);
    do {
        region->Read(stream);
        i++;
        region++;
    } while (i < 2);

    cInStreamBits *bits = (cInStreamBits *)&stream;
    int bitPos = bits->mBitPos;
    unsigned char *data = bits->mBuf;
    int *bitPtr = &bits->mBitPos;
    unsigned char byte = data[bitPos >> 3];
    bitPos = *bitPtr;
    int mask = 1 << (bitPos & 7);
    bitPos++;
    *bitPtr = bitPos;
    mFlag = (byte & mask) != 0;

    gcMap *map = *(gcMap **)0x37D7FC;
    if (map != 0) {
        int ready = 0;
        if ((map->mFlags & 1) != 0) {
            ready = 1;
        }
        if (ready != 0) {
            __asm__ volatile(
                "lbu $8, 0x14(%0)\n\t"
                "addiu $5, %0, 0x4\n\t"
                "or $4, %1, $0\n\t"
                "or $6, $0, $0\n\t"
                "or $7, $0, $0\n\t"
                ".extern __0fFgcMapVLoadRegionsBackgroundPC6GcGUIDT76IgcRegion_PC6HgcEventTCbTE\n\t"
                ".set noreorder\n\t"
                "jal __0fFgcMapVLoadRegionsBackgroundPC6GcGUIDT76IgcRegion_PC6HgcEventTCbTE\n\t"
                "or $9, $0, $0\n\t"
                ".set reorder"
                :
                : "r"(this), "r"(map)
                : "$4", "$5", "$6", "$7", "$8", "$9", "memory");
        }
    }
}
