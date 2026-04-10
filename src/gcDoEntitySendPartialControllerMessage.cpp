#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct gcDoEntitySendPartialControllerMessageData {
    char _pad[0x68];
    gcExpression **mChildren;
};

gcExpression *gcDoEntitySendPartialControllerMessage::GetChild(int index) const {
    gcDoEntitySendPartialControllerMessageData *self = (gcDoEntitySendPartialControllerMessageData *)this;
    return self->mChildren[index];
}
