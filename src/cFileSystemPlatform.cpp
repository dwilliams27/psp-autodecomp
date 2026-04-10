#include "cFileSystemPlatform.h"
#include "cFile.h"

extern "C" int sceKernelExitGame(void);
extern "C" int sceIoClose(int fd);
extern "C" int sceKernelDelayThread(unsigned int usec);

static void (*sSuspendCallback)(void *);
static void *sSuspendCallbackData;
static int sSuspendFlags;

int cFileSystemPlatform::IsCDDvD(void) {
    return 1;
}

void cFileSystemPlatform::SetSuspendCallback(void (*callback)(void *), void *data) {
    sSuspendCallback = callback;
    sSuspendCallbackData = data;
}

void cFileSystemPlatform::ReplaceChar(char *str, char from, char to) {
    char *p = str;
    char c = *p;
    if (c != 0) {
        do {
            if (c == from) {
                *p = to;
            }
            p++;
            c = *p;
        } while (c != 0);
    }
}

int cFileSystemPlatform::ExitCallback(int, int, void *) {
    sceKernelExitGame();
    return 0;
}

void cFileSystemPlatform::Close(cFilePlatform *fp) {
    if (fp->mHandle >= 0) {
        cFileSystemPlatform::WaitAsync(fp);
        sceIoClose(fp->mHandle);
        fp->mHandle = -1;
    }
}

void cFileSystemPlatform::WaitSuspend(void) {
    int suspended = ((sSuspendFlags & 4) != 0) & 0xFF;
    if (suspended) {
        do {
            sceKernelDelayThread(1000000);
            suspended = ((sSuspendFlags & 4) != 0) & 0xFF;
        } while (suspended);
    }
}

void eMoviePlatform::read_delete(void) {
}

void eSoundData::CreateData(void) {
}

void eBspTree::PlatformFree(void) {
}

void eBspTree::SetHullData(int, const mVec3 *, int, const unsigned short *, unsigned int) {
}
