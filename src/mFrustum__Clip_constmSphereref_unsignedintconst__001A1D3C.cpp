// ODR-WARNING: split translation unit for mFrustum::Clip(const mSphere&, unsigned int) const.
// Keep this local declaration in sync with the real class layout only.

struct mSphere {
    float x, y, z, radius;
};

struct mPlane {
    float x, y, z, w;
};

struct mFrustum {
    mPlane planes[6];
    int Clip(const mSphere &, unsigned int) const;
};

#pragma control sched=2
int mFrustum::Clip(const mSphere &sphere, unsigned int mask) const {
    const mPlane *p = planes;
    const mPlane *end = planes + 6;
    unsigned int bit = 1;
    for (; p != end; p = (const mPlane *)((const char *)p + 0x10), bit <<= 1) {
        if (mask & bit)
            continue;

        // VFPU 3-component dot of plane normal with sphere center.
        register const mPlane *rp __asm__("$t0") = p;
        register const mSphere *rs __asm__("$a1") = &sphere;
        int dotbits;
        __asm__ volatile(
            ".word 0xd9060000\n"   /* lv.q   C120, 0($t0)  */
            ".word 0xd8a70000\n"   /* lv.q   C130, 0($a1)  */
            ".word 0x64878604\n"   /* vdot.t S100, C120, C130 */
            "mfv %0, S100\n"
            : "=r"(dotbits)
            : "r"(rp), "r"(rs)
            : );
        float dot = *(float *)&dotbits;
        float radius = sphere.radius;
        float planew = p->w;
        if (dot + radius < planew)
            return 0;
        if (!(dot - radius < planew))
            mask |= bit;
    }
    return mask;
}
#pragma control sched=2
