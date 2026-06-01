// eAudio::PauseAllSounds(void) static
// Address: 0x0001ed94, Size: 128B, Obj: eAll_psp.obj

class cTimeValue {
public:
    int value;
    cTimeValue() : value(0) {}
};

class eAudio {
public:
    static void PauseSound(int channel, cTimeValue time);
    static void PauseAllSounds();
};

extern int g_audioPauseRefCount;   // 0x37D0DC
extern int g_audioNumSounds;       // 0x37D0D0

void eAudio::PauseAllSounds()
{
    if (g_audioPauseRefCount == 0) {
        for (int i = 0; i < g_audioNumSounds; i++) {
            PauseSound(i, cTimeValue());
        }
    }
    g_audioPauseRefCount = g_audioPauseRefCount + 1;
}
