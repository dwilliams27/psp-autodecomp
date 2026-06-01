// eCameraEffectMgr::Update(cTimeValue) @ 0x0002d6d8, 192B, eAll_psp.obj
// Symbol: __0fQeCameraEffectMgrGUpdate6KcTimeValue
//
// Split-TU: the class is declared locally because Update is missing from
// src/eCameraEffectMgr.cpp (that TU defines Write/Read/EnableEffect only, so
// there is no duplicate symbol).
//
// Two codegen-critical idioms:
//   * The scale constant at 0x0036C800 is referenced as a named extern global
//     so SNC emits `lui %hi / lwc1 %lo(reg)` with the low half folded into the
//     load offset (matching the original). A raw `*(float*)0x36C800` literal
//     makes SNC materialize the full address with a separate `addiu`. The two
//     relocation words are masked by compare_func.
//   * The 0/1 clamp is written as a nested ternary so SNC promotes the upper
//     bound to a branch-likely (`bc1fl` with `mov.s` in the delay slot) and
//     colors the working value into $f13. An if/else-if chain instead emits a
//     regular `bc1t` + nop (one instruction longer) with swapped registers.

#pragma control sched=2

class cInStream;
class cOutStream;
class eCameraEffect;

extern const float g_eCameraEffectScale;  // 0x0036C800

template <class T>
class cHandleT {
public:
    int mIndex;
    bool operator!=(const cHandleT &other) const { return mIndex != other.mIndex; }
};

class cTimeValue {
public:
    int mTime;
};

#include "eAudioFader.h"

struct eCameraEffectMgrEntry {
    eAudioFader mFader;                  // 0x00..0x0F
    cHandleT<eCameraEffect> mHandle;     // 0x10..0x13
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

class eCameraEffectMgr {
public:
    char _pad[16];                            // 0x00..0x0F
    eCameraEffectMgrEntry mEntries[8];        // 0x10..0xAF

    void Update(cTimeValue t);
};

void eCameraEffectMgr::Update(cTimeValue t) {
    eCameraEffectMgrEntry *e = mEntries;
    int i = 0;
    do {
        if (e->mFader.mActive) {
            int dur = e->mFader.mDuration;
            if (dur) {
                float k = g_eCameraEffectScale;
                float v = (((k * (float)t.mTime) / (k * (float)dur)) * e->mFader.mTarget)
                          + e->mFader.mCurrent;
                v = v <= 0.0f ? 0.0f : (!(v < 1.0f) ? 1.0f : v);
                e->mFader.mCurrent = v;
                if (v <= 0.0f) {
                    e->mFader.mActive = 0;
                }
            }
        }
        i++;
        e++;
    } while (i < 8);
}
