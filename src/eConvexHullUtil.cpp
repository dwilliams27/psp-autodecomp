// eConvexHullUtil.cpp - decompiled from eAll_psp.obj

#pragma control sched=1

class eConvexHullUtil {
public:
    void TensorSubExpression(float, float, float, float &, float &, float &) const;
};

void eConvexHullUtil::TensorSubExpression(float a, float b, float c, float &P, float &S, float &T) const {
    volatile float *outP = &P;
    volatile float *outS = &S;
    volatile float *outT = &T;

    float sum = a + b;
    float p = sum + c;
    *outP = p;
    __asm__ volatile("" ::: "memory");

    float a2 = a * a;
    float sb = sum * b;
    float s0 = a2 + sb;
    float cp = c * p;
    float s = s0 + cp;
    *outS = s;
    __asm__ volatile("" ::: "memory");

    float a3 = a2 * a;
    float s0b = s0 * b;
    float t0 = a3 + s0b;
    float cs = c * s;
    *outT = t0 + cs;
}

#pragma control sched=2
