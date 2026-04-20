#pragma once

#include "eAudioFader.h"

class cTimeValue;
class mOCS;
template <class T> class cHandleT;
class eSoundData;

// eAudioGroup: owns 3 eAudioFaders and spans a range of channel indices.
// +0x00: int   mChannelStart   first channel index (init 0)
// +0x04: int   mChannelEnd     last channel index  (init -1)
// +0x08: eAudioFader mFaders[3]  (3*16 = 48 bytes)  last fader ends at 0x38
struct eAudioGroup {
    int mChannelStart;          // 0x00
    int mChannelEnd;            // 0x04
    eAudioFader mFaders[3];     // 0x08..0x37

    eAudioGroup();
    float GetVolume() const;
    void ResetUserFaders();
    void StopSound(cHandleT<eSoundData> handle, cTimeValue dt);
    void Update(cTimeValue dt, int arg2, const mOCS *arg3);
};
