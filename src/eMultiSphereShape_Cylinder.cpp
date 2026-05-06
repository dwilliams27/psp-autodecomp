// ODR-WARNING: this TU intentionally redeclares eMultiSphereShape with a
// minimal member list for methods absent from the canonical header. Do not
// include eMultiSphereShape.h here unless all matched siblings are rechecked.
struct mVec3;
struct mOCS;
class eShape;
class eCylinderShape;
class eCollisionContactInfo;

class eMultiSphereShape {
public:
    char _pad[0x80];
    float mRadius;
    float mHalfLength;

    int Collide(const eCylinderShape *, int, int, const mOCS &, const mOCS &,
                eCollisionContactInfo *) const;
    void GetInertialTensor(float, mVec3 *) const;
};

class eCollision {
public:
    static int ShapeMultiSphere(const eShape &, const eMultiSphereShape &,
                                const mOCS &, const mOCS &,
                                eCollisionContactInfo *);
};

#pragma control sched=1
int eMultiSphereShape::Collide(const eCylinderShape *shape, int, int,
                               const mOCS &ocs1, const mOCS &ocs2,
                               eCollisionContactInfo *info) const {
    int hit = eCollision::ShapeMultiSphere(*(const eShape *)shape, *this, ocs2,
                                           ocs1, info);
    int count;
    char *p;
    int i = 0;
    if (hit != 0) {
        count = *(int *)((char *)info + 0x14);
        if (i < count) {
            p = (char *)info + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
                count = *(int *)((char *)info + 0x14);
            } while (i < count);
        }
        return 1;
    }
    return 0;
}
#pragma control sched=2

#pragma control sched=1
void eMultiSphereShape::GetInertialTensor(float mass, mVec3 *out) const {
    float radius = mRadius;
    float rSq = radius * radius;
    float cylVol = (rSq * 3.1415927f) * (mHalfLength * 2.0f);
    float sphereVol = (rSq * radius) * 4.1887903f;
    float totalVol = cylVol + sphereVol;
    float cylMass = (cylVol / totalVol) * mass;
    float sphereMass = (sphereVol / totalVol) * mass;
    float sphereI = (sphereMass * 0.4f) * rSq;
    float cylZ = (cylMass * 0.5f) * rSq;
    float cylXY = (cylMass * (1.0f / 12.0f)) *
                  (rSq * 3.0f + (mHalfLength * 2.0f) * (mHalfLength * 2.0f));
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
