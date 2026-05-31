/* sinf  @ 0x001bbd7c  (gMain_psp.obj)
 * fdlibm single-precision sine.
 */

typedef float f32;

extern f32 __kernel_sinf(f32 x, f32 y, int iy);
extern f32 __kernel_cosf(f32 x, f32 y);
extern int __ieee754_rem_pio2f(f32 x, f32 *y);

f32 sinf(f32 x)
{
    f32 y[2];
    int n, ix;

    union { f32 value; int word; } gf_u;
    gf_u.value = x;
    ix = gf_u.word;
    ix &= 0x7fffffff;

    if (ix <= 0x3f490fd8) {                 /* |x| < pi/4 */
        return __kernel_sinf(x, 0.0f, 0);
    } else if (ix >= 0x7f800000) {          /* sin(Inf or NaN) is NaN */
        return 0.0f;
    } else {                                /* argument reduction needed */
        n = __ieee754_rem_pio2f(x, y);
        switch (n & 3) {
        case 0:  return  __kernel_sinf(y[0], y[1], 1);
        case 1:  return  __kernel_cosf(y[0], y[1]);
        case 2:  return -__kernel_sinf(y[0], y[1], 1);
        default: return -__kernel_cosf(y[0], y[1]);
        }
    }
}
