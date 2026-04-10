#include "gcDoReturn.h"
#include "cBase.h"

struct gcDoStateFunctionData {
    char _pad[0x14];
    gcExpression **mChildren;
};

void gcDoStateFunction::SetChild(int index, gcExpression *child) {
    gcDoStateFunctionData *self = (gcDoStateFunctionData *)this;
    self->mChildren[index] = child;
}
