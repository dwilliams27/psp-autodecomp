#include "eAudioChannel.h"
#include "eAudioGroup.h"
#include "mOCS.h"

extern char *g_eAudio_channels;
extern char g_eAudio_channelActive[];
extern int g_eAudio_updateCount;
extern void *gSpeakerConfigTable[];

extern "C" int sceSasSetKeyOff(int iVoiceNum);
extern "C" int sceSasSetVolume(int iVoiceNum, int leftVolume, int rightVolume, int effectLeftVolume, int effectRightVolume);
extern "C" int sceSasSetPitch(int iVoiceNum, int pitch);

class eAudioPlatform {
public:
    static void StopStream(int);
    static void SetStreamVolume(int, float);
    static void ComputeVolume(float, const float *, int *, int *);
};

class eAudio {
public:
    static void Reset();
    static int IsPlayingSound(int);
    static int GetSoundTime(int);
    static void StopChannel(const eAudioChannel *);
    static void PlatformUpdate();
    static void SetChannelVolume(const eAudioChannel *, float, int, const mOCS *);
    static void SetChannelFrequency(const eAudioChannel *, float);
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

void eAudio::SetChannelVolume(const eAudioChannel *channel, float volume, int numSpeakers, const mOCS *speakers) {
    int ch = channel->mField54 - 1;
    float panning[4];

    if (ch >= 0) {
        channel->CalcPanning(numSpeakers, speakers, panning);
        if (ch < 32) {
            int left;
            int right;
            eAudioPlatform::ComputeVolume(volume, panning, &left, &right);
            sceSasSetVolume(ch, left, right, 0, 0);
        } else {
            eAudioPlatform::SetStreamVolume(ch, volume);
        }
    }
}

void eAudio::SetChannelFrequency(const eAudioChannel *channel, float frequency) {
    int ch = channel->mField54 - 1;

    if (ch >= 0 && ch < 32) {
        void *entry = 0;
        int config = channel->mSpeakerConfig;
        if (config != 0) {
            entry = gSpeakerConfigTable[config & 0xFFFF];
        }

        float scaled = (float)(int)((float)*(int *)((char *)entry + 0x48) * frequency);
        float pitchFloat = (scaled / 44100.0f) * 4096.0f;
        int pitch;
        if (pitchFloat < 0.0f) {
            pitch = (int)(pitchFloat - 0.5f);
        } else {
            pitch = (int)(pitchFloat + 0.5f);
        }
        sceSasSetPitch(ch, pitch);
    }
}
