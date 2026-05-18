// eAudioPlatform::eStreamThread::CloseAtracFile(void) @ 0x0008d40c

extern "C" int sceAtracReleaseAtracID(int);

class cFilePlatform {
public:
    void Close(void);
};

namespace eAudioPlatform {

class eStreamThread {
public:
    void CloseAtracFile(void);
};

#pragma control sched=1

void eStreamThread::CloseAtracFile(void) {
    ((cFilePlatform *)((char *)this + 0x23C))->Close();
    int atracId = *(int *)((char *)this + 0x35C);
    if (atracId >= 0) {
        sceAtracReleaseAtracID(atracId);
        *(int *)((char *)this + 0x35C) = -1;
    }

    struct AtracInfo {
        char b0;
        char _pad[255];
        int f256;
        int f260;
        int f264;
        int f268;
        int f272;
    } local;
    local.b0 = 0;
    local.f256 = 0;
    local.f264 = -1;
    __asm__ volatile("" ::: "memory");
    local.f268 = 0;
    local.f272 = 0;
    *(AtracInfo *)((char *)this + 0x128) = local;
}

}  // namespace eAudioPlatform
