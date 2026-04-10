#include "gcDoReturn.h"
#include "cBase.h"

struct gcDoStateParentHandlerData {
    char _pad[0x0C];
    gcExpression **mChildren;
};

gcExpression *gcDoStateParentHandler::GetChild(int index) const {
    gcDoStateParentHandlerData *self = (gcDoStateParentHandlerData *)this;
    return self->mChildren[index];
}
