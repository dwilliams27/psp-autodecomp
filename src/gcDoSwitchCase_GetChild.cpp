#include "gcDoReturn.h"

gcExpression *gcDoSwitchCase::GetChild(int index) const {
    gcDoSwitchCaseData *self = (gcDoSwitchCaseData *)this;
    return self->mChildren[index];
}
