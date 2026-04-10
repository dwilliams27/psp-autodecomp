#include "gcDoUIFade.h"

void gcValBinaryOp::SetChild(int index, gcExpression *child) {
    gcValBinaryOpData *self = (gcValBinaryOpData *)this;
    self->mChildren[index] = child;
}
