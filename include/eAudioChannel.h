#pragma once

class cMemPool;
class cTimeValue;
class mOCS;

// eAudioChannel: audio channel with panning, volume, and fade state.
// Default ctor (0x1dfc9c) inits:
//   +0x00: int = 0           (flags)
//   +0x04: int = 0           (handle/id)
//   +0x08: byte = 0          (state flag)
//   +0x0C: float = 1.0       (volume)
//   +0x10: float = 1.0       (pitch)
//   +0x14: int = 0
//   +0x18: float = 1.0       (pan left)
//   +0x1C: float = 1.0       (pan right)
//   +0x20: float = 0.0       (float field)
//   +0x24: float = 1.0       (distance scale)
//   +0x28..0x2B: pad
//   +0x2C: byte = 0          (flag)
//   +0x30: float = 1.0       (min dist)
//   +0x34: float = 1.0       (max dist)
//   +0x38: int = 0
//   +0x3C: byte = 0          (flag)
//   +0x40: float = 1.0
//   +0x44: float = 1.0
//   +0x48: int = 0
//   +0x4C: int = 0            (sound ptr/handle)
//   +0x50: int = 0            (speaker config / channel mask)
//   +0x54: int = 0
// CalcPanning (0x1d224) reads:
//   +0x00: flags (int, bit 0x80 = 3D)
//   +0x4C: listener pointer/handle
//   +0x50: speaker config (int, low 16 bits = speaker count)
// Struct size: at least 0x58 (88 bytes)
struct eAudioChannel {
    int mFlags;                 // 0x00
    int mHandle;                // 0x04
    unsigned char mState;       // 0x08
    char _pad09[3];             // 0x09
    float mVolume;              // 0x0C
    float mPitch;               // 0x10
    int mField14;               // 0x14
    float mPanLeft;             // 0x18
    float mPanRight;            // 0x1C
    float mField20;             // 0x20
    float mDistanceScale;       // 0x24
    char _pad28[4];             // 0x28
    unsigned char mFlag2C;      // 0x2C
    char _pad2D[3];             // 0x2D
    float mMinDist;             // 0x30
    float mMaxDist;             // 0x34
    int mField38;               // 0x38
    unsigned char mFlag3C;      // 0x3C
    char _pad3D[3];             // 0x3D
    float mField40;             // 0x40
    float mField44;             // 0x44
    int mField48;               // 0x48
    int mListenerPtr;           // 0x4C
    int mSpeakerConfig;         // 0x50
    int mField54;               // 0x54

    eAudioChannel();
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void CalcPanning(int, const mOCS *, float *) const;
    void Update(cTimeValue, float, int, const mOCS *);
};
