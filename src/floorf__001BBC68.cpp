// floorf @ 0x001bbc68 (gMain_psp.obj) — newlib/fdlibm floorf
// Byte-exact reconstruction of the standard libm implementation.

typedef int s32;
typedef unsigned int u32;
typedef float f32;

static const f32 huge = 1.0e30f;

extern "C" f32 floorf(f32 x)
{
    s32 i0, j0;
    u32 i, ix;

    union { f32 f; s32 i; } gf;
    gf.f = x;
    i0 = gf.i;

    j0 = ((i0 >> 23) & 0xff) - 0x7f;
    if (j0 < 23) {
        if (j0 < 0) {
            /* raise inexact if x != 0 */
            if (huge + x > (f32)0.0) {
                if (i0 >= 0) {
                    i0 = 0;
                } else if ((i0 & 0x7fffffff) != 0) {
                    i0 = (s32)0xbf800000;
                }
            }
        } else {
            i = (0x007fffff) >> j0;
            if ((i0 & i) == 0) return x;  /* x is integral */
            if (huge + x > (f32)0.0) {    /* raise inexact flag */
                if (i0 < 0) i0 += (0x00800000) >> j0;
                i0 &= (~i);
            }
        }
    } else {
        if (j0 == 0x80) return x + x;   /* inf or NaN */
        else return x;                   /* x is integral */
    }
    union { s32 i; f32 f; } sf;
    sf.i = i0;
    return sf.f;
}
