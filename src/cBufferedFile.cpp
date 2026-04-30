class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cBufferedFile_DelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);
int sceKernelCreateFpl(const char *, unsigned int, unsigned int, unsigned int,
                       unsigned int, void *);
void sceKernelAllocateFpl(int, void *, unsigned int);
int sceKernelDeleteFpl(int);
int sceKernelFreeFpl(int, void *);

class cFilePlatform {
public:
    char _pad[0x104];
    int mSize;
    int mHandle;
    char _pad2[0x14];

    ~cFilePlatform(void);
};

extern "C" void __0oNcFilePlatformctv(cFilePlatform *);

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

    cBufferedFile(unsigned int shift);
    ~cBufferedFile(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            cBufferedFile_DelRec *rec =
                (cBufferedFile_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
    void Close(void);
    bool IsOpen(void) const;
};

cBufferedFile::cBufferedFile(unsigned int shift) {
    *(unsigned int *)((char *)this + 0) = shift;
    mBufferSize = 1 << shift;
    *(int *)((char *)this + 0x10) = 0;
    mCurrentBuffer = 0;
    mBufPos = 0;
    __0oNcFilePlatformctv(&mPlatform);
    mAllocHandle = -1;

    unsigned int opts[2];
    opts[0] = 8;
    opts[1] = 0x40;
    int handle = sceKernelCreateFpl((const char *)0x36CB74, 2, 0, mBufferSize,
                                    2, opts);
    register int allocHandle asm("a0");
    __asm__ volatile("move %0, %1" : "=r"(allocHandle) : "r"(handle));
    sceKernelAllocateFpl(allocHandle,
                         (*(int *)((char *)this + 0x144) = handle,
                          (char *)this + 8),
                         0);
    sceKernelAllocateFpl(mAllocHandle, &mBufferPtr[1], 0);
    mFilePos[0] = 0;
    mFilePos[1] = 0;
}

bool cBufferedFile::IsOpen(void) const {
    return mPlatform.mHandle >= 0;
}

cBufferedFile::~cBufferedFile(void) {
    Close();
    sceKernelFreeFpl(mAllocHandle, mBufferPtr[0]);
    sceKernelFreeFpl(mAllocHandle, mBufferPtr[1]);
    sceKernelDeleteFpl(mAllocHandle);
}
