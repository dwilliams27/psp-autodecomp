// ODR-WARNING: this TU intentionally redeclares eBipedController with only the
// minimal layout needed for UpdateGroundMovement so we do not edit the shared
// header. Split-TU pattern.
//
// The zero-vector store path uses the SNC mtc1/mfc1/mtv/sv.q construction
// pattern (docs/research/snc-constructor-codegen.md). There is no C/C++
// equivalent for the GPR->VFPU "mtv" lane moves, so a small inline-asm block is
// used for that path only. The dominant body logic is plain C float math.

class mVec3;

class eBipedController {
public:
    char _pad0[0x3C];
    unsigned char mGroundFlag;   // 0x3C
    char _pad1[0x3];
    float mGroundStep;           // 0x40

    void UpdateGroundMovement(mVec3 *, mVec3 *);
};

#pragma control sched=2

void eBipedController::UpdateGroundMovement(mVec3 *a1, mVec3 *a2) {
    if (mGroundFlag != 0) {
        float zero = 0.0f;
        __asm__ volatile("" : "+f"(zero));
        float z2 = ((float *)a2)[2];
        if (z2 > zero) {
            float z1 = ((float *)a1)[2];
            if (z1 > zero) {
                float d = z2 - mGroundStep;
                if (d < zero) {
                    d = zero;
                }
                float r = z1 - d;
                if (r < zero) {
                    r = zero;
                }
                ((float *)a1)[2] = r;
                mGroundFlag = 0;
                ((float *)a2)[2] = zero;
                return;
            }
        }
        int x = (int &)zero;
        int y, w;
        __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
        __asm__ volatile("mfc1 %0, %1" : "=r"(w) : "f"(zero));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :: "r"(x), "r"(y), "r"(w), "r"(a2) : "memory");
    }
}
