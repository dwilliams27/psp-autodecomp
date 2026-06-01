// cFileSystem::Read(cFileHandle *, void *, unsigned int) static
// Address: 0x00004ff4  Size: 128B  Obj: cAll_psp.obj
// Symbol: __0fLcFileSystemEReadP6LcFileHandlePvUiT
//
// Split-TU: local class declarations (Read is absent from include/cFileSystem.h
// and adding new method decls to headers is banned).

class cBufferedFile {
public:
    char pad0[0x04];
    unsigned int mBufferSize;
    void *mBufferPtr[2];
    char pad1[0x04];
    int mCurrentBuffer;
    int mBufPos;
    unsigned int mFilePos[2];
    char mPlatform[0x120];
    int mAllocHandle;

    void SetFilePos(unsigned int pos);
    int Read(void *buf, unsigned int size);
};

struct cFileHandle {
    cBufferedFile mBufferedFile;   // 0x00
    unsigned int mDataStart;       // 0x144
    unsigned int mField14C;        // 0x148
    unsigned int mCurrentPos;      // 0x150
    unsigned char mFlag;           // 0x154
};

class cFileSystem {
public:
    static int Read(cFileHandle *handle, void *buf, unsigned int size);
};

int cFileSystem::Read(cFileHandle *handle, void *buf, unsigned int size)
{
    if (size != 0) {
        handle->mBufferedFile.SetFilePos(handle->mCurrentPos);
        int n = handle->mBufferedFile.Read(buf, size);
        handle->mCurrentPos += n;
        return n;
    }
    return 0;
}
