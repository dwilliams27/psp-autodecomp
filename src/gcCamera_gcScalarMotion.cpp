// gcCamera::gcScalarMotion — gcAll_psp.obj
//   0x000fbd30  Write(cOutStream &) const   (104B)
//   0x000fbd98  Read(cInStream &)           (104B)
//
// Layout: three floats (mF0, mF4, mF8) followed by an embedded
// gcCamera::gcMotionLimits at offset 0x0C.

#include "cOutStream.h"

extern "C" float fabsf(float);

class cInStream {
public:
    void Read(float &, bool);
};

class cTimeValue {
public:
    int mTime;
};

struct gcCamera {
    struct gcMotionLimits {
        void Write(cOutStream &) const;
        void Read(cInStream &);
    };

    struct gcScalarMotion {
        float           mF0;
        float           mF4;
        float           mF8;
        gcMotionLimits  mLimits;

        void Write(cOutStream &) const;
        void Read(cInStream &);
        void Set(float, cTimeValue);
    };
};

void gcCamera::gcScalarMotion::Write(cOutStream &s) const {
    s.Write(mF0, true);
    s.Write(mF4, true);
    s.Write(mF8, true);
    mLimits.Write(s);
}

void gcCamera::gcScalarMotion::Read(cInStream &s) {
    s.Read(mF0, true);
    s.Read(mF4, true);
    s.Read(mF8, true);
    mLimits.Read(s);
}

void gcCamera::gcScalarMotion::Set(float value, cTimeValue time) {
    int ticks = time.mTime;
    if (ticks == 0) {
        mF4 = value;
        mF0 = value;
        mF8 = 0.0f;
    } else {
        float start = mF0;
        mF4 = value;
        *(float *)((char *)this + 0x14) =
            fabsf(value - start) / (*(float *)0x0036C800 * (float)ticks);
    }
}
