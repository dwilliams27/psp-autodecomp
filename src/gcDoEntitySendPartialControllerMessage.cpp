#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern const char gcDoEntitySendPartialControllerMessage_base_name[];
extern const char gcDoEntitySendPartialControllerMessage_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySendPartialControllerMessage;

struct gcDoEntitySendPartialControllerMessageData {
    char _pad[0x68];
    gcExpression **mChildren;
};

gcExpression *gcDoEntitySendPartialControllerMessage::GetChild(int index) const {
    gcDoEntitySendPartialControllerMessageData *self = (gcDoEntitySendPartialControllerMessageData *)this;
    return self->mChildren[index];
}

const cType *gcDoEntitySendPartialControllerMessage::GetType(void) const {
    if (!type_gcDoEntitySendPartialControllerMessage) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySendPartialControllerMessage_base_name,
                        gcDoEntitySendPartialControllerMessage_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySendPartialControllerMessage = cType::InitializeType(
            0, 0, 0x10B, type_action,
            gcDoEntitySendPartialControllerMessage::New, 0, 0, 0x80);
    }
    return type_gcDoEntitySendPartialControllerMessage;
}

void gcDoEntitySendPartialControllerMessage::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntitySendPartialControllerMessage) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntitySendPartialControllerMessage_base_name,
                            gcDoEntitySendPartialControllerMessage_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntitySendPartialControllerMessage = cType::InitializeType(
                0, 0, 0x10B, type_action,
                gcDoEntitySendPartialControllerMessage::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntitySendPartialControllerMessage;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoEntitySendPartialControllerMessage *)copy;
}
