// eAudio::ResumeAllSounds(void) static
// Address: 0x0001ee88, Size: 116B, Obj: eAll_psp.obj

class cTimeValue {
public:
    int value;
    cTimeValue() : value(0) {}
};

class eAudio {
public:
    static void ResumeSound(int channel, cTimeValue time);
    static void ResumeAllSounds();
};

extern int g_audioPauseRefCount;   // 0x37D0DC
extern int g_audioNumSounds;       // 0x37D0D0

void eAudio::ResumeAllSounds()
{
    if (--g_audioPauseRefCount == 0) {
        for (int i = 0; i < g_audioNumSounds; i++) {
            ResumeSound(i, cTimeValue());
        }
    }
}
