// cFileSystemPlatform::WaitAsync(cFilePlatform *) static @ 0x0000F084
// Symbol: __0fTcFileSystemPlatformJWaitAsyncP6NcFilePlatformT

extern "C" int sceIoWaitAsync(int fd, long long *result);
extern "C" int sceKernelChangeThreadPriority(int thid, int priority);
extern "C" int sceKernelRotateThreadReadyQueue(int priority);
extern "C" int sceKernelGetThreadId(void);
extern "C" int sceKernelGetThreadCurrentPriority(void);

class cFilePlatform {
public:
    char pad0[0x108];               // 0x000..0x107
    int  mAsyncFd;                  // 0x108
    char pad1[0x1];                 // 0x10C
    unsigned char mAsyncPending;    // 0x10D
    char mResultFlag;               // 0x10E
    char pad2[0x1];                 // 0x10F
    unsigned int mResult;           // 0x110
};

class cFileSystemPlatform {
public:
    static int WaitAsync(cFilePlatform *f);
};

extern long long gAsyncThreshold;   // 0x36CBB8

int cFileSystemPlatform::WaitAsync(cFilePlatform *f)
{
    long long res;
    int slot[3];

    if (f->mAsyncFd >= 0 && f->mAsyncPending != 0) {
        slot[1] = (0x6F);
        slot[2] = sceKernelGetThreadCurrentPriority();
        slot[0] = sceKernelGetThreadId();
        sceKernelChangeThreadPriority(slot[0], 0x6F);
        sceKernelRotateThreadReadyQueue(slot[1]);
        sceKernelChangeThreadPriority(slot[0], slot[2]);

        if (sceIoWaitAsync(f->mAsyncFd, &res) == 0) {
            char less = res < gAsyncThreshold;
            f->mResultFlag = less;
            f->mResult = less ? 0 : (unsigned int)res;
            f->mAsyncPending = 0;
            return 1;
        }
    }
    return 0;
}
