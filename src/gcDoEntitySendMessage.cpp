#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct gcDoEntitySendMessageData {
    char _pad[0x50];
    gcExpression **mChildren;
};

gcExpression *gcDoEntitySendMessage::GetChild(int index) const {
    gcDoEntitySendMessageData *self = (gcDoEntitySendMessageData *)this;
    return self->mChildren[index];
}

void gcDoEntitySendMessage::SetChild(int index, gcExpression *child) {
    gcDoEntitySendMessageData *self = (gcDoEntitySendMessageData *)this;
    self->mChildren[index] = child;
}
