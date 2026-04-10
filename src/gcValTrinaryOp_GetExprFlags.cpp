#include "gcValTrinaryOp.h"

int gcValTrinaryOp::GetExprFlags(void) const {
    gcValTrinaryOpData *self = (gcValTrinaryOpData *)this;
    int op = self->mOp;
    int result = 0xb;
    if (op == 8) {
        result = 0;
    }
    return result;
}
