#include "gcDoUIFade.h"
#include "cBase.h"

struct gcDoUISendMessageChildData {
    char _pad[0x30];
    gcExpression **mChildren;
};

gcExpression *gcDoUISendMessage::GetChild(int index) const {
    gcDoUISendMessageChildData *self = (gcDoUISendMessageChildData *)this;
    return self->mChildren[index];
}
