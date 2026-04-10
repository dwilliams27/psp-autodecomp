#include "gcDoReturn.h"
#include "cBase.h"

struct gcDoStateFunctionData {
    char _pad[0x14];
    gcExpression **mChildren;
};

gcExpression *gcDoStateFunction::GetChild(int index) const {
    gcDoStateFunctionData *self = (gcDoStateFunctionData *)this;
    return self->mChildren[index];
}
