/* __kernel_sinf  @ 0x001c02f4  (gMain_psp.obj)
 * fdlibm kernel sine, single precision.  __kernel_sinf(x, y, iy)
 */

typedef float f32;

static const f32 S1 = -1.6666667163e-01f; /* 0xbe2aaaab */
static const f32 S2 =  8.3333337680e-03f; /* 0x3c088889 */
static const f32 S3 = -1.9841270114e-04f; /* 0xb9500d01 */
static const f32 S4 =  2.7557314297e-06f; /* 0x3638ef1b */
static const f32 S5 = -2.5050759689e-08f; /* 0xb2d72f34 */
static const f32 S6 =  1.5896910177e-10f; /* 0x2f2ec9d3 */
static const f32 half = 5.0000000000e-01f;

f32 __kernel_sinf(f32 x, f32 y, int iy)
{
    f32 z, r, v;
    int ix;

    union { f32 value; int word; } gf_u;
    gf_u.value = x;
    ix = gf_u.word;
    ix &= 0x7fffffff;                       /* high word of x */
    if (ix < 0x32000000) {                  /* |x| < 2**-27 */
        if ((int)x == 0) return x;          /* generate inexact */
    }
    z = x * x;
    v = z * x;
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (iy == 0) return x + v * (S1 + z * r);
    return x - ((z * (half * y - v * r) - y) - v * S1);
}
