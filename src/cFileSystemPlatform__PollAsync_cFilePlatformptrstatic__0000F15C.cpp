// cFileSystemPlatform::PollAsync(cFilePlatform *) static @ 0x0000F15C
// Symbol: __0fTcFileSystemPlatformJPollAsyncP6NcFilePlatformT

extern "C" int sceIoPollAsync(int fd, long long *result);

class cFilePlatform {
public:
    char pad0[0x108];       // 0x000..0x107
    int  mAsyncFd;          // 0x108
    char pad1[0x1];         // 0x10C
    unsigned char mAsyncPending; // 0x10D
    char mResultFlag;       // 0x10E
    char pad2[0x1];         // 0x10F
    unsigned int mResult;   // 0x110
};

class cFileSystemPlatform {
public:
    static int PollAsync(cFilePlatform *f);
};

extern long long gAsyncThreshold;   // 0x36CBB8

int cFileSystemPlatform::PollAsync(cFilePlatform *f)
{
    long long res;

    if (f->mAsyncFd >= 0 && f->mAsyncPending != 0 &&
        sceIoPollAsync(f->mAsyncFd, &res) == 0) {
        char less = res < gAsyncThreshold;
        f->mResultFlag = less;
        f->mResult = less ? 0 : (unsigned int)res;
        f->mAsyncPending = 0;
        return 1;
    }
    return 0;
}
