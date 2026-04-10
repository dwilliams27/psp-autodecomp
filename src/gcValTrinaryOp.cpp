#include "gcValTrinaryOp.h"

gcExpression *gcValTrinaryOp::GetChild(int index) const {
    gcValTrinaryOpData *self = (gcValTrinaryOpData *)this;
    return self->mChildren[index];
}
