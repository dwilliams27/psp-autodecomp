// eAudioPlatform::eStreamThread::eStreamThread(void)
// Address: 0x0008d084, Size: 192B
// Obj: eAll_psp.obj
// Symbol: __0o5OeAudioPlatformNeStreamThreadctv

extern "C" {
    void *memset(void *, int, unsigned int);
    int sceKernelCreateMutex(const char *name, int attr, int initCount, void *option);
    // cFilePlatform default ctor, called unconditionally on embedded member
    // at +0x23C. Relocation is masked by compare_func; only the call form
    // (jal + addiu a0,s0,572) is byte-significant.
    void cFilePlatform_ctor(void *self);
}

class eAudioPlatform {
public:
    class eStreamThread {
    public:
        int            f0;        // +0
        char           f4;        // +4
        char           f5;        // +5
        char           pad6[2];
        int            f8;        // +8
        int            fC;        // +12
        int            f10;       // +16  mutex
        char           f14;       // +20
        char           pad15[255];
        int            f114;      // +276
        char           pad118[4];
        int            f11C;      // +284
        int            f120;      // +288
        int            f124;      // +292
        char           f128;      // +296
        char           pad129[255];
        int            f228;      // +552
        char           pad22C[4];
        int            f230;      // +560
        int            f234;      // +564
        int            f238;      // +568
        char           file[288];     // +572 (0x23C) embedded cFilePlatform
        int            f35C;      // +860
        char           pad360[32];
        char           bufA[0x8000];  // +896 (0x380)
        char           bufB[0x20002]; // +0x8380

        eStreamThread();
    };
};

#pragma control sched=1
eAudioPlatform::eStreamThread::eStreamThread()
{
    f0 = -1;
    f4 = 0;
    f5 = 0;
    __asm__ volatile("" ::: "memory");
    f8 = 1;
    fC = 0;
    f10 = sceKernelCreateMutex("eAudioPlatform::eStreamThread", 0, 0, 0);
    f14 = 0;
    f114 = 0;
    f11C = -1;
    f120 = 0;
    f124 = 0;
    f128 = 0;
    f228 = 0;
    f230 = -1;
    f234 = 0;
    f238 = 0;
    cFilePlatform_ctor(file);
    f35C = -1;
    memset(bufA, 0, 0x7FFC);
    memset(bufB, 0, 0x20002);
}
