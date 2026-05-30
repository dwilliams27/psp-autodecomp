// mBSpline::Interpolate (static), 0x001a2bac, 140B, leaf.
//   out = sum over i in [-2,1] of w[i] * pts[ clamp(n+i, 0, count-1) * stride ]
// VFPU: accumulator lives in *out (lv.q/sv.q each iteration); weighted control
// point via vscl.t; accumulate via vadd.t. Native v4sf loads, asm only for the
// VFPU compute ops (vscl.t/vadd.t which have no C equivalent).
//
// ODR-WARNING: minimal local declarations only. Do not include shared headers;
// declare the class/method locally (split-TU pattern).
//
// LOAD-BEARING LOCAL NAMES: the loop locals are deliberately named `v1819`
// (the induction var) and `v41408` (the clamped index). This is the exact
// form the register-aware permuter found. SNC canonicalizes commutative
// `addu`/`mult` operand order in a way that is sensitive to local-name
// hashing, so renaming these two identifiers (even a pure rename) flips
// `addu v0,a0,t2` -> `addu v0,t2,a0` and `mult t0,t3` -> `mult t3,t0`,
// breaking the byte match (3/140 bytes). Do NOT rename them.

typedef int v4sf_t __attribute__((mode(V4SF)));
typedef int vs_t __attribute__((mode(VS)));
typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 qw;
};

class mBSpline {
public:
    static void Interpolate(const int n, const float *w, const mVec3 *pts,
                            int count, int stride, mVec3 *out);
};

void mBSpline::Interpolate(const int n, const float *w, const mVec3 *pts,
                           int count, int stride, mVec3 *out) {
    // Zero the accumulator (x,y,z) -> mtc1/mfc1/mtv/sv.q pattern.
    {
        float z = 0.0f;
        int gx, gy, gz;
        __asm__ volatile("mfc1 %0, %1" : "=r"(gx) : "f"(z));
        __asm__ volatile("mfc1 %0, %1" : "=r"(gy) : "f"(z));
        __asm__ volatile("mfc1 %0, %1" : "=r"(gz) : "f"(z));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :: "r"(gx), "r"(gy), "r"(gz), "r"(out) : "memory"
        );
    }

    int v1819 = -2;
    int hi = count - 1;
    do {
        int v41408 = n + v1819;
        if (v41408 > 0) {
            if (v41408 >= hi) v41408 = hi;
        } else {
            v41408 = 0;
        }
        int off = (v41408) * stride;
        vs_t wv = __builtin_ftovs(w[v1819 + 2]);
        v4sf_t acc = *(const v4sf_t *)out;
        const mVec3 *cp = (const mVec3 *)((const char *)pts + off);
        v4sf_t p = *(const v4sf_t *)cp;
        __asm__ volatile(
            "vscl.t %1, %1, %2\n"
            "vadd.t %0, %0, %1\n"
            : "+v"(acc), "+v"(p)
            : "v"(wv)
        );
        *(v4sf_t *)out = acc;
        v1819++;
    } while (v1819 < 2);
}
