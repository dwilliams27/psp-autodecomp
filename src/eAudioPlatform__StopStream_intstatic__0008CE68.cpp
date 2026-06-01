// eAudioPlatform::StopStream(int) static
// Address: 0x0008ce68, Size: 116B
// Obj: eAll_psp.obj
// Symbol: __0fOeAudioPlatformKStopStreamiT

struct eAudioPlatform_eStreamRequest {
    char  cmd;          // +0
    char  pad0[255];
    int   field256;     // +256
    int   field260;     // +260
    int   id;           // +264
    int   field268;     // +268
    int   field272;     // +272
};

class eAudioPlatform {
public:
    class eStreamThread {
    public:
        void RequestStream(const eAudioPlatform_eStreamRequest &req);
    };

    char  pad0[5];
    char  stopFlag;     // +5
    // ... eStreamThread members continue at offset 0 (this == eStreamThread)

    static void StopStream(int id);
};

#define AUDIO_STREAM_BASE 0x47658
#define AUDIO_STREAM_STRIDE 0x283C0

#pragma control sched=1
void eAudioPlatform::StopStream(int id)
{
    int idx = id - 32;
    __asm__ volatile("" ::: "memory");
    eAudioPlatform::eStreamThread *thread =
        (eAudioPlatform::eStreamThread *)(idx * AUDIO_STREAM_STRIDE + AUDIO_STREAM_BASE);

    if (id == *(int *)((char *)thread + 560)) {
        *((char *)thread + 5) = 1;
        return;
    }

    eAudioPlatform_eStreamRequest req;
    req.cmd = 0;
    req.field256 = 0;
    __asm__ volatile("" ::: "memory");
    req.id = -1;
    req.field268 = 0;
    req.field272 = 0;
    thread->RequestStream(req);
}
