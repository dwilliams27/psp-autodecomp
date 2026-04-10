#include "gcValTrinaryOp.h"

void gcValTrinaryOp::SetChild(int index, gcExpression *child) {
    gcValTrinaryOpData *self = (gcValTrinaryOpData *)this;
    self->mChildren[index] = child;
}
