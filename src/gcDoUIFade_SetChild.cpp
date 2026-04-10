#include "gcDoUIFade.h"
#include "cBase.h"

struct gcDoUIFadeChildData {
    char _pad[0x78];
    gcExpression **mChildren;
};

void gcDoUIFade::SetChild(int index, gcExpression *child) {
    gcDoUIFadeChildData *self = (gcDoUIFadeChildData *)this;
    self->mChildren[index] = child;
}
