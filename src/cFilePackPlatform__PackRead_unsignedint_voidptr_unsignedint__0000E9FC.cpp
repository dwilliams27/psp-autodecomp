#include "cFileSystem.h"

class cFileSystemRead {
public:
    static int Read(cFileHandle *handle, void *buf, unsigned int size);
};

class cFilePackPlatform {
public:
    void *mPackData;         // 0x00
    char _pad4[0x14];        // 0x04
    cFileHandle *mHandle;    // 0x18

    int PackRead(unsigned int offset, void *buf, unsigned int size);
};

int cFilePackPlatform::PackRead(unsigned int offset, void *buf, unsigned int size) {
    if (mHandle != 0) {
        cFileSystem::SetCurrentPos(mHandle, offset);
        return cFileSystemRead::Read(mHandle, buf, size);
    }
    return 0;
}
