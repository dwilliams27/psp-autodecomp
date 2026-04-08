#include "gcDoReturn.h"

void gcDoSwitchCase::SetChild(int index, gcExpression *child) {
    gcDoSwitchCaseData *self = (gcDoSwitchCaseData *)this;
    self->mChildren[index] = child;
}
