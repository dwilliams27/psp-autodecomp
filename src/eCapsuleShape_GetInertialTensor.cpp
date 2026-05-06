// ODR-WARNING: this TU intentionally redeclares eCapsuleShape with a minimal
// member list for a method absent from the canonical header. Do not include
// eCapsuleShape.h here unless all matched siblings are rechecked.
struct mVec3;

class eCapsuleShape {
public:
    char _pad[0x80];
    float radius;
    float halfHeight;

    void GetInertialTensor(float, mVec3 *) const;
};

#pragma control sched=1
void eCapsuleShape::GetInertialTensor(float mass, mVec3 *out) const {
    float r = radius;
    float rSq = r * r;
    float cylVol = (rSq * 3.1415927f) * (halfHeight * 2.0f);
    float sphereBase = r;
    __asm__ volatile("mul.s %0, %1, %0" : "+f"(sphereBase) : "f"(rSq));
    float sphereVol = sphereBase * 4.1887903f;
    float totalVol = cylVol + sphereVol;
    float cylMass = (cylVol / totalVol) * mass;
    float sphereMass = (sphereVol / totalVol) * mass;
    float sphereI = (sphereMass * 0.4f) * rSq;
    float cylZ = (cylMass * 0.5f) * rSq;
    float cylXY = (cylMass * (1.0f / 12.0f)) *
                  (rSq * 3.0f + (halfHeight * 2.0f) * (halfHeight * 2.0f));
    __asm__ volatile("add.s %0, %1, %0" : "+f"(cylXY) : "f"(sphereI));
    __asm__ volatile("add.s %0, %0, %1" : "+f"(sphereI) : "f"(cylZ));
    float xy = cylXY;
    float z = sphereI;

    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(xy));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(xy));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "jr $ra\n"
        "sv.q C120, 0($a1)\n"
        "addu $zero, %3, $zero\n"
        ".set pop\n"
        :: "r"(a), "r"(b), "r"(c), "r"(out)
        : "memory"
    );
}
#pragma control sched=2
