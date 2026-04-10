#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct gcDoFunctionData {
    char _pad[0x10];
    gcExpression **mChildren;
};

void gcDoFunction::SetChild(int index, gcExpression *child) {
    gcDoFunctionData *self = (gcDoFunctionData *)this;
    self->mChildren[index] = child;
}
