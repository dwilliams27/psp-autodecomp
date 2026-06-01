// eMoviePlatform::soundbuf_thread(void) @ 0x000130b0
class eMoviePlatform {
public:
    int soundbuf_thread();
    void control_waitConditionSoundStart();
    int soundbuf_checkDecodeEnd();
    void soundbuf_swapbuf(char *);
    void control_setCondition(unsigned int);
};

extern "C" {
    int sceWaveAudioGetRestSample(int);
    void sceKernelDelayThread(unsigned int);
    void sceKernelExitThread(int);
    int sceKernelSignalSema(int, int);
    int sceKernelWaitSema(int, int, void *);
}

int eMoviePlatform::soundbuf_thread() {
    control_waitConditionSoundStart();

    for (;;) {
        int end = soundbuf_checkDecodeEnd();
        int n = *(int *)((char *)this + 0x330);
        if (end != 0 && n == 0) {
            break;
        }
        if (n > 0) {
            int idx = *(int *)((char *)this + 0x32C);
            soundbuf_swapbuf(*(char **)((char *)this + idx * 4 + 0x308));
            sceKernelWaitSema(*(int *)((char *)this + 0x304), 1, 0);
            *(int *)((char *)this + 0x330) = *(int *)((char *)this + 0x330) - 1;
            sceKernelSignalSema(*(int *)((char *)this + 0x304), 1);
            *(int *)((char *)this + 0x344) = *(int *)((char *)this + 0x344) + 1;
        } else {
            int d = *(int *)((char *)this + 0x348) + 1;
            *(int *)((char *)this + 0x348) = d;
            sceKernelDelayThread(0x3E8);
        }
    }

    while (sceWaveAudioGetRestSample(2) != 0) {
        sceKernelDelayThread(0x3E8);
    }
    control_setCondition(0x20);
    sceKernelExitThread(0);
    return 0;
}
