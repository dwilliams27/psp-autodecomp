// ePathPoint::ePathPoint(void) @ 0x0005ab78
// Symbol: __0oKePathPointctv
// Default ctor: VFPU quad store of zeros at 0..12, two swc1 zeros at 16,20.

struct ePathPoint {
    float a;   // 0x00
    float b;   // 0x04
    float c;   // 0x08
    float d;   // 0x0c
    float e;   // 0x10
    float f;   // 0x14
    char _pad[0x08];
public:
    ePathPoint();
};

ePathPoint::ePathPoint() {
    float z = 0.0f;
    int r1, r2, r3;
    __asm__ volatile("mfc1 %0, %1" : "=r"(r1) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(r2) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(r3) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(r1), "r"(r2), "r"(r3), "r"(this) : "memory"
    );
    e = z;
    f = z;
}
