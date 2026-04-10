#include "gcDoUIFade.h"

int gcValBinaryOp::GetExprFlags(void) const {
    gcValBinaryOpData *self = (gcValBinaryOpData *)this;
    int op = self->mOp;
    int result = 3;
    switch (op) {
        case 12:
        case 13:
        case 14:
        case 16:
            result = 0xb;
            break;
    }
    return result;
}
