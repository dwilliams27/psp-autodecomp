// eConvexHullUtil::TensorSubExpression(float, float, float, float&, float&, float&) const
// Address: 0x0006e77c, Size: 64B  Obj: eAll_psp.obj
// Symbol: __0fPeConvexHullUtilTTensorSubExpressionfNCBRfNCEK

class eConvexHullUtil {
public:
    void TensorSubExpression(float a, float b, float c,
                             float &out1, float &out2, float &out3) const;
};

#pragma control sched=1

void eConvexHullUtil::TensorSubExpression(float a, float b, float c,
                                          float &out1, float &out2, float &out3) const {
    float ab = a + b;
    float q = ab + c;
    out1 = q;
    __asm__ volatile("" ::: "memory");

    float aaa = a * a;
    float p = aaa + ab * b;
    out2 = p + c * q;

    out3 = a * a * a + (aaa + ab * b) * b + c * out2;
}
