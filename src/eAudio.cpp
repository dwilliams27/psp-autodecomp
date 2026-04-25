#include "eAudioChannel.h"
#include "eAudioGroup.h"

extern char *g_eAudio_channels;
extern char g_eAudio_channelActive[];
extern int g_eAudio_updateCount;

extern "C" int sceSasSetKeyOff(int iVoiceNum);

class eAudioPlatform {
public:
    static void StopStream(int);
};

class eAudio {
public:
    static void Reset();
    static int IsPlayingSound(int);
    static int GetSoundTime(int);
    static void StopChannel(const eAudioChannel *);
    static void PlatformUpdate();
};

void eAudio::Reset() {
    unsigned int addr = 0x40E88;
    int i = 0;
    char *p = (char *)0;
    p = p + addr;
    do {
        ((eAudioGroup *)p)->ResetUserFaders();
        i += 1;
        p += 0x38;
    } while (i < 4);
}

int eAudio::IsPlayingSound(int arg0) {
    return (*(int *)(g_eAudio_channels + arg0 * 0x58) & 3) != 0;
}

int eAudio::GetSoundTime(int arg0) {
    volatile int result;
    char *p = g_eAudio_channels + arg0 * 0x58;
    int flags = *(int *)p & 3;
    flags = (flags != 0) & 0xFF;
    p += 4;
    if (!flags) {
        result = 0;
        return result;
    }
    return *(int *)p;
}

void eAudio::StopChannel(const eAudioChannel *channel) {
    int ch = channel->mField54 - 1;
    if (ch >= 0) {
        if (ch < 32) {
            sceSasSetKeyOff(ch);
        } else {
            eAudioPlatform::StopStream(ch);
        }
        g_eAudio_channelActive[ch] = 0;
    }
}

void eAudio::PlatformUpdate() {
    g_eAudio_updateCount += 1;
}
