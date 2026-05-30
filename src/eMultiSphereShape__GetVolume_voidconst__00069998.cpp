class eMultiSphereShape {
public:
    char _pad[0x80];
    float mRadius;  // 0x80
    float mHeight;  // 0x84

    float GetVolume(void) const;
};

#pragma control sched=1
float eMultiSphereShape::GetVolume(void) const {
    float v71507 = mRadius;
    float r2 = v71507 * v71507;
    __asm__ volatile("" ::: "memory");
    float a = r2 * 3.1415927f;
    __asm__ volatile("" ::: "memory");
    float h2 = mHeight * 2.0f;
    float lhs = a * h2;
    float r3 = r2;
    r3 = r3 * v71507;
    __asm__ volatile("" ::: "memory");
    float rhs = r3 * 4.1887903f;
    return lhs + rhs;
}
