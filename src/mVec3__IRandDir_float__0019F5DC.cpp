// mVec3::IRandDir(float) @ 0x0019f5dc  symbol __0fFmVec3IIRandDirf
// Random unit direction on a spherical cap scaled by mag (polar axis = Y).

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 qw;
    void IRandDir(float mag);
};

extern "C" int cIRand();
extern "C" float sinf(float);
extern "C" float cosf(float);

void mVec3::IRandDir(float mag) {
    float u1 = (float)(unsigned int)cIRand();
    float theta = mag * (1.0f - u1 * 2.328306e-10f * 2.0f) * 0.017453292f;
    float s_theta = sinf(theta);
    float c_theta = cosf(theta);

    float u2 = (float)(unsigned int)cIRand();
    float angle2 = u2 * 2.328306e-10f * 360.0f * 0.017453292f;
    float v95827 = sinf(angle2);
    float c_a2 = cosf(angle2);

    float *p = (float *)this;
    p[1] = c_theta;
    p[0] = c_a2 * s_theta;
    p[2] = s_theta * v95827;
}
