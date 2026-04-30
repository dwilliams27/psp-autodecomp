#include "cFile.h"

int sceKernelCreateFpl(const char *, unsigned int, unsigned int, unsigned int,
                       unsigned int, void *);
void sceKernelAllocateFpl(int, void *, unsigned int);
extern "C" void __0oNcFilePlatformctv(cFilePlatform *);

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
