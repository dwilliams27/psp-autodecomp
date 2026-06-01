// gcCamera::gcMotionLimits::Read(cInStream &) @ 0x000fbae0  92B  gcAll_psp.obj
// Symbol: __0f5IgcCameraOgcMotionLimitsEReadR6JcInStream
//
// Split-TU: gcCamera::gcMotionLimits is not declared in any header, so the
// class is declared locally here (mirrors the sibling
// gcCamera::gcXYZMotion::Read split-TU). gcMotionLimits holds three floats
// at offsets 0x00, 0x04, 0x08, each read straight-line via
// cInStream::Read(float&, bool=true).

class cInStream {
public:
    void Read(float &, bool);
};

struct gcCamera {
    struct gcMotionLimits {
        float a;    // 0x00
        float b;    // 0x04
        float c;    // 0x08

        void Read(cInStream &);
    };
};

void gcCamera::gcMotionLimits::Read(cInStream &s) {
    s.Read(a, true);
    s.Read(b, true);
    s.Read(c, true);
}
