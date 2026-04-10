#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct gcDoEntitySendPartialControllerMessageData2 {
    char _pad[0x68];
    gcExpression **mChildren;
};

void gcDoEntitySendPartialControllerMessage::SetChild(int index, gcExpression *child) {
    gcDoEntitySendPartialControllerMessageData2 *self = (gcDoEntitySendPartialControllerMessageData2 *)this;
    self->mChildren[index] = child;
}
