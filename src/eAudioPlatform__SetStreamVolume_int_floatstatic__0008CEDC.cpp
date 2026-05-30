// eAudioPlatform::SetStreamVolume(int, float) static
// Address: 0x0008cedc, Size: 76B
// Obj: eAll_psp.obj
// Symbol: __0fOeAudioPlatformPSetStreamVolumeifT

struct AudioStreamSlot {
    char pad0[12];
    int volume;       // +12
    char pad1[560 - 16];
    int id;           // +560
};

class eAudioPlatform {
public:
    static void SetStreamVolume(int id, float vol);
};

#pragma control sched=1
void eAudioPlatform::SetStreamVolume(int id, float vol)
{
    int idx = id - 32;
    __asm__ volatile("" ::: "memory");
    AudioStreamSlot *slot = (AudioStreamSlot *)(idx * 0x283C0 + 0x47658);
    if (id == slot->id) {
        slot->volume = (int)(32768.0f * vol);
    }
}
