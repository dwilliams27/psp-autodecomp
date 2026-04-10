#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct gcDoFunctionData {
    char _pad[0x10];
    gcExpression **mChildren;
};

gcExpression *gcDoFunction::GetChild(int index) const {
    gcDoFunctionData *self = (gcDoFunctionData *)this;
    return self->mChildren[index];
}
