class cTimeValue {
public:
    int mTime;
};

#include "eAudioFader.h"

eAudioFader::eAudioFader() {
    mActive = 0;
    mCurrent = 1.0f;
    mTarget = 1.0f;
    mDuration = 0;
}

int eAudioFader::Update(cTimeValue dt) {
    if (!mActive) return 0;

    int duration = mDuration;
    float target = mTarget;

    if (duration == 0) {
        mCurrent = target;
        mActive = 0;
        return 1;
    }

    float ratio = (float)dt.mTime / (float)duration;
    float current = mCurrent;

    if (duration < 0) {
        float result = current + (1.0f - target) * ratio;
        mCurrent = result;
        if (result <= target) {
            mCurrent = target;
            mActive = 0;
            return 1;
        }
    } else {
        float delta = target;
        float result = current + (delta * ratio);
        mCurrent = result;
        if (!(result < target)) {
            mCurrent = target;
            mActive = 0;
            return 1;
        }
    }
    return 0;
}
