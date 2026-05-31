// mRay::Intersect(const mSphere &) const @ 0x001a2350
//   (mAll_psp.obj, leaf). Ray-sphere intersection (boolean).
//
//   if (ray.t == 0) return 0
//   d  = sphere.center - ray.origin          (VFPU vsub.t)
//   distSq = dot(d, d)                        (VFPU vdot.t -> f17)
//   radiusSq = sphere.radius * sphere.radius
//   if (distSq <= radiusSq) return 1          // origin inside sphere
//   proj = dot(d, ray.dir)                    (VFPU vdot.t -> f13)
//   if (proj < 0 && !(distSq <= radiusSq)) return 0
//   perpSq = distSq - proj*proj
//   if (!(perpSq <= radiusSq)) return 0
//   h = sqrt(radiusSq - perpSq)
//   tHit = (distSq <= radiusSq) ? proj + h : proj - h
//   if (ray.t <= 0) return 1
//   if (!(tHit <= ray.t)) return 0
//   return 1
//
// VFPU vsub.t / vdot.t have no C equivalent and live in inline asm with explicit
// VFPU registers (the mRay::Intersect(plane) / mLine sibling pattern). The two dot
// products are exported to float via mfv/mtc1 so all comparisons, the sqrt and the
// branches stay in C. The (distSq <= radiusSq) condition is reused verbatim three
// times so the compiler keeps it as a single source comparison.
//
// ODR-WARNING: minimal local declarations only (split-TU pattern). Do NOT add
// these to include/*.h.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mSphere {
    SceULong128 qw;       // center xyz @ 0x00, radius @ 0x0C
};

struct mRay {
    SceULong128 origin;       // 0x00
    SceULong128 direction;    // 0x10
    float t;                  // 0x20

    int Intersect(const mSphere &sphere) const;
};

extern "C" float sqrtf(float);

int mRay::Intersect(const mSphere &sphere) const {
    float v90064 = this->t;
    if (v90064 == 0.0f) {
        return 0;
    }

    float distSq;
    int v57053;
    __asm__ volatile(
        "lv.q C120, 0x0(%0)\n"            /* C120 = sphere.center */
        "lv.q C130, 0x0(%1)\n"            /* C130 = ray.origin    */
        "vsub.t C120, C120, C130\n"       /* C120 = center - origin */
        "vdot.t S100, C120, C120\n"
        :
        : "r"(&sphere), "r"(this)
    );
    float radius = *(const float *)((const char *)&sphere + 0x0C);
    __asm__ volatile(
        "mfv $a2, S100\n"
        "mtc1 $a2, %0\n"
        : "=f"(distSq)
        :
        : "$6"
    );
    (void)v57053;
    float radiusSq = radius * radius;
    if (distSq <= radiusSq) {
        return 1;
    }

    float proj;
    __asm__ volatile(
        "lv.q C130, 0x10(%1)\n"           /* C130 = ray.direction */
        "vdot.t S100, C120, C130\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"                  /* proj */
        : "=f"(proj)
        : "r"(this)
        : "$4"
    );

    if (proj < 0.0f && !(distSq <= radiusSq)) {
        return 0;
    }

    float perpSq = distSq - proj * proj;
    if (!(perpSq <= radiusSq)) {
        return 0;
    }

    float tHit;
    float v57386 = sqrtf(radiusSq - perpSq);
    if (!(distSq <= radiusSq)) {
        tHit = proj - v57386;
    } else {
        tHit = proj + v57386;
    }

    if (v90064 <= 0.0f || tHit <= v90064) {
        return 1;
    }
    return 0;
}
