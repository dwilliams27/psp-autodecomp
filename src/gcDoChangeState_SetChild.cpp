#include "gcDoChangeState.h"
#include "cBase.h"

struct gcDoChangeStateData {
    char _pad[0x18];
    gcExpression **mChildren;
};

void gcDoChangeState::SetChild(int index, gcExpression *child) {
    gcDoChangeStateData *self = (gcDoChangeStateData *)this;
    self->mChildren[index] = child;
}
