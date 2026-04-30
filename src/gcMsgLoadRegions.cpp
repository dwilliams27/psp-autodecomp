// gcMsgLoadRegions - gcAll_psp.obj
//   0x00264898 New(nwMsgBuffer &) static
//   0x00124d10 Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)

class nwMsg;
class nwAddress;
class gcRegion;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));

class cInStream;
class cOutStream {
public:
    unsigned char *mData;
    int mCapacity;
    int mBitPos;
    int mCRC;
    unsigned char mDirty;
    char _pad11;
    unsigned char mOverflow;
    char _pad13;
    int mCRCBitPos;
};

struct cGUID {
    int a;
    int b;
    void Read(cInStream &);
    void Write(cOutStream &) const;
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

    gcMsgLoadRegions(cGUIDT<gcRegion> *, bool);
    static nwMsg *New(nwMsgBuffer &);
    void Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const;
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

gcMsgLoadRegions::gcMsgLoadRegions(cGUIDT<gcRegion> *regions, bool flag) {
    gcMsgLoadRegions *self = this;
    cGUIDT<gcRegion> *regionBase = regions;
    self->mVTable = (void *)0x389070;
    __vec_new((char *)self + 4, 2, 8, (void (*)(void *))0x245578);
    self->mFlag = flag;
    int i = 0;
    do {
        cGUIDT<gcRegion> *dst = &((cGUIDT<gcRegion> *)((char *)self + 4))[i];
        cGUIDT<gcRegion> *src = &regionBase[i];
        int a = src->a;
        int b = src->b;
        dst->a = a;
        dst->b = b;
        __asm__ volatile("" ::: "memory");
        i++;
    } while (i < 2);
}

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

void gcMsgLoadRegions::Write(cOutStream &stream, nwSocketHandle,
                             const nwAddress &, nwConnectionHandle) const {
    int i = 0;
    const cGUIDT<gcRegion> *region = &mRegions[0];
    do {
        region->Write(stream);
        i++;
        region++;
    } while (i < 2);

    int value = mFlag & 0xFF;
    int bitPos = stream.mBitPos;
    int bit = bitPos & 7;
    unsigned char *out = stream.mData + (bitPos >> 3);
    int nextBit = bitPos + 1;
    unsigned char overflow = stream.mOverflow;
    stream.mBitPos = nextBit;

    if (overflow == 0) {
        if (stream.mCapacity < ((stream.mBitPos + 7) >> 3)) {
            stream.mOverflow = 1;
            overflow = stream.mOverflow;
        }
    }

    unsigned int writable = overflow < 1U;

    if ((writable & 0xFF) != 0) {
        unsigned char old = *out;
        int mask = 1 << bit;
        *out = (old & ~mask) | ((value != 0) << bit);
    }
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
