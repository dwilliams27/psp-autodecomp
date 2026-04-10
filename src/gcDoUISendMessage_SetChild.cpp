#include "gcDoUIFade.h"
#include "cBase.h"

struct gcDoUISendMessageChildData {
    char _pad[0x30];
    gcExpression **mChildren;
};

void gcDoUISendMessage::SetChild(int index, gcExpression *child) {
    gcDoUISendMessageChildData *self = (gcDoUISendMessageChildData *)this;
    self->mChildren[index] = child;
}
