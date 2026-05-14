// ODR-WARNING: this TU intentionally redeclares eBoxShape with a minimal
// member list for a method absent from the canonical header. Do not include
// eBoxShape.h here unless all matched siblings are rechecked.
struct mVec3;

class eBoxShape {
public:
    char _pad[0x80];
    float mHalfExtents[4];

    void GetInertialTensor(float, mVec3 *) const;
};

#pragma control sched=2
void eBoxShape::GetInertialTensor(float mass, mVec3 *out) const {
    float scale = mass / 3.0f;
    const float *h = mHalfExtents;
    float y2 = h[1] * h[1];
    float z2 = h[2] * h[2];
    float xOut = scale * (y2 + z2);
    float x2 = h[0] * h[0];
    float yOut = scale * (x2 + z2);
    float zOut = scale * (x2 + y2);

    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(xOut));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(yOut));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(zOut));
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
