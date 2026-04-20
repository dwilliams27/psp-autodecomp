class cTimeValue {
public:
    int mTime;
};

template <class T> class cHandleT {
public:
    int mHandle;
};

class eSoundData;
class mOCS;

#include "eAudioGroup.h"

extern "C" {
    extern char *g_eAudio_channels;      // at 0x37D0D4 (eAudio::s_channels)
    extern float g_eAudio_masterVolume;  // at 0x37D0CC (eAudio::s_fMasterVolume)
    void eAudio__StopSound_int_cTimeValuestatic(int idx, cTimeValue dt);
    void eAudioChannel__Update(void *ch, cTimeValue dt, float vol, int a2, const mOCS *a3);
}

// ----------------------------------------------------------------------
// eAudioGroup::GetVolume() const @ 0x0001d6c0 (48 bytes)
// ----------------------------------------------------------------------
float eAudioGroup::GetVolume() const {
    float v = 1.0f;
    for (int i = 0; i < 3; i++) {
        v *= mFaders[i].mCurrent;
    }
    return v;
}

// ----------------------------------------------------------------------
// eAudioGroup::eAudioGroup() @ 0x001dfd04 (72 bytes)
// ----------------------------------------------------------------------
eAudioGroup::eAudioGroup() : mChannelStart(0), mChannelEnd(-1) {
}

// ----------------------------------------------------------------------
// eAudioGroup::ResetUserFaders() @ 0x0001d420 (136 bytes)
// ----------------------------------------------------------------------
void eAudioGroup::ResetUserFaders() {
    cTimeValue dt;
    dt.mTime = 0;
    int dummy0 = 0;
    int dummy1 = 0;
    for (int i = 1; i < 3; i++) {
        eAudioFader &f = mFaders[i];
        f.mTarget = 1.0f;
        int dur;
        if (f.mCurrent < f.mTarget) {
            dur = dt.mTime;
        } else {
            dur = -dt.mTime;
        }
        f.mDuration = dur;
        f.mActive = 1;
        if (dt.mTime == 0) {
            f.mCurrent = f.mTarget;
        }
    }
    (void)dummy0;
    (void)dummy1;
}

// ----------------------------------------------------------------------
// eAudioGroup::StopSound(cHandleT<eSoundData>, cTimeValue) @ 0x0001d6f0 (216 bytes)
// ----------------------------------------------------------------------
void eAudioGroup::StopSound(cHandleT<eSoundData> handle, cTimeValue dt) {
    int idx = mChannelStart;
    if (mChannelEnd >= idx) {
        int offs = idx * 0x58;
        do {
            char *ch = g_eAudio_channels + offs;
            bool active = ((*(int *)ch & 3) != 0);
            if (active) {
                bool match = (handle.mHandle == *(int *)(ch + 0x50));
                if (match) {
                    eAudio__StopSound_int_cTimeValuestatic(idx, dt);
                }
            }
            idx++;
            offs += 0x58;
        } while (mChannelEnd >= idx);
    }
}

// ----------------------------------------------------------------------
// eAudioGroup::Update(cTimeValue, int, const mOCS *) @ 0x0001d4a8 (248 bytes)
// ----------------------------------------------------------------------
void eAudioGroup::Update(cTimeValue dt, int arg2, const mOCS *arg3) {
    int i = 0;
    eAudioFader *f = mFaders;
    do {
        f->Update(dt);
        i++;
        f++;
    } while (i < 3);

    float volume = g_eAudio_masterVolume * GetVolume();
    int idx = mChannelStart;
    if (mChannelEnd >= idx) {
        int offs = idx * 0x58;
        do {
            eAudioChannel__Update(g_eAudio_channels + offs, dt, volume, arg2, arg3);
            idx++;
            offs += 0x58;
        } while (mChannelEnd >= idx);
    }
}
