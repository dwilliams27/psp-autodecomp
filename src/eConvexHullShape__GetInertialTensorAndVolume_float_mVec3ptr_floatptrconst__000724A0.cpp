// eConvexHullShape::GetInertialTensorAndVolume(float, mVec3 *, float *) const @ 0x000724a0
// Split-TU: eConvexHullShape / eConvexHullUtil declared locally.
// In sched zone within eAll_psp.obj unity build.

typedef int v4sf_t __attribute__((mode(V4SF)));

struct mVec3 {
    float x, y, z;
};

class eConvexHullUtil {
public:
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float pad1C;
    v4sf_t q20;
    v4sf_t q30;
    float m40;

    void AddPolygon(const mVec3 *, unsigned const char *, int);
    void Begin();
    void Finish(float);
};

class eConvexHullShape {
public:
    char pad[0x80];
    const mVec3 *m_verts;        // 0x80
    char pad84[0x8];
    unsigned const char *m_idx;  // 0x8C
    int m_numPolys;              // 0x90
    const unsigned char *m_poly; // 0x94

    void GetInertialTensorAndVolume(float, mVec3 *, float *) const;
};

#pragma control sched=1
void eConvexHullShape::GetInertialTensorAndVolume(float density, mVec3 *out, float *volume) const {
    eConvexHullUtil util;
    util.m00 = 0.0f;
    util.m04 = 0.0f;
    util.m08 = 0.0f;
    util.m0C = 0.0f;
    util.m10 = 0.0f;
    util.m14 = 0.0f;
    util.m18 = 0.0f;
    float z = 0.0f;
    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x20(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(&util));
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x30(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(&util));
    util.m40 = 0.0f;

    util.Begin();
    const unsigned char *poly = m_poly;
    for (int i = 0; i < m_numPolys; i++) {
        unsigned const char *idx = m_idx + poly[0];
        __asm__ volatile("" ::: "memory");
        util.AddPolygon(m_verts, idx, poly[1]);
        poly += 4;
    }
    util.Finish(density);
    *volume = util.m40;
    __asm__ volatile("" ::: "memory");
    *(v4sf_t *)out = util.q30;
}
