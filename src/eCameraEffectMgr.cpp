// -----------------------------------------------------------------------------
// eCameraEffectMgr methods (eAll_psp.obj)
// -----------------------------------------------------------------------------

class cInStream;
class cOutStream;
class eCameraEffect;

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

    void EnableEffect(int idx, bool enable, cTimeValue dur, cHandleT<eCameraEffect> h);
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

// -----------------------------------------------------------------------------
// eCameraEffectMgr::Write(cOutStream &) const  @ 0x0002d7bc (84B)
// -----------------------------------------------------------------------------
void eCameraEffectMgr::Write(cOutStream &s) const {
    int i = 0;
    eCameraEffectMgrEntry *e = (eCameraEffectMgrEntry *)mEntries;
    do {
        e->Write(s);
        i++;
        e++;
    } while (i < 8);
}

// -----------------------------------------------------------------------------
// eCameraEffectMgr::Read(cInStream &)  @ 0x0002d810 (84B)
// -----------------------------------------------------------------------------
void eCameraEffectMgr::Read(cInStream &s) {
    int i = 0;
    eCameraEffectMgrEntry *e = (eCameraEffectMgrEntry *)mEntries;
    do {
        e->Read(s);
        i++;
        e++;
    } while (i < 8);
}

// -----------------------------------------------------------------------------
// eCameraEffectMgr::EnableEffect(int, bool, cTimeValue, cHandleT<eCameraEffect>)
//   @ 0x0002d620 (184B)
// -----------------------------------------------------------------------------
void eCameraEffectMgr::EnableEffect(int idx, bool enable, cTimeValue dur,
                                    cHandleT<eCameraEffect> h) {
    if (idx < 0 || idx >= 8) return;
    int *durPtr = &mEntries[idx].mFader.mDuration;
    if (enable) {
        cHandleT<eCameraEffect> *hp = &mEntries[idx].mHandle;
        if (*hp != h) {
            mEntries[idx].mFader.mCurrent = 0.0f;
        }
        mEntries[idx].mFader.mTarget = 1.0f;
        *durPtr = dur.mTime;
        mEntries[idx].mFader.mActive = 1;
        *hp = h;
        if (dur.mTime == 0) {
            mEntries[idx].mFader.mCurrent = 1.0f;
        }
    } else {
        mEntries[idx].mFader.mTarget = -1.0f;
        *durPtr = dur.mTime;
        if (dur.mTime == 0) {
            mEntries[idx].mFader.mActive = 0;
            mEntries[idx].mFader.mCurrent = 0.0f;
        }
    }
}
