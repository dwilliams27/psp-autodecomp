typedef unsigned int u32;
typedef signed int s32;

extern "C" u32 isnan(double x) {
    union { double d; s32 w[2]; } u;
    u.d = x;
    s32 lo = u.w[0];
    s32 hi = u.w[1];
    return (u32) (0x7FF00000 - ((hi & 0x7FFFFFFF) | ((u32) (lo | -lo) >> 0x1F))) >> 0x1F;
}
