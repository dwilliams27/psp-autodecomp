#pragma once

class cTimeValue;
class cOutStream;
class cInStream;

// eAudioFader: small struct for audio fade control.
// Default ctor (0x1dfc7c) inits:
//   +0x00: byte = 0     (active flag)
//   +0x04: float = 1.0  (current value)
//   +0x08: float = 1.0  (target value)
//   +0x0C: int = 0      (duration, in time units; 0 = instant)
// Update (0x1ce74) reads:
//   +0x00: active flag (byte)
//   +0x04: current value (float)
//   +0x08: target value (float)
//   +0x0C: duration (int, negative = fade down from 1-target)
// Struct size: 0x10 (16 bytes)
struct eAudioFader {
    unsigned char mActive;      // 0x00
    char _pad01[3];             // 0x01
    float mCurrent;             // 0x04
    float mTarget;              // 0x08
    int mDuration;              // 0x0C

    eAudioFader();
    int Update(cTimeValue);
    void Write(cOutStream &) const;
    void Read(cInStream &);
};
