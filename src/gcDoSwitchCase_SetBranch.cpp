#include "gcDoReturn.h"

void gcDoSwitchCase::SetBranch(int index, gcExpression *expr) {
    gcDoSwitchCaseData *self = (gcDoSwitchCaseData *)this;
    if (index == 0) {
        self->mBranch = expr;
    }
}
