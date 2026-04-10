#include "gcDoChangeState.h"
#include "cBase.h"

struct gcDoChangeStateData {
    char _pad[0x18];
    gcExpression **mChildren;
};

gcExpression *gcDoChangeState::GetChild(int index) const {
    gcDoChangeStateData *self = (gcDoChangeStateData *)this;
    return self->mChildren[index];
}
