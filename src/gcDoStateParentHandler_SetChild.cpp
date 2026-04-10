#include "gcDoReturn.h"
#include "cBase.h"

struct gcDoStateParentHandlerData {
    char _pad[0x0C];
    gcExpression **mChildren;
};

void gcDoStateParentHandler::SetChild(int index, gcExpression *child) {
    gcDoStateParentHandlerData *self = (gcDoStateParentHandlerData *)this;
    self->mChildren[index] = child;
}
