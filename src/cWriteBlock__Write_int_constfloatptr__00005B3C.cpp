// cWriteBlock::Write(int, const float *) @ 0x00005B3C
// Split-TU: cWriteBlock is defined in src/cWriteBlock.cpp; this overload is
// missing from that TU, so declare the class locally here.

struct cFileHandle;

class cFile {
public:
    cFileHandle *mHandle;
};

class cFileSystem {
public:
    static int Write(cFileHandle *, const void *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    void Write(int, const float *);
};

extern unsigned char gByteSwap;

void cWriteBlock::Write(int count, const float *data) {
    if (!gByteSwap) {
        cFileSystem::Write(mFile->mHandle, data, count * 4);
    } else {
        while (count > 0) {
            unsigned int tmp = *(const unsigned int *)data;
            unsigned int hi = ((tmp & 0xFF000000) >> 24) | ((tmp & 0xFF0000) >> 8);
            unsigned int mid = (tmp & 0xFF00) << 8;
            unsigned int lo = (tmp & 0xFF) << 24;
            lo = mid | lo;
            tmp = hi | lo;
            cFileSystem::Write(mFile->mHandle, &tmp, 4);
            count--;
            data++;
        }
    }
}
