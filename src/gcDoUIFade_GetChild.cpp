#include "gcDoUIFade.h"
#include "cBase.h"

struct gcDoUIFadeChildData {
    char _pad[0x78];
    gcExpression **mChildren;
};

gcExpression *gcDoUIFade::GetChild(int index) const {
    gcDoUIFadeChildData *self = (gcDoUIFadeChildData *)this;
    return self->mChildren[index];
}
