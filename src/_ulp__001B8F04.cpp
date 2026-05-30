// _ulp  @ 0x001b8f04  (gMain_psp.obj)
// Computes the ulp (unit in the last place) of a double.
// Argument arrives in a0/a1 (soft-float ABI), result returned in v0/v1.

typedef union {
    double d;
    struct {
        int lo;
        int hi;
    } w;
} ieee_double;

extern "C" double _ulp(double x)
{
    ieee_double in;
    ieee_double out;
    in.d = x;

    int e = (in.w.hi & 0x7FF00000) - 0x03400000;
    if (e > 0) {
        out.w.hi = e;
        out.w.lo = 0;
    } else {
        int n = (-e) >> 0x14;
        if (n < 0x14) {
            out.w.hi = 0x80000 >> n;
            out.w.lo = 0;
        } else {
            out.w.hi = 0;
            int m = n - 0x14;
            int v = 1;
            if (m < 0x1F) {
                v = 1 << (0x1F - m);
            }
            out.w.lo = v;
        }
    }
    return out.d;
}
