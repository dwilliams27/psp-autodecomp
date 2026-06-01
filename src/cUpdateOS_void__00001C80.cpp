// cUpdateOS(void) @ 0x00001c80, cAll_psp.obj
// Polling/yield wrapper: pumps suspend wait, kernel callbacks, file-status
// dispatch, then computes an "OS ready" boolean.

typedef unsigned char u8;
typedef signed int s32;

extern void cFileSystemPlatform__WaitSuspend();
extern s32 cFileSystem__GetFileStatus();
extern s32 cIsAppRestarting(u8);
extern "C" int sceKernelCheckCallback(void);

typedef void (*FileStatusHandler)(s32, s32, void *);

extern unsigned char gAppQuitFlag;

s32 cUpdateOS()
{
    cFileSystemPlatform__WaitSuspend();
    sceKernelCheckCallback();

    s32 status = cFileSystem__GetFileStatus();
    if (status != 5) {
        FileStatusHandler handler = *(FileStatusHandler *)0x37C054;
        if (handler != 0) {
            handler(status, status, (void *)handler);
        }
    }

    u8 quit = *(u8 *)0x37BC18;
    s32 ready = 0;
    if (quit == 0 && cIsAppRestarting(quit) == 0) {
        ready = 1;
    }
    return ready & 0xFF;
}
