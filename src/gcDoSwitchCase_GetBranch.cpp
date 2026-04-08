#include "gcDoReturn.h"

gcExpression *gcDoSwitchCase::GetBranch(int index) const {
    gcDoSwitchCaseData *self = (gcDoSwitchCaseData *)this;
    gcExpression *result = 0;
    if (index == 0) {
        result = self->mBranch;
    }
    return result;
}
