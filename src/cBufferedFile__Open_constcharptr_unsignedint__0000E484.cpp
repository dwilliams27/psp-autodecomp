// cBufferedFile::Open(const char *, unsigned int) @ 0x0000e484, cAll_psp.obj
// Split-TU: classes declared locally to avoid editing shared header.

extern "C" int sceKernelDelayThread(unsigned int usec);

class cThread {
public:
    static bool InPrimary(void);
};

class cFilePlatform {
public:
    char _pad[0x104];
    int mSize;
    int mHandle;
    char _pad2[0x14];

    bool Open(const char *name);
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

    bool Open(const char *name, unsigned int pos);
    void Reset(unsigned int pos);
    bool IsOpen(void) const { return mPlatform.mHandle >= 0; }
};

bool cBufferedFile::Open(const char *name, unsigned int pos) {
    while (!mPlatform.Open(name) && !cThread::InPrimary()) {
        sceKernelDelayThread(0xF4240);
    }
    if (IsOpen()) {
        Reset(pos);
        return true;
    }
    return false;
}
