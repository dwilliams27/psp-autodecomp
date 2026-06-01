/* tanf  @ 0x001bbe90  (gMain_psp.obj)
 * fdlibm single-precision tangent.
 */

typedef float f32;

extern f32 __kernel_tanf(f32 x, f32 y, int iy);
extern int __ieee754_rem_pio2f(f32 x, f32 *y);

f32 tanf(f32 x)
{
    f32 y[2];
    int n, ix;

    union { f32 value; int word; } gf_u;
    gf_u.value = x;
    ix = gf_u.word;
    ix &= 0x7fffffff;

    if (ix <= 0x3f490fda) {                 /* |x| < pi/4 */
        return __kernel_tanf(x, 0.0f, 1);
    } else if (ix >= 0x7f800000) {          /* tan(Inf or NaN) is NaN */
        return 0.0f;
    } else {                                /* argument reduction needed */
        n = __ieee754_rem_pio2f(x, y);
        return __kernel_tanf(y[0], y[1], 1 - ((n & 1) << 1));
    }
}
