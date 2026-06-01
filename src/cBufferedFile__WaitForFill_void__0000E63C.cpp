// cBufferedFile::WaitForFill(void) @ 0x0000e63c, cAll_psp.obj
// Split-TU: classes declared locally to avoid editing shared header.

extern "C" int sceKernelDelayThread(unsigned int usec);

class cFilePlatform {
public:
    char _pad[0x104];
    int mSize;
    int mHandle;
    char _pad2[0x2];
    unsigned char mPending;
    char _pad3[0x11];

    void ReadRetry(void);
    void WaitAsync(void);
};

class cBufferedFile {
public:
    char pad0[0x04];
    unsigned int mBufferSize;
    void *mBufferPtr[2];
    char pad1[0x04];
    int mCurrentBuffer;
    int mBufPos;
    unsigned int mFilePos[2];
    cFilePlatform mPlatform;
    int mAllocHandle;

    void WaitForFill(void);
};

void cBufferedFile::WaitForFill(void) {
    mPlatform.WaitAsync();
    while (mPlatform.mPending != 0) {
        sceKernelDelayThread(0xF4240);
        mPlatform.ReadRetry();
        mPlatform.WaitAsync();
    }
}
