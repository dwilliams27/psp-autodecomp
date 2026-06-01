// mBSpline::Interpolate(const int, const float, const mVec3 *, int, int, mVec3 *) static
// Address: 0x001a2c38, Size: 116B
// Obj: mAll_psp.obj
// Symbol: __0fImBSplineLInterpolateCiCfPC6FmVec3iTEP6FmVec3T
//
// Scalar-t overload: evaluate the 4 basis weights into a stack buffer, then
// forward to the float-pointer Interpolate overload (0x001a2bac).
// Pure forwarder; non-leaf.
//
// ODR-WARNING: minimal local declarations only. Split-TU: declare the class
// locally; do not include shared headers.

struct mVec3 {
    unsigned int qw __attribute__((mode(TI)));
};

class mBSpline {
public:
    static void EvaluateBasis(float t, float *out);
    static void Interpolate(const int n, const float *w, const mVec3 *pts,
                            int count, int stride, mVec3 *out);
    static void Interpolate(const int n, const float t, const mVec3 *pts,
                            int count, int stride, mVec3 *out);
};

void mBSpline::Interpolate(const int n, const float t, const mVec3 *pts,
                           int count, int stride, mVec3 *out) {
    float w[4];
    EvaluateBasis(t, w);
    Interpolate(n, w, pts, count, stride, out);
}
