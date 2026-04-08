#include "gcDoUIFade.h"

void gcDoWhile::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

int gcValBinaryOp::GetMaxChildren(void) const {
    return 2;
}

float gcValCameraFollowEntity3rdVariable::Evaluate(void) const {
    return 0.0f;
}

void gcValCameraFollowEntity3rdVariable::Set(float) {
}

float gcValCameraValue::Evaluate(void) const {
    return 0.0f;
}
