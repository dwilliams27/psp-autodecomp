/* cosf  @ 0x001bbb40  (gMain_psp.obj)
 * fdlibm single-precision cosine.
 */

typedef float f32;
typedef int s32;

extern "C" f32 __kernel_cosf(f32 x, f32 y);
extern "C" f32 __kernel_sinf(f32 x, f32 y, s32 iy);
extern "C" s32 __ieee754_rem_pio2f(f32 x, f32 *y);

extern "C" f32 cosf(f32 x)
{
    f32 y[2];
    s32 n, ix;

    union { f32 value; s32 word; } gf_u;
    gf_u.value = x;
    ix = gf_u.word;

    ix &= 0x7fffffff;

    /* |x| ~< pi/4 */
    if (ix <= 0x3f490fd8) return __kernel_cosf(x, 0.0f);

    /* cos(Inf or NaN) is NaN */
    else if (ix >= 0x7f800000) return 0.0f;

    /* argument reduction needed */
    else {
        n = __ieee754_rem_pio2f(x, y);
        switch (n & 3) {
            case 0:  return  __kernel_cosf(y[0], y[1]);
            case 1:  return -__kernel_sinf(y[0], y[1], 1);
            case 2:  return -__kernel_cosf(y[0], y[1]);
            default: return  __kernel_sinf(y[0], y[1], 1);
        }
    }
}
