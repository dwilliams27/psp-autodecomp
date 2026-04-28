// gcCamera::gcScalarMotion — gcAll_psp.obj
//   0x000fbd30  Write(cOutStream &) const   (104B)
//   0x000fbd98  Read(cInStream &)           (104B)
//
// Layout: three floats (mF0, mF4, mF8) followed by an embedded
// gcCamera::gcMotionLimits at offset 0x0C.

#include "cOutStream.h"

class cInStream {
public:
    void Read(float &, bool);
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
