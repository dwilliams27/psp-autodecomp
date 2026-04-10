#include "gcDoUIFade.h"

gcExpression *gcValBinaryOp::GetChild(int index) const {
    gcValBinaryOpData *self = (gcValBinaryOpData *)this;
    return self->mChildren[index];
}
